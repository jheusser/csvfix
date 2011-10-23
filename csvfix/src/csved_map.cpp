//---------------------------------------------------------------------------
// csved_map.cpp
//
// map from one value to another
//
// Copyright (C) 2009 Neil Butterworth
//---------------------------------------------------------------------------

#include "a_base.h"
#include "a_collect.h"
#include "csved_cli.h"
#include "csved_map.h"
#include "csved_strings.h"

using std::string;
using std::vector;

namespace CSVED {

//---------------------------------------------------------------------------
// Register map command
//---------------------------------------------------------------------------

static RegisterCommand <MapCommand> rc1_(
	CMD_MAP,
	"map betwen CSV field values"
);

//----------------------------------------------------------------------------
// Help text
//----------------------------------------------------------------------------

const char * const MAP_HELP = {
	"map input field values to new values on output\n"
	"usage: csvfix map [flags] [file ...]\n"
	"where flags are:\n"
	"  -f fields\tfields to perform mapping on (default is all)\n"
	"  -fv vals\tcomma-separated list of values to map from\n"
	"  -tv vals\tpossibly empty list oof values to map to\n"
	"  -ic\t\tignore case when mapping (default is to respect case)\n"
	"#ALL"
};

//---------------------------------------------------------------------------
// standard command constructor
//---------------------------------------------------------------------------

MapCommand ::	MapCommand( const string & name,
								const string & desc )
		: Command( name, desc, MAP_HELP ), mIgnoreCase( false ) {

	AddFlag( ALib::CommandLineFlag( FLAG_COLS, false, 1 ) );
	AddFlag( ALib::CommandLineFlag( FLAG_FROMV, true, 1 ) );
	AddFlag( ALib::CommandLineFlag( FLAG_TOV, true, 1 ) );
	AddFlag( ALib::CommandLineFlag( FLAG_ICASE, false, 0 ) );
}

//---------------------------------------------------------------------------
// Process flags then map inputs
//---------------------------------------------------------------------------

int MapCommand :: Execute( ALib::CommandLine & cmd ) {

	ProcessFlags( cmd );

	IOManager io( cmd );
	CSVRow row;

	while( io.ReadCSV( row ) ) {
		DoMapping( row );
		io.WriteRow( row );
	}

	return 0;
}

//---------------------------------------------------------------------------
// Do mapping for single row. If  alist of fields was not specified on
// command line, map all fields.
//---------------------------------------------------------------------------

void MapCommand :: DoMapping( CSVRow & row )  {

	if ( mFields.size() == 0 ) {
		for ( unsigned int i = 0; i < row.size(); i++ ) {
			MapValue( row[i] );
		}
	}
	else {
		for ( unsigned int i = 0; i < mFields.size(); i++ ) {
			if ( mFields[i] < row.size() ) {
				MapValue( row[ mFields[i] ] );
			}
		}
	}
}


//---------------------------------------------------------------------------
// Map single value
//---------------------------------------------------------------------------

void MapCommand :: MapValue( std::string & val ) {
	for ( unsigned int i = 0; i < mFrom.Size(); i++ ) {
		if ( (mIgnoreCase && ALib::Equal( val, mFrom.At(i) ))
				|| ( ! mIgnoreCase && val == mFrom.At(i)) ) {
			if ( mTo.Size() == 0 ) {
				val = "";
			}
			else if ( mTo.Size() == mFrom.Size() ) {
				val = mTo.At( i );
			}
			else {
				val = mTo.At( mTo.Size() - 1 );
			}
			break;
		}
	}
}

//---------------------------------------------------------------------------
// Process command line flags. Need a list of fields and from and to values
//---------------------------------------------------------------------------

void MapCommand :: ProcessFlags( ALib::CommandLine & cmd ) {
	mIgnoreCase = cmd.HasFlag( FLAG_ICASE );

	if ( cmd.HasFlag( FLAG_COLS ) ) {
		CommaListToIndex( ALib::CommaList( cmd.GetValue( FLAG_COLS )), mFields );
		if ( mFields.size() == 0 ) {
			CSVTHROW( "Field list cannot be empty" );
		}
	}

	mFrom = ALib::CommaList( cmd.GetValue( FLAG_FROMV ));
	if ( mFrom.Size() == 0 ) {
		CSVTHROW( "Need from values specified by " << FLAG_FROMV );
	}
	mTo = ALib::CommaList( cmd.GetValue( FLAG_TOV ));
	if ( mTo.Size() > mFrom.Size() ) {
		CSVTHROW( "List of 'to values' longer than list of 'from values" );
	}
}

//------------------------------------------------------------------------

} // end namespace

// end

