//---------------------------------------------------------------------------
// csved_flatten.cpp
//
// flatten and unflatten commands
//
// Copyright (C) 2009 Neil Butterworth
//---------------------------------------------------------------------------

#include "a_base.h"
#include "a_collect.h"

#include "csved_cli.h"
#include "csved_flatten.h"
#include "csved_strings.h"

using std::string;
using std::vector;

namespace CSVED {


//---------------------------------------------------------------------------
// Register commands
//---------------------------------------------------------------------------

static RegisterCommand <FlattenCommand> rc1_(
	CMD_FLATTEN,
	"flatten to single row"
);

static RegisterCommand <UnflattenCommand> rc2_(
	CMD_UNFLATTEN,
	"convert single row to multiple rows"
);

//----------------------------------------------------------------------------
// Help text
//----------------------------------------------------------------------------

const char * const FLAT_HELP = {
	"flattens multiple rows into a single row\n"
	"usage: csvfix flatten [flags] [files ...]\n"
	"where flags are:\n"
	"  -k key\tspecify list of key fields (default is first field)\n"
	"  -r \t\tremove key in output (default is retain)\n"
	"  -f data\tspecify list of data fields (default is all but first)\n"
	"#SMQ,SEP,IBL,IFN,OFL"
};

const char * const UNFLAT_HELP = {
	"converts repeated single row values to multiple rows\n"
	"usage: csvfix unflatten [flags] [files ...]\n"
	"where flags are:\n"
	"  -k key\tspecify list of key fields (default is first field)\n"
	"  -n data\tnumberr of data fields perr output row (default is 1)\n"
	"#SMQ,SEP,IBL,IFN,OFL"
};

//------------------------------------------------------------------------
// Flatten
//---------------------------------------------------------------------------

FlattenCommand :: FlattenCommand( const string & name,
									const string & desc )
		: Command( name, desc, FLAT_HELP ) {

	AddFlag( ALib::CommandLineFlag( FLAG_COLS, false, 1 ) );
	AddFlag( ALib::CommandLineFlag( FLAG_KEY, false, 1 ) );
	AddFlag( ALib::CommandLineFlag( FLAG_REMOVE, false, 0 ) );

}

//---------------------------------------------------------------------------
// Flatten rows, ouputing new row when key changes.
//---------------------------------------------------------------------------

int FlattenCommand :: Execute( ALib::CommandLine & cmd ) {

	ProcessFlags( cmd );

	IOManager io( cmd );
	CSVRow row;

	mKey = "";
	mData.clear();
	int read = 0;

	while( io.ReadCSV( row ) ) {
		string key = MakeKey( row );
		if ( read++ == 0 ) {
			NewKey( row );
		}
		else if ( key != mKey ) {
			io.WriteRow( mData );
			NewKey( row );
		}
		AddData( row );
		mKey = key;
	}
	io.WriteRow( mData );

	return 0;
}

//----------------------------------------------------------------------------
// Create key by concating values separated by null character.
//----------------------------------------------------------------------------

string FlattenCommand :: MakeKey( const CSVRow & row ) const {
	string key = "";
	for ( unsigned int i = 0; i < mKeyFields.size(); i++ ) {
		if ( mKeyFields[i] < row.size() ) {
			key += row[ mKeyFields[i] ];
		}
		key += '\0';
	}
	return key;
}

//----------------------------------------------------------------------------
// When key changes, remove old data and start with new key
//----------------------------------------------------------------------------

void FlattenCommand :: NewKey( const CSVRow & row ) {
	mData.clear();
	if ( mKeepKey ) {
		for ( unsigned int i = 0; i < mKeyFields.size(); i++ ) {
			if ( mKeyFields[i] < row.size() ) {
				mData.push_back( row[ mKeyFields[i] ] );
			}
			else {
				mData.push_back( "" );
			}
		}
	}
}

//----------------------------------------------------------------------------
// Add data to end of row to be output
//----------------------------------------------------------------------------

void FlattenCommand :: AddData( const CSVRow & row ) {
	if ( mDataFields.size() == 0 ) {
		for ( unsigned int i = 0; i < row.size(); i++ ) {
			if ( ! ALib::Contains( mKeyFields, i  )) {
				mData.push_back( row[i] );
			}
		}
	}
	else {
		for ( unsigned int i = 0; i < mDataFields.size(); i++ ) {
			if ( mDataFields[i] < row.size() ) {
				mData.push_back( row[ mDataFields[i] ] );
			}
			else {
				mData.push_back( "" );
			}
		}
	}
}

//---------------------------------------------------------------------------
// Get flatten user options
//---------------------------------------------------------------------------

void FlattenCommand :: ProcessFlags( const ALib::CommandLine & cmd ) {

	ALib::CommaList dl( cmd.GetValue( FLAG_COLS, "" ));
	CommaListToIndex( dl, mDataFields );
	ALib::CommaList kl( cmd.GetValue( FLAG_KEY, "1" ));
	CommaListToIndex( kl, mKeyFields );
	mKeepKey = ! cmd.HasFlag( FLAG_REMOVE );
}

//------------------------------------------------------------------------
// Unflatten
//---------------------------------------------------------------------------

UnflattenCommand :: UnflattenCommand( const string & name,
									const string & desc )
		: Command( name, desc, UNFLAT_HELP ) {

	AddFlag( ALib::CommandLineFlag( FLAG_KEY, false, 1 ) );
	AddFlag( ALib::CommandLineFlag( FLAG_NUM, false, 1 ) );

}

//---------------------------------------------------------------------------
// Unflatten single row to multiple rows
//---------------------------------------------------------------------------

int UnflattenCommand :: Execute( ALib::CommandLine & cmd ) {

	ProcessFlags( cmd );

	IOManager io( cmd );
	CSVRow row;

	while( io.ReadCSV( row ) ) {
		CSVRow key = GetKey( row );
		WriteRows( io, key, row );
	}
	return 0;
}

//----------------------------------------------------------------------------
// Build key field list which is output first
//----------------------------------------------------------------------------

CSVRow UnflattenCommand :: GetKey( const CSVRow & row ) const {
	CSVRow key;
	for ( unsigned int i = 0; i < mKeyFields.size(); i++ ) {
		if ( mKeyFields[i] < row.size() ) {
			key.push_back( row[ mKeyFields[i] ] );
		}
	}
	return key;
}

//----------------------------------------------------------------------------
// Write output rows built from key and some data rows.
//----------------------------------------------------------------------------

void UnflattenCommand :: WriteRows( IOManager & io, const CSVRow & key,
										const CSVRow & row ) {

	unsigned int i = 0;
	while( i < row.size() ) {
		CSVRow out( key );
		unsigned int n = mNumDataFields;
		bool added = false;
		while( n && i < row.size() ) {
			if ( ! ALib::Contains( mKeyFields, i )) {
				out.push_back( row[i] );
				added = true;
				n--;
			}
			i++;
		}
		if ( added ) {
			io.WriteRow( out );
		}
	}
}


//----------------------------------------------------------------------------
// Unflatten options
//----------------------------------------------------------------------------

void UnflattenCommand :: ProcessFlags( const ALib::CommandLine & cmd ) {
	ALib::CommaList kl( cmd.GetValue( FLAG_KEY, "1" ));
	CommaListToIndex( kl, mKeyFields );
	string ns =  cmd.GetValue( FLAG_NUM, "1" );
	if ( ! ALib::IsInteger( ns ) ) {
		CSVTHROW( "Number of data per output must be integer" );
	}
	int n = ALib::ToInteger( ns );
	if ( n <= 0 ) {
		CSVTHROW( "Number of data per output must be greater than zero" );
	}
	mNumDataFields = n;
}


//----------------------------------------------------------------------------

} // end namespace

// end

