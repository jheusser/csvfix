//---------------------------------------------------------------------------
// csved_exclude.cpp
//
// exclude fields from output
//
// Copyright (C) 2009 Neil Butterworth
//---------------------------------------------------------------------------

#include "a_base.h"
#include "a_collect.h"
#include "csved_cli.h"
#include "csved_exclude.h"
#include "csved_strings.h"

using std::string;
using std::vector;

namespace CSVED {

//---------------------------------------------------------------------------
// Register exclude command
//---------------------------------------------------------------------------

static RegisterCommand <ExcludeCommand> rc1_(
	CMD_EXCLUDE,
	"exclude fields from output"
);

//----------------------------------------------------------------------------
// Help text
//----------------------------------------------------------------------------

const char * const EXCL_HELP = {
	"exclude specific CSV input fields from output\n"
	"usage: csvfix exclude  [flags] [file ...]\n"
	"where flags are:\n"
	"  -f fields\tlist of fields to exclude\n"
	"#ALL"
};

//----------------------------------------------------------------------------
// Standard command ctor
//---------------------------------------------------------------------------

ExcludeCommand ::ExcludeCommand( const string & name,
								const string & desc )
		: Command( name, desc, EXCL_HELP ) {

	AddFlag( ALib::CommandLineFlag( FLAG_COLS, true, 1 ) );
}

//---------------------------------------------------------------------------
// Get user specifie doptions and then read input, removing excluded cols
//---------------------------------------------------------------------------

int ExcludeCommand :: Execute( ALib::CommandLine & cmd ) {

	ProcessFlags( cmd );

	IOManager io( cmd );
	CSVRow row;

	while( io.ReadCSV( row ) ) {
		CSVRow r = Exclude( row );
		io.WriteRow( r );
	}

	return 0;
}

//---------------------------------------------------------------------------
// Handle all user options with error checking
//---------------------------------------------------------------------------

void ExcludeCommand :: ProcessFlags( const ALib::CommandLine & cmd ) {

	string sn = cmd.GetValue( FLAG_COLS, ""  );
	CommaListToIndex( ALib::CommaList( sn ), mFields );
	if ( mFields.size() == 0 ) {
		CSVTHROW( "Field list  specified by " << FLAG_COLS
				<< " cannot be empty" );
	}
}

//---------------------------------------------------------------------------
// Copy all fields that are not excluded to new row
//---------------------------------------------------------------------------

CSVRow ExcludeCommand :: Exclude( const CSVRow & r ) const {

	CSVRow out;

	for ( unsigned int i = 0; i < r.size(); i++ ) {
		if ( ! ALib::Contains( mFields, i ) ) {
			out.push_back( r.at( i ) );
		}
	}
	return out;
}


} // end namespace

// end

