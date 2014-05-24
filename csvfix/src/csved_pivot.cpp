//---------------------------------------------------------------------------
// csved_pivot.cpp
//
// Simple pivot table stuff.
//
// Copyright (C) 2014 Neil Butterworth
//---------------------------------------------------------------------------

#include "a_base.h"
#include "a_collect.h"
#include "csved_cli.h"
#include "csved_pivot.h"
#include "csved_strings.h"

using std::string;
using std::vector;

namespace CSVED {

//---------------------------------------------------------------------------
// Possible pivoting operations
//---------------------------------------------------------------------------

const char * const OP_SUM = "sum";
const char * const OP_COUNT = "count";
const char * const OP_AVG = "avg";

//---------------------------------------------------------------------------
// Register pivot command
//---------------------------------------------------------------------------

static RegisterCommand <PivotCommand> rc1_(
	CMD_PIVOT,
	"simple pivot tables"
);

//----------------------------------------------------------------------------
// Help text
//----------------------------------------------------------------------------

const char * const PIVOT_HELP = {
	"performs simple pivot table operations on CSV input\n"
	"usage: csvfix pivot [flags] [file ...]\n"
	"where flags are:\n"
	"  -c field\tfield to use for column headers\n"
	"  -r field\tfield to use for row header \n"
	"  -a act\taction to perfrm (one of sum, avg, count)\n"
	"#ALL,SKIP,PASS"
};

//---------------------------------------------------------------------------
// standard command constructor
//---------------------------------------------------------------------------

PivotCommand :: PivotCommand( const string & name,
								const string & desc )
		: Command( name, desc, PIVOT_HELP ) {

	AddFlag( ALib::CommandLineFlag( FLAG_ACTION, true, 1 ) );
	AddFlag( ALib::CommandLineFlag( FLAG_COL, true, 1 ) );
	AddFlag( ALib::CommandLineFlag( FLAG_ROW, true, 1 ) );

}

//---------------------------------------------------------------------------
// Pivot the data!
//---------------------------------------------------------------------------

int PivotCommand :: Execute( ALib::CommandLine & cmd ) {

	GetSkipOptions( cmd );
	IOManager io( cmd );
	CSVRow row;

	while( io.ReadCSV( row ) ) {
	}

	return 0;
}

//---------------------------------------------------------------------------
// Get positive integer value for row and column fields
//---------------------------------------------------------------------------

unsigned int GetRowCol( const ALib::CommandLine & cmd, const string & option ) {
    string rc = cmd.GetValue( option );
    if ( ! ALib::IsInteger( rc ) ) {
        CSVTHROW( "Value for " << option << " must be integer" );
    }
    int n = ALib::ToInteger( rc );
    if ( n <= 0 ) {
        CSVTHROW( "Value for " << option << " must be greater than zero" );
    }
    return (unsigned int) n;
}

//---------------------------------------------------------------------------
// Get the action to perform when pivotting.
//---------------------------------------------------------------------------

PivotCommand::Action GetAction( const ALib::CommandLine & cmd ) {

    string as = cmd.GetValue( FLAG_ACTION );
    if ( as == OP_SUM ) {
        return PivotCommand::Action::Sum;
    }
    else if ( as == OP_COUNT ) {
        return PivotCommand::Action::Count;
    }
    else if ( as == OP_AVG ) {
        return PivotCommand::Action::Average;
    }
    else {
        CSVTHROW( "Invalid value for " << FLAG_ACTION << ". Need one of sum, count, average" );
    }

}

//---------------------------------------------------------------------------
// Get options - all are requred.
//---------------------------------------------------------------------------

void PivotCommand :: ProcessFlags( const ALib::CommandLine & cmd ) {
    mRow = GetRowCol( cmd, FLAG_ROW );
    mCol = GetRowCol( cmd, FLAG_COL );
    if ( mRow == mCol ) {
        CSVTHROW( "Row and column options cannot have the same value" );
    }
    mAction = GetAction( cmd );
}

//------------------------------------------------------------------------

} // end namespace

// end

