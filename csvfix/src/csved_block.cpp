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
				: Command( name, desc, BLOCK_HELP ) {

	AddFlag( ALib::CommandLineFlag( FLAG_BEXPR, false, 1 ) );
	AddFlag( ALib::CommandLineFlag( FLAG_EEXPR, false, 1 ) );
	AddFlag( ALib::CommandLineFlag( FLAG_ACTKEEP, false, 0 ) );
	AddFlag( ALib::CommandLineFlag( FLAG_ACTREM, false, 0 ) );
	AddFlag( ALib::CommandLineFlag( FLAG_BLKEXC, false, 0 ) );
}


//----------------------------------------------------------------------------
///----------------------------------------------------------------------------

int BlockCommand :: Execute( ALib::CommandLine & cmd ) {
	ProcessFlags( cmd );
	return 0;
}

//----------------------------------------------------------------------------
// Get command line options.
//----------------------------------------------------------------------------


void BlockCommand :: ProcessFlags( const ALib::CommandLine & cmd ) {
	string beginex = cmd.GetValue( FLAG_BEXPR, "" );
	if ( beginex != "" ) {
		mBeginEx.Compile( beginex );
	}
	string endex = cmd.GetValue( FLAG_EEXPR, "" );
	if ( endex != "" ) {
		mEndEx.Compile( endex );
	}
	if ( endex == "" && beginex == "" ) {
		CSVTHROW( "Need " << FLAG_BEXPR << " and/or " << FLAG_EEXPR );
	}
	if ( cmd.HasFlag( FLAG_ACTREM ) && cmd.HasFlag( FLAG_ACTKEEP ) ) {
		CSVTHROW( "Can specify only one of "
						<< FLAG_ACTKEEP << " or " << FLAG_ACTREM );
	}
	if ( cmd.HasFlag( FLAG_ACTREM ) ) {
		mAction = Remove;
	}
	if ( cmd.HasFlag( FLAG_ACTKEEP ) ) {
		mAction = Keep;
	}
	else {
		CSVTHROW( "Need one of " << FLAG_ACTKEEP << " or " << FLAG_ACTREM );
	}
	mExclusive = cmd.HasFlag( FLAG_BLKEXC );
}

//----------------------------------------------------------------------------

} // namespace





