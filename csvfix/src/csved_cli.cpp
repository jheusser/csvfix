//---------------------------------------------------------------------------
// csved_cli.cpp
//
// Command line handler for csvfix
//
// Copyright (C) 2009 Neil Butterworth
//---------------------------------------------------------------------------

#include "a_str.h"
#include "csved_cli.h"
#include "csved_except.h"
#include "csved_strings.h"
#include "csved_version.h"

using std::string;
using std::vector;
using std::cerr;

namespace CSVED {

//---------------------------------------------------------------------------
// Single instance of map to translate command names to command objects.
//---------------------------------------------------------------------------

CLIHandler::DictType * CLIHandler::mDict = 0;

//---------------------------------------------------------------------------
// Create command line handler from command line params of main()
//---------------------------------------------------------------------------

CLIHandler :: CLIHandler( int argc, char * argv[] )
	: mCmdLine( argc, argv) {
	InitDict();
}

//---------------------------------------------------------------------------
// Run command based on command line parameters
// The arg value indexed by 1 is the internal command.
//---------------------------------------------------------------------------

int CLIHandler :: ExecCommand() {
	if ( mCmdLine.Argc() == 1 ) {
		return Info();
	}
	else if ( mCmdLine.Argc() == 2 &&  (
				mCmdLine.Argv( 1 ) == CMD_USAGE ||
				mCmdLine.Argv( 1 ) == CMD_HELP ) ) {
		return Help();
	}
	else if ( mCmdLine.Argc() >= 2 && mCmdLine.Argv( 1 ) == CMD_HELP ) {
		return HelpCmd();
	}
	else {
		Command * cmd = FindCommand();
		cmd->CheckFlags( mCmdLine );
		return cmd->Execute( mCmdLine );
	}
}

//---------------------------------------------------------------------------
// Find named command or report error
//---------------------------------------------------------------------------

Command * CLIHandler :: FindCommand() {
	if ( mCmdLine.Argc() == 1 ) {
		CSVTHROW( "No command specified - try 'csvfix usage'" );
	}
	else if ( mDict->Contains( mCmdLine.Argv( 1 ) )) {
		return mDict->Get( mCmdLine.Argv( 1 ) );
	}
	else {
		Command * c = FindAbbrev( mCmdLine.Argv( 1 ) );
		if (  c == 0 ) {
			CSVTHROW( "Unknown command - try 'csvfix help'" );
		}
		return c;
	}
}

//----------------------------------------------------------------------------
// Find possibly abbreviated command
//----------------------------------------------------------------------------

Command * CLIHandler :: FindAbbrev( const  string & ab ) {
	vector <string> mPossibles;
	mDict->GetAbbreviations( ab, mPossibles );
	if ( mPossibles.size() == 0 ) {
		return nullptr;
	}
	else if ( mPossibles.size() > 1 ) {
		string msg = "Ambiguous command - candidates are: ";
		for( unsigned int i = 0; i < mPossibles.size(); i++ ) {
			msg += mPossibles[i] + " ";
		}
		CSVTHROW( msg );
	}
	else {
		return mDict->Get( mPossibles[0] );
	}

}

//----------------------------------------------------------------------------
// Display message in response to 'csvfix' (no command)
//----------------------------------------------------------------------------

int CLIHandler :: Info() {
	cerr << "\n" << CSVED_NAME << " " << CSVED_VERS << " (" << CSVED_VDATE << ")\n";
	cerr << CSVED_CPYR << "\n" << CSVED_NOWAR;
	cerr << "\n\n";
	cerr << "csvfix is a CSV stream editor\n";
	cerr << "use 'csvfix help' to see a list of commands\n";
	cerr << "use 'csvfix help command' to see help on a specific command\n";
	return 0;
}

//---------------------------------------------------------------------------
// Display message in response to 'csvfix help'
//---------------------------------------------------------------------------

int CLIHandler :: HelpCmd() {
	Command * cp = FindAbbrev( mCmdLine.Argv(2) );
	if ( cp == nullptr ) {
		cerr << "no such command: " << mCmdLine.Argv(2) << "\n";
		cerr << "use 'csvfix help' to see a list of commands\n";
		return 1;
	}
	if ( cp->Help() == "" ) {
		cerr << "sorry, no help available on this command\n";
		cerr << "the command may be slated for removal from CSVfix\n";
		return 1;
	}
	else {
		cerr << cp->Help() << "\n";
	}
	return 0;
}

//---------------------------------------------------------------------------
// Display message in response to 'csvfix help'
//---------------------------------------------------------------------------

int CLIHandler :: Help() {
	cerr << "\nusage: csvfix command [flags] file ...\n"
				<< "where 'command' is one of: \n\n";

	vector <string> cmds;
	mDict->GetNames( cmds );
	string spaces( 30, ' ' );

	unsigned int width = 0;
	for ( unsigned int j = 0; j < cmds.size(); j++ ) {
		width = ALib::Max( width, (unsigned int ) cmds[j].size() );
	}

	for ( unsigned int i = 0; i < cmds.size(); i++ ) {
		Command * cp = mDict->Get( cmds[i] );
		cerr << "    " << cmds[i]
				<<  spaces.substr( 0, (width + 2) - cmds[i].size() )
				<< cp->Desc() << std::endl;
	}

	return 0;
}

//---------------------------------------------------------------------------
// Static to initialise the name -> command dictionary
//---------------------------------------------------------------------------

void CLIHandler :: InitDict() {
	static bool needinit = true;
	if ( needinit ) {
		mDict = new DictType;
		needinit = false;
	}
}

//---------------------------------------------------------------------------
// Add a named command to the dictionary.
//---------------------------------------------------------------------------

void CLIHandler :: AddCommand( const string & name, Command * cmd ) {
	InitDict();
	if ( mDict->Contains( name ) ) {
		CSVTHROW( "Duplicate command name " << ALib::SQuote( name ) );
	}
	mDict->Add( name, cmd );
}

//---------------------------------------------------------------------------

} // end namespace


// end

