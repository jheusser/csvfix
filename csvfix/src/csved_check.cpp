//---------------------------------------------------------------------------
// csved_check.cpp
//
// Check CSV records actually are CSV
//
// Copyright (C) 2009 Neil Butterworth
//---------------------------------------------------------------------------

#include "a_base.h"
#include "csved_cli.h"
#include "csved_check.h"
#include "csved_strings.h"

using std::string;
using std::vector;

namespace CSVED {

//---------------------------------------------------------------------------
// Register check command
//---------------------------------------------------------------------------

static RegisterCommand <CheckCommand> rc1_(
	CMD_CHECK,
	"check CSV record format conforms to CSV standard"
);

//----------------------------------------------------------------------------
// Help text
//----------------------------------------------------------------------------

const char * const CHECK_HELP = {
	"check that records in file conform to CSV standards\n"
	"usage: csvfix check [flags] [files ...]\n"
	"where flags are:\n"
	"  -rfc\t\tcheck against RFC standard\n"
	"  -x\t\tfilter out bad records\n"
	"  -ni\t\tdon't print file/line info\n"
	"  -o ofn\twrite output to ofn instead of standard output\n"
};

//----------------------------------------------------------------------------
// Special values for states
//----------------------------------------------------------------------------

const int ANY_CHAR = 1;
const int END_CHAR= 2;
const int END_STATE= 3;
const int NO_STATE = 0;

//----------------------------------------------------------------------------
// A state has start, character that can cause a stransition, and the
// state to end in after transition.
//----------------------------------------------------------------------------

struct State {
	int mStart;
	char mTrans;
	int mEnd;
};

//----------------------------------------------------------------------------
// State transition table for CSV as understood by CSVfix
//----------------------------------------------------------------------------

State CSVFixTrans[] = {
	{ 1, 	END_CHAR, 	END_STATE },
	{ 1, 	'"', 		2 },
	{ 1, 	ANY_CHAR, 	1 },
	{ 2, 	'"', 		3 },
	{ 2, 	ANY_CHAR, 	2 },
	{ 3, 	'"', 		2 },
	{ 3, 	END_CHAR, 	END_STATE },
	{ 3, 	ANY_CHAR, 	1 },
	{ NO_STATE, 0, 0}
};

//----------------------------------------------------------------------------
// Starting in state 'start', try to transition to new state via transition
// character 'sc' using transition table 'states'.
//----------------------------------------------------------------------------

int NextState( int start, char c, const State * states ) {
	for ( int i = 0; states[i].mStart != NO_STATE; i++ ) {
		if ( states[i].mStart == start ) {
			if ( states[i].mTrans == c
					|| (states[i].mTrans == ANY_CHAR && c != END_CHAR) ) {
				return states[i].mEnd;
			}
		}
	}
	return NO_STATE;
}

//----------------------------------------------------------------------------
// Check a record using the state transitio table 'states'.
//----------------------------------------------------------------------------

bool CheckRecord( const string & line, const State * states )  {
	unsigned int pos = 0;
	int state = 1;
	while( pos <= line.size() ) {
		char c = (pos == line.size()) ? END_CHAR : line[pos];
		state = NextState( state, c, states );
		if ( state == NO_STATE ) {
			return false;
		}
		else if ( state == END_STATE ) {
			return true;
		}
		pos++;
	}
	return false;
}

//------------------------------------------------------------------------
// Standard command ctor
//---------------------------------------------------------------------------

CheckCommand :: CheckCommand( const string & name,
								const string & desc )
		: Command( name, desc, CHECK_HELP ),
			mPrintFileInfo( true ), mFilterBad( false ), mRFC( false )  {

	AddFlag( ALib::CommandLineFlag( FLAG_FILTBAD, false, 0 ));
	AddFlag( ALib::CommandLineFlag( FLAG_NOINFO, false, 0 ));
	AddFlag( ALib::CommandLineFlag( FLAG_RFC, false, 0 ));

}

//---------------------------------------------------------------------------
// Get user specified options and then read input.
//---------------------------------------------------------------------------

int CheckCommand :: Execute( ALib::CommandLine & cmd ) {

	ProcessFlags( cmd );

	IOManager io( cmd );
	string line;
	int errors = 0;

	while( io.ReadLine( line ) ) {
		if ( CheckRecord( line, CSVFixTrans ) ) {
			if ( mFilterBad ) {
				io.Out()  << line << "\n";
			}
		}
		else {
			errors++;
			if ( ! mFilterBad ) {
				if ( mPrintFileInfo ) {
					io.Out()  << io.CurrentFileName() << ":"
								<< io.CurrentLine()  << " ";
				}
				io.Out() << line << "\n";
			}
		}
	}
	return errors == 0 ? 0 : 1;
}



//---------------------------------------------------------------------------
// Handle all user options with error checking
//---------------------------------------------------------------------------

void CheckCommand :: ProcessFlags( const ALib::CommandLine & cmd ) {
	mRFC = cmd.HasFlag( FLAG_RFC );
	mPrintFileInfo = ! cmd.HasFlag( FLAG_NOINFO );
	mFilterBad = cmd.HasFlag( FLAG_FILTBAD );
}


} // end namespace

// end

