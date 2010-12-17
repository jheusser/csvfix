//---------------------------------------------------------------------------
// csved_util.cpp
//
// utilities for CSVED
//
// Copyright (C) 2009 Neil Butterworth
//---------------------------------------------------------------------------

#include "a_base.h"
#include "a_env.h"
#include "a_str.h"
#include "csved_util.h"
#include "csved_except.h"

using std::string;
using std::vector;

namespace CSVED {

//---------------------------------------------------------------------------
// Convert comma list to vecor of ints to be used as col index. We now allow
// ranges in the form n1:n2.
//---------------------------------------------------------------------------

static void MakeAscending( int n1, int n2, FieldList & fl ) {
	while( n1 <= n2 ) {
		fl.push_back( n1 - 1 );
		n1++;
	}
}

static void MakeDescending( int n1, int n2, FieldList & fl ) {
	while( n1 >= n2 ) {
		fl.push_back( n1 - 1 );
		n1--;
	}
}

void CommaListToIndex( const ALib::CommaList & cl,
						FieldList & idx ) {
	idx.clear();
	for ( unsigned int i = 0; i < cl.Size(); i++ ) {
		string cle = cl.At(i);
		vector <string> fl;
		ALib::Split( cle, ':', fl );
		if ( fl.size() > 2 ) {
			CSVTHROW( "Invalid field: " << cle );
		}
		else if ( fl.size() == 2 ) {
			if ( ! (ALib::IsInteger(fl[0]) && ALib::IsInteger(fl[1])) ) {
				CSVTHROW( "Invalid range: " << cle );
			}
			int n1 = ALib::ToInteger( fl[0] );
			int n2 = ALib::ToInteger( fl[1] );
			if ( n1 < 1 || n2 < 1 ) {
				CSVTHROW( "Invalid range: " << cle );
			}
			if ( n1 < n2 ) {
				MakeAscending( n1, n2, idx );
			}
			else {
				MakeDescending( n1, n2, idx );
			}
		}
		else {
			if ( ! ALib::IsInteger( cle ) ) {
				CSVTHROW( "Need integer, not '" << cle << "'" );
			}
			int n = ALib::ToInteger( cle );
			if ( n < 1 ) {
				CSVTHROW( "Index must be greater than zero, not '" << cle << "'" );
			}
			idx.push_back( n - 1 );		// convert to zero-based
		}
	}
}


//---------------------------------------------------------------------------

}	// end namespace

// end


