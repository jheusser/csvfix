//---------------------------------------------------------------------------
// csved_split.cpp
//
// split single field to multple fields
//
// Copyright (C) 2009 Neil Butterworth
//---------------------------------------------------------------------------

#include "a_base.h"
#include "a_collect.h"
#include "csved_cli.h"
#include "csved_split.h"
#include "csved_strings.h"

using std::string;
using std::vector;

namespace CSVED {

//---------------------------------------------------------------------------
// Register split command
//---------------------------------------------------------------------------


static RegisterCommand <SplitFixed> rc1_(
	CMD_SPLFIX,
	"split input using fixed sizes"
);

static RegisterCommand <SplitChar> rc2_(
	CMD_SPLCHR,
	"split at character"
);

//----------------------------------------------------------------------------
// Help text
//----------------------------------------------------------------------------

const char * const FSPLIT_HELP = {
	"split CSV field at fixed positions\n"
	"usage: csvfix split_fixed  [flags] [file ...]\n"
	"where flags are:\n"
	"  -f field\tindex of the field to be split\n"
	"  -p plist\tlist of positions to split, in start:len format\n"
	"  -k\t\tretain field being split in output (default is discard it)\n"
	"#ALL"
};

const char * const CSPLIT_HELP = {
	"split CSV field at specific character(s)\n"
	"usage: csvfix split_char  [flags] [file ...]\n"
	"where flags are:\n"
	"  -f field\tindex of the field to be split\n"
	"  -c char\tcharacter to split at (default is space)\n"
	"  -k\t\tretain field being split in output (default is discard it)\n"
	"#ALL"
};

//---------------------------------------------------------------------------
// Standard Command ctor
//---------------------------------------------------------------------------

SplitBase :: SplitBase( const string & name,
						const string & desc,
						const string & help )
		: Command( name, desc, help ) {

	AddFlag( ALib::CommandLineFlag( FLAG_COLS, true, 1 ) );
	AddFlag( ALib::CommandLineFlag( FLAG_KEEP, false, 0 ) );
}

//---------------------------------------------------------------------------
// Get flags common to all split commands
//---------------------------------------------------------------------------

void SplitBase :: GetCommonFlags( ALib::CommandLine & cl ) {
	mKeep = cl.HasFlag( FLAG_KEEP );
	string nf = cl.GetValue( FLAG_COLS, "" );
	if ( ! ALib::IsInteger( nf ) ) {
		CSVTHROW( "Field specified by " << FLAG_COLS << " must be integer" );
	}
	mField = ALib::ToInteger( nf ) - 1;
	if ( mField < 0 ) {
		CSVTHROW( "Invalid field index: " << nf );
	}
}

//---------------------------------------------------------------------------
// Helper to insert the split fields into the row at the position
// specified by the -f flag.
//---------------------------------------------------------------------------

void SplitBase :: Insert( CSVRow & row, const CSVRow & split ) {
	CSVRow tmp;
	for ( unsigned int j = 0; j < row.size(); j++ ) {
		if ( j == Field() ) {
			for ( unsigned int k = 0; k < split.size(); k++ ) {
				tmp.push_back( split[k] );
			}
			if ( Keep() ) {
				tmp.push_back( row[j] );
			}
		}
		else {
			tmp.push_back( row[j] );
		}
	}
	row.swap( tmp );
}

//------------------------------------------------------------------------
// Split by fixed positions in field
//---------------------------------------------------------------------------

SplitFixed :: SplitFixed( const string & name,
							const string & desc )
	: SplitBase( name, desc, FSPLIT_HELP ) {
	AddFlag( ALib::CommandLineFlag( FLAG_POS, true, 1 ) );
}

//---------------------------------------------------------------------------
// Get fixed positions then split all inputs
//---------------------------------------------------------------------------

int SplitFixed :: Execute( ALib::CommandLine & cmd ) {

	GetCommonFlags( cmd );
	string pos = cmd.GetValue( FLAG_POS );
	if ( ALib::IsEmpty( pos ) ) {
		CSVTHROW( "Need list of position pairs specified by " << FLAG_POS );
	}

	CreatePositions( pos );

	IOManager io( cmd );
	CSVRow row;

	while( io.ReadCSV( row ) ) {
		Split( row );
		io.WriteRow( row );
	}


	return 0;
}

//---------------------------------------------------------------------------
// Split based on fixed positions. Needs refactoring.
//---------------------------------------------------------------------------

void SplitFixed :: Split( CSVRow & row ) {
	CSVRow tmp;
	string target = Field() < row.size() ? row[ Field() ] : "";
	for ( unsigned int i = 0; i < mPositions.size(); i++ ) {
		unsigned int start = mPositions[i].first;
		unsigned int len = mPositions[i].second;
		if ( start > target.size() ) {
			tmp.push_back( "" );
		}
		else {
			tmp.push_back( target.substr( start, len ) );
		}
	}
	Insert( row, tmp );
}

//---------------------------------------------------------------------------
// Create pairs describing position in a field. Each pair is a start
// position and a length, separated by a colon.
//---------------------------------------------------------------------------

void SplitFixed :: CreatePositions( const string & ps ) {
	ALib::CommaList cl( ps );
	for ( unsigned int i = 0; i < cl.Size(); i++ ) {
		vector <string> tmp;
		if ( ALib::Split( cl.At(i), ':', tmp ) != 2 ) {
			CSVTHROW( "Invalid position " << cl.At(i) );
		}
		AddPosition( tmp[0], tmp[1] );
	}
}

//---------------------------------------------------------------------------
// Validate position and add to list
//---------------------------------------------------------------------------

void SplitFixed :: AddPosition( const string & spos,
									const string & slen ) {
	if ( ! ALib::IsInteger( spos ) || ! ALib::IsInteger( slen ) ) {
		CSVTHROW( "Invalid position " << spos << ":" << slen );
	}

	int pos = ALib::ToInteger( spos );
	int len = ALib::ToInteger( slen);

	if ( pos <= 0 || len <= 0 ) {
		CSVTHROW( "Invalid position " << spos << ":" << slen );
	}

	mPositions.push_back(
		std::make_pair( (unsigned int) pos - 1, (unsigned int) len )
	);
}

//---------------------------------------------------------------------------
// Split field at character value. Default is to split on spaces
//---------------------------------------------------------------------------

SplitChar :: SplitChar( const string & name, const string & desc )
	: SplitBase( name, desc, CSPLIT_HELP ) {
	AddFlag( ALib::CommandLineFlag( FLAG_CHAR, false, 1 ) );
}

//---------------------------------------------------------------------------
// Get the character to split on, then split the field specified by the -f
// flag for all inputs.
//---------------------------------------------------------------------------

int SplitChar :: Execute( ALib::CommandLine & cmd ) {

	GetCommonFlags( cmd );
	string sc = cmd.GetValue( FLAG_CHAR, " " );
	if ( sc == "" ) {
		CSVTHROW( "Need characters specified by " << FLAG_CHAR );
	}
	else {
		mChars = ALib::UnEscape( sc );
	}

	IOManager io( cmd );
	CSVRow row;

	while( io.ReadCSV( row ) ) {
		Split( row );
		io.WriteRow( row );
	}

	return 0;
}

//---------------------------------------------------------------------------
// Perform split on characters (pre CSVfix 0.9 only single char was allowed)
// specified with -c flag.
//
// Note we call different overloads of ALib::Split depending on whether
// we have a single character or a string to avoid backward compatibility
// problems.
//---------------------------------------------------------------------------

void SplitChar :: Split( CSVRow & row ) {
	string target = Field() < row.size() ? row[ Field() ] : "";
	CSVRow tmp;
	if ( mChars.size() == 1 ) {
		ALib::Split( target, mChars[0], tmp );
	}
	else {
		ALib::Split( target, mChars, tmp );
	}
	Insert( row, tmp );
}

} // end namespace

// end

