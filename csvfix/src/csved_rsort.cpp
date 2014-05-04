//---------------------------------------------------------------------------
// csved_rsort.cpp
//
// Do in-row sorting of CSV fields
//
// Copyright (C) 2009 Neil Butterworth
//---------------------------------------------------------------------------

#include "a_base.h"
#include "csved_cli.h"
#include "csved_rsort.h"
#include "csved_strings.h"

using std::string;
using std::vector;

namespace CSVED {

//---------------------------------------------------------------------------
// Register sequence command
//---------------------------------------------------------------------------

static RegisterCommand <RowSortCommand> rc1_(
	CMD_RSORT,
	"in-row sort of CSV fields"
);

//----------------------------------------------------------------------------
// Help text
//----------------------------------------------------------------------------

const char * const ROWSORT_HELP = {
	"in-row sort of CSV fields\n"
	"usage: csvfix rowsort [flags] [files ...]\n"
	"where flags are:\n"
	"  -f fields\tfields to sort (default is all)\n"
	"  -a \t\tsort ascending (default)\n"
	"  -d \t\tsort descending \n"
	"  -l \t\tlexographic order (default)\n"
	"  -n \t\tnumeric order\n"
	"#SMQ,SEP,IBL,IFN,OFL,SKIP,PASS"
};

//------------------------------------------------------------------------
// Standard command ctor
//---------------------------------------------------------------------------

RowSortCommand :: RowSortCommand( const string & name,
								const string & desc )
		: Command( name, desc, ROWSORT_HELP ) {

}

//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

int RowSortCommand :: Execute( ALib::CommandLine & cmd ) {

	GetSkipOptions( cmd );
	ProcessFlags( cmd );

	IOManager io( cmd );
	CSVRow row;

	while( io.ReadCSV( row ) ) {
		if( Skip( row ) ) {
			continue;
		}
		if ( ! Pass( row ) ) {
		}
		io.WriteRow( row );
	}

	return 0;
}

//---------------------------------------------------------------------------
// Handle all user options with error checking
//---------------------------------------------------------------------------

void RowSortCommand :: ProcessFlags( const ALib::CommandLine & cmd ) {

}

} // end namespace

// end

