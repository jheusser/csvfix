//---------------------------------------------------------------------------
// csved_erase.cpp
//
// Erase fields from CSV records using regualr expressions
//
// Copyright (C) 2009 Neil Butterworth
//---------------------------------------------------------------------------

#include "a_base.h"
#include "a_collect.h"
#include "csved_cli.h"
#include "csved_erase.h"
#include "csved_strings.h"
#include "csved_evalvars.h"

using std::string;
using std::vector;

namespace CSVED {

//---------------------------------------------------------------------------
// Register exclude command
//---------------------------------------------------------------------------

static RegisterCommand <EraseCommand> rc1_(
	CMD_ERASE,
	"erase fields using regular expressions"
);

//----------------------------------------------------------------------------
// Help text
//----------------------------------------------------------------------------

const char * const ERASE_HELP = {
	"erase fields using regular expressions"
	"usage: csvfix erase  [flags] [file ...]\n"
	"where flags are:\n"
	"  -f fields\tlist of fields to exclude (default is all)\n"
	"  -r regexp\terase fields that match this regular expression\n"
    "  -n regex\terase fields which do not match this regular expression\n"
	"  -k\t\tif all fields are erased, retain empty row (default is to delete it)\n"
	"#ALL,SKIP,PASS"
};

//----------------------------------------------------------------------------
// Standard command ctor
//---------------------------------------------------------------------------

EraseCommand ::EraseCommand( const string & name,
								const string & desc )
		: Command( name, desc, ERASE_HELP ) {

}

//---------------------------------------------------------------------------
// Get user-specified options and then read input, removing erased cols.
//---------------------------------------------------------------------------

int EraseCommand :: Execute( ALib::CommandLine & cmd ) {

	GetSkipOptions( cmd );
	ProcessFlags( cmd );

	IOManager io( cmd );
	CSVRow row;

	while( io.ReadCSV( row ) ) {
		if ( Skip( io, row ) ) {
			continue;
		}
		if ( ! Pass( io, row ) ) {
		}
		io.WriteRow( row );
	}

	return 0;
}


//---------------------------------------------------------------------------
// Handle all user options with error checking. Currently possible to pass
// empty expression, but probably shouldn't be.
//---------------------------------------------------------------------------

void EraseCommand :: ProcessFlags( const ALib::CommandLine & cmd ) {

	NotBoth( cmd, FLAG_REVCOLS, FLAG_COLS, ReqOp::Required );

}



} // end namespace

// end

