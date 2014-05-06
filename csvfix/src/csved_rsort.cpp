//---------------------------------------------------------------------------
// csved_rsort.cpp
//
// Do in-row sorting of CSV fields
//
// Copyright (C) 2009 Neil Butterworth
//---------------------------------------------------------------------------

#include "a_base.h"
#include "a_collect.h"
#include "csved_cli.h"
#include "csved_rsort.h"
#include "csved_strings.h"

#include <algorithm>

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

//---------------------------------------------------------------------------
// Standard command ctor
//---------------------------------------------------------------------------

RowSortCommand :: RowSortCommand( const string & name,
								const string & desc )
		: Command( name, desc, ROWSORT_HELP ),
            mStartPos(UINT_MAX), mSortAscending( true ), mSortLex( true ){
   	AddFlag( ALib::CommandLineFlag( FLAG_COLS, false, 1, false) );
   	AddFlag( ALib::CommandLineFlag( FLAG_ASC, false, 0, false) );
   	AddFlag( ALib::CommandLineFlag( FLAG_DESC, false, 0, false) );
   	AddFlag( ALib::CommandLineFlag( FLAG_LEX, false, 0, false) );
   	AddFlag( ALib::CommandLineFlag( FLAG_NUM, false, 0, false) );
}

//---------------------------------------------------------------------------
// Sort fields in CSV records
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
            SortRow( row );
		}
		io.WriteRow( row );
	}

	return 0;
}

template <typename T>
struct SortDesc {
    bool operator()( const T & a, const T  & b ) {
        return a >= b;
    }
};

//---------------------------------------------------------------------------
// Get the fields to be sorted into a temporary vector
//---------------------------------------------------------------------------

vector <string> RowSortCommand :: GetSortFields( const CSVRow & r )  {
    vector <string> sf;
    for( unsigned int i = 0; i < r.size(); i++ ) {
        if ( mFields.size() == 0 || ALib::Contains( mFields, i ) ) {
            if ( mStartPos == UINT_MAX ) {
                mStartPos = i;
            }
            sf.push_back( r[i] );
        }
    }
    return sf;

}

//---------------------------------------------------------------------------
// Put the sorted fields back in the row for output.
//---------------------------------------------------------------------------

void RowSortCommand ::PutSortFields(  CSVRow & row,
                                        const vector <string> & sf ) const {
    for( unsigned int i =0; i < sf.size(); i++ ) {
        row[mStartPos + i] = sf[i];
    }
}


//---------------------------------------------------------------------------
// Sort CSV fields in a single CSV record
//---------------------------------------------------------------------------

void RowSortCommand :: SortRow( CSVRow & row ) {
    vector <string> sf = GetSortFields( row );
    if ( mSortLex ) {
        if ( mSortAscending ) {
            std::sort( sf.begin(), sf.end() );
        }
        else {
            SortDesc <string> sd;
            std::sort( sf.begin(), sf.end(), sd );
        }
    }
    else {
        CSVTHROW( "not implemented yet" );
    }
    PutSortFields( row, sf );
}

//---------------------------------------------------------------------------
// Handle all user options with error checking
//---------------------------------------------------------------------------

void RowSortCommand :: ProcessFlags( const ALib::CommandLine & cmd ) {
    NotBoth( cmd, FLAG_ASC, FLAG_DESC );
    NotBoth( cmd, FLAG_LEX, FLAG_NUM );
    mSortAscending = ! cmd.HasFlag( FLAG_DESC );
    mSortLex = ! cmd.HasFlag( FLAG_NUM );
    if ( cmd.HasFlag( FLAG_COLS )) {
        ALib::CommaList cl( cmd.GetValue( FLAG_COLS ));
        CommaListToIndex( cl, mFields );
    }
}

} // end namespace

// end

