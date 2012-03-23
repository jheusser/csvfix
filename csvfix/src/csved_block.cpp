//---------------------------------------------------------------------------
// csved_block.cpp
//
// perform actions on blocks of CSV records
//
// Copyright (C) 2012 Neil Butterworth
//---------------------------------------------------------------------------

#include "a_base.h"
#include "a_collect.h"
#include "csved_cli.h"
#include "csved_block.h"
#include "csved_evalvars.h"
#include "csved_strings.h"
#include <string>
using std::string;

namespace CSVED {


//---------------------------------------------------------------------------
// Register block command
//---------------------------------------------------------------------------

static RegisterCommand <BlockCommand> rc1_(
	CMD_BLOCK,
	"perform actions on blocks of CSV records"
);


//----------------------------------------------------------------------------
// Help
//----------------------------------------------------------------------------

const char * const BLOCK_HELP = {
	"/ perform actions on blocks of CSV records\n"
	"usage: csvfix block  [flags] [file ...]\n"
	"where flags are:\n"
	"#ALL"
};

//----------------------------------------------------------------------------
// The block command
//----------------------------------------------------------------------------

BlockCommand :: BlockCommand( const string & name, const string & desc )
				: Command( name, desc, BLOCK_HELP ) ,
					mAction( None ), mExclusive( false ) {

	AddFlag( ALib::CommandLineFlag( FLAG_BEXPR, true, 1 ) );
	AddFlag( ALib::CommandLineFlag( FLAG_EEXPR, false, 1 ) );
	AddFlag( ALib::CommandLineFlag( FLAG_ACTKEEP, false, 0 ) );
	AddFlag( ALib::CommandLineFlag( FLAG_ACTREM, false, 0 ) );
	AddFlag( ALib::CommandLineFlag( FLAG_BLKEXC, false, 0 ) );
}


//----------------------------------------------------------------------------
///----------------------------------------------------------------------------

enum State { Outside, Inside };

int BlockCommand :: Execute( ALib::CommandLine & cmd ) {

	ProcessFlags( cmd );
	IOManager io( cmd );
	CSVRow row;
	State state = Outside;
	bool block = true;

	while( io.ReadCSV( row ) ) {
		if ( state == Outside ) {
			AddVars( mBeginEx, io, row );
			if ( AtBeginBlock() ) {
				block = ! mExclusive;
				state = Inside;
			}
			else {
				block = false;
			}
		}
		else if ( state == Inside ) {
			AddVars( mEndEx, io, row );
			if ( AtEndBlock() ) {
				block = ! mExclusive;
				state = Outside;
			}
			else {
				block = true;
			}
		}
		else {
			CSVTHROW( "Bad state" );
		}

		//std::cout << "Block is " << block << std::endl;
		if ( (block && mAction == Keep) || (!block && mAction == Remove) ) {
			io.WriteRow( row );
		}
	}
	return 0;
}

bool BlockCommand :: AtEndBlock()  {
	return ALib::Expression::ToBool( mEndEx.Evaluate() );
}

bool BlockCommand :: AtBeginBlock()  {
	return ALib::Expression::ToBool( mBeginEx.Evaluate() );
}

//----------------------------------------------------------------------------
// Get command line options.
//----------------------------------------------------------------------------

void BlockCommand :: ProcessFlags( const ALib::CommandLine & cmd ) {

	string beginex = cmd.GetValue( FLAG_BEXPR );
	string emsg = mBeginEx.Compile( beginex );
	if ( emsg != "" ) {
		CSVTHROW( "Invalid expression for " << FLAG_BEXPR );
	}

	string endex = cmd.GetValue( FLAG_EEXPR, beginex );
	emsg = mEndEx.Compile( endex );
	if ( emsg != "" ) {
		CSVTHROW( "Invalid expression for " << FLAG_EEXPR );
	}

	if ( cmd.HasFlag( FLAG_ACTREM ) && cmd.HasFlag( FLAG_ACTKEEP ) ) {
		CSVTHROW( "Can specify only one of "
						<< FLAG_ACTKEEP << " or " << FLAG_ACTREM );
	}
	if ( cmd.HasFlag( FLAG_ACTREM ) ) {
		mAction = Remove;
	}
	else if ( cmd.HasFlag( FLAG_ACTKEEP ) ) {
		mAction = Keep;
	}
	else {
		CSVTHROW( "Need one of " << FLAG_ACTKEEP << " or " << FLAG_ACTREM );
	}
	mExclusive = cmd.HasFlag( FLAG_BLKEXC );
}


//----------------------------------------------------------------------------

} // namespace





