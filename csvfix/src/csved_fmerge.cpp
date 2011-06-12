//---------------------------------------------------------------------------
// csved_fmerge.cpp
//
// merge multiple sorted csv files
//
// Copyright (C) 20011 Neil Butterworth
//---------------------------------------------------------------------------

#include "a_base.h"
#include "a_collect.h"
#include "csved_cli.h"
#include "csved_fmerge.h"
#include "csved_strings.h"

#include <string>
#include <vector>
#include <memory>

using std::string;

namespace CSVED {


//---------------------------------------------------------------------------
// Register fmerge  command
//---------------------------------------------------------------------------

static RegisterCommand <FMergeCommand> rc1_(
	CMD_FMERGE,
	"merge multiple sorted CSV files"
);

//----------------------------------------------------------------------------
// Help
//----------------------------------------------------------------------------

const char * const FMERGE_HELP = {
	"merge multiple sorted CSV files into single output\n"
	"usage: csvfix fmerge [flags] file ...\n"
	"where flags are:\n"
	"  -f fields\tfields to compare when merging (default all)\n"
	"#ALL"
};

//----------------------------------------------------------------------------
// The fmerge command
//----------------------------------------------------------------------------

FMergeCommand :: FMergeCommand( const string & name, const string & desc )
				: Command( name, desc, FMERGE_HELP ) {

	AddFlag( ALib::CommandLineFlag( FLAG_COLS, false, 1 ) );

}

bool FMergeCommand :: GetARow( CSVRow & row ) {

	for ( unsigned int i = 0; i < mInputs.size(); i++ ) {
		if ( mInputs[i]->ParseNext( row ) ) {
			return true;
		}
	}
	return false;

}

bool FMergeCommand :: OutputOneRow( IOManager & io ) {

	for ( unsigned int i = 0; i < mInputs.size(); i++ ) {

		if ( mInputs[i] ->ParseNext( row ) ) {

		}
}

int FMergeCommand :: Execute( ALib::CommandLine & cmd ) {

	ProcessFlags( cmd );
	IOManager io( cmd );

	mInputs.clear();
	for ( unsigned int i = 0; i < io.InStreamCount(); i++ ) {
		mInputs.push_back( SpType( io.CreateStreamParser( i ) ) );
	}

	CSVRow arow;
	if ( ! GetARow( arow ) ) {
		ATHROW( "Empty inputs" );
	}
	mOutStack.push( arow );

	while ( OutputOneRow( io ) {

	}

}

bool FMergeCommand :: HaveInput() const {
	return mInputs.size() != 0;
}

//----------------------------------------------------------------------------
// Get command line options
//----------------------------------------------------------------------------

void FMergeCommand :: ProcessFlags( ALib::CommandLine & cmd ) {

	if ( cmd.HasFlag( FLAG_COLS ) ) {
		ALib::CommaList cl( cmd.GetValue( FLAG_COLS ) );
		CommaListToIndex( cl, mFields );
	}
}

//----------------------------------------------------------------------------

} // namespace





