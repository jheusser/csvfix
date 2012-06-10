//---------------------------------------------------------------------------
// csved_cli.h
//
// Command handler for CSVED
//
// Copyright (C) 2009 Neil Butterworth
//---------------------------------------------------------------------------

#ifndef INC_CSVED_CLI_H
#define INC_CSVED_CLI_H

#include "a_base.h"
#include "a_dict.h"
#include "a_env.h"
#include "csved_command.h"

namespace CSVED {

//---------------------------------------------------------------------------
// Manages command dictionary and translates user input to command execution
//---------------------------------------------------------------------------


class CLIHandler {

	public:

		typedef ALib::Dictionary <Command *> DictType;

		CLIHandler( int argc, char * argv[] );

		int ExecCommand();

		static void AddCommand( const std::string & name,
								Command * cmd );
	private:

		Command * FindCommand();
		Command * FindAbbrev( const std::string & ab );

		int Help();
		int HelpCmd();
		int Info();

		static void InitDict();
		static DictType * mDict;

		ALib::CommandLine mCmdLine;

};

//---------------------------------------------------------------------------
// Template class used to register commands with command dictionary
//----------------------------------------------------------------------------

template <typename T> struct RegisterCommand {

	RegisterCommand( const std::string & name,
					const std::string & desc ) {

		Command * cp =  new T( name, desc );
		CLIHandler::AddCommand( name, cp );
	}

	RegisterCommand( const std::string & name,
					const std::string & desc,
					const std::string & help ) {

		Command * cp =  new T( name, desc );
		cp->AddHelp( help );
		CLIHandler::AddCommand( name, cp );
	}
};

//---------------------------------------------------------------------------

}	// namespace

#endif
