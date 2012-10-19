//---------------------------------------------------------------------------
// csved_config.cpp
//
// Configuration (aliases and defaults) for CSVfix
//
// Copyright (C) 2012 Neil Butterworth
//---------------------------------------------------------------------------

#include "a_str.h"
#include "a_env.h"
#include "csved_config.h"
#include "csved_except.h"
#include "csved_strings.h"
#include "csved_version.h"
#include "csved_cli.h"

#include <fstream>

using namespace std;

namespace CSVED {

//----------------------------------------------------------------------------
// Config file name and location is OS dependent
//----------------------------------------------------------------------------

#ifdef _WIN32
#include <direct.h>
const string CONFIG_FILE = "csvfix.ini";
const string HOME_VAR = "USERPROFILE";
#else
#include <direct.h>
const string CONFIG_FILE = ".csvfix";
const string HOME_VAR = "HOME";
#endif

const string DEF_STR = "defaults";
const string ALIAS_STR = "alias";

//----------------------------------------------------------------------------
// Get configuration  from specified  directory
//----------------------------------------------------------------------------

static string GetConfigNameFromDir( const string & dir ) {
	string sep;
	char last  = dir[dir.size() - 1];
	if ( last != '\\' && last != '/' ) {
		sep = "/";
	}
	return dir + sep + CONFIG_FILE;
}


//----------------------------------------------------------------------------
// Get the full path to the user config file.
//----------------------------------------------------------------------------

static string GetUserConfig() {
	string dir = ALib::GetEnv( HOME_VAR );
	if ( dir == "" ) {
		CSVTHROW( "Environment variable " << HOME_VAR << " not set" );
	}
	return GetConfigNameFromDir( dir );
}

//----------------------------------------------------------------------------
// Get the full path to the local (current directory) config file.
//----------------------------------------------------------------------------

static string GetLocalConfig() {
	const int BUFFSIZE = 1024;
	char dir[ BUFFSIZE ];
	getcwd( dir, BUFFSIZE );
	return GetConfigNameFromDir( dir );
}

//----------------------------------------------------------------------------
// See if  a line should be ignored - empty or comment
//----------------------------------------------------------------------------

static bool Ignore( const string & line ) {
	for( unsigned int i = 0; i < line.size(); i++ ) {
		if ( line[i] == '#' ) {
			return true;
		}
		else if ( ! std::isspace( line[i] ) ) {
			return false;
		}
	}
	return true;
}

//----------------------------------------------------------------------------

Config :: Config( CLIHandler * cli ) : mCli( cli ) {
	if ( ! Populate( GetLocalConfig() ) ) {
		Populate( GetUserConfig() );
	}
}

//----------------------------------------------------------------------------

bool  Config :: Populate( const string cfg ) {
	std::ifstream f( cfg.c_str() );
	if ( ! f.is_open() ) {
		return false;
	}
	string line;
	while( getline( f, line ) ) {
		if ( Ignore( line ) ) {
			continue;
		}
		try {
			ProcessSetting( line );
		}
		catch( const Exception & e ) {
			CSVTHROW( "In " << cfg << ": " <<  e.what() );
		}
	}
	return true;
}

void Config :: ProcessSetting( const string line ) {
	std::istringstream is( line );
	string cmd;
	is >> cmd;
	if ( cmd == DEF_STR ) {
		ProcessDefaults( is );
	}
	else if ( cmd == ALIAS_STR ) {
		ProcessAlias( is );
	}
	else {
		CSVTHROW( "Invalid configuration entry: " << line );
	}
}


void Config :: ProcessAlias( std::istringstream & is ) {
	string alias, cmd;
	if ( ! (is >> alias) ) {
		CSVTHROW( "No alias name" );
	}
	if ( mAliases.find( alias ) != mAliases.end() ) {
		CSVTHROW( "Duplicate alias " << alias );
	}
	if ( ! (is >> cmd ) ) {
		CSVTHROW( "No command for alias " << alias );
	}
	if ( ! mCli->HasCommand( cmd ) ) {
		CSVTHROW( "No such command as " << cmd << " for alias " << alias );
	}
	string opt;
	while( is >> opt ) {
		cmd += (" " + opt);
	}
	mAliases.insert( std::make_pair( alias, cmd ) );
}

void Config :: ProcessDefaults( std::istringstream & is ) {
}


string Config :: GetCommand( const string & alias ) const {
	AMapType::const_iterator it = mAliases.find( alias );
	if ( it == mAliases.end() ) {
		return "";
	}
	else {
		return it->second;
	}
}



//---------------------------------------------------------------------------

} // end namespace


// end

