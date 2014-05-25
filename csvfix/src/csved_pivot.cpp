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
	"  -a act\taction to perform (one of sum, avg, count)\n"
	"  -f field\tfield that represents the fact you want to perform action on\n"
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
	AddFlag( ALib::CommandLineFlag( FLAG_FACT, true, 1 ) );

}

//---------------------------------------------------------------------------
// Pivot the data!
//---------------------------------------------------------------------------

int PivotCommand :: Execute( ALib::CommandLine & cmd ) {

	GetSkipOptions( cmd );
    ProcessFlags( cmd );
	IOManager io( cmd );
	CSVRow row;
    unsigned int rowcount = 0;

	while( io.ReadCSV( row ) ) {
        ColRow cr = MakeColRow( row );
        AddFact( cr, GetFact( row ));
        mCols.insert( row[mCol] );
        mRows.insert( row[mRow] );
        rowcount++;
	}

    OutputPivot( io, rowcount );

	return 0;
}

//---------------------------------------------------------------------------
// Output the pivot data
//---------------------------------------------------------------------------

void PivotCommand :: OutputPivot( IOManager & io, unsigned int rowcount ) {

    CSVRow r;
    r.push_back( "" );    // corresponds to row header
    for ( auto col : mCols ) {
        r.push_back( col );
    }
    io.WriteRow( r );

    for( auto row : mRows ) {
        r.clear();
        r.push_back( row );
        for ( auto col : mCols ) {
            ColRow cr( col, row );
            double val = mColRowValMap[ cr ];
            r.push_back( ALib::Str( val ) );
        }
        io.WriteRow( r );
    }


}

//---------------------------------------------------------------------------
// Add fact value to total, or simply count it.
//---------------------------------------------------------------------------

void PivotCommand :: AddFact( const ColRow & cr, const string & fact )  {
    if ( (mAction == Action::Average || mAction == Action::Sum) ) {
        if ( ! ALib::IsNumber( fact ) ) {
            CSVTHROW( "Non-numeric fact: " << fact);
        }
        double fd = ALib::ToReal( fact );
        mColRowValMap[cr] += fd;
    }
    else {
        mColRowValMap[cr] += 1;
    }
}

//---------------------------------------------------------------------------
// Get fact field as string.
//---------------------------------------------------------------------------

string PivotCommand :: GetFact( const CSVRow & row ) const {
    if ( mFact >= row.size() ) {
        CSVTHROW( "Invalid fact index: " << mFact );
    }
    return row[mFact];
}

//---------------------------------------------------------------------------
// Create ColRow combination object from two CSV fields.
//---------------------------------------------------------------------------

PivotCommand::ColRow PivotCommand:: MakeColRow( const CSVRow & row ) const {

    if ( mCol >= row.size() || mRow >= row.size() ) {
        CSVTHROW( "Invalid row/column index" );
    }

    return PivotCommand::ColRow( row[mCol], row[mRow] );
}

//---------------------------------------------------------------------------
// Get positive integer value for row ,column ad fact  fields
//---------------------------------------------------------------------------

static unsigned int GetField( const ALib::CommandLine & cmd,
                                const string & option ) {
    string rc = cmd.GetValue( option );
    if ( ! ALib::IsInteger( rc ) ) {
        CSVTHROW( "Value for " << option << " must be integer" );
    }
    int n = ALib::ToInteger( rc );
    if ( n <= 0 ) {
        CSVTHROW( "Value for " << option << " must be greater than zero" );
    }
    return (unsigned int) n - 1;
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
    mRow = GetField( cmd, FLAG_ROW );
    mCol = GetField( cmd, FLAG_COL );
    mFact = GetField( cmd, FLAG_FACT );
    if ( mRow == mCol ) {
        CSVTHROW( "Row and column options cannot have the same value" );
    }
    mAction = GetAction( cmd );
}

//------------------------------------------------------------------------

} // end namespace

// end

