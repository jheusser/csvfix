//---------------------------------------------------------------------------
// csved_call.cpp
//
// call function in dll from csvfix
//
// Copyright (C) 2012 Neil Butterworth
//---------------------------------------------------------------------------

#include "a_base.h"
#include "a_collect.h"
#include "csved_cli.h"
#include "csved_call.h"
#include "csved_evalvars.h"
#include "csved_strings.h"
#include <string>
using std::string;

namespace CSVED {


//---------------------------------------------------------------------------
// Register block command
//---------------------------------------------------------------------------

static RegisterCommand <CallCommand> rc1_(
	CMD_CALL,
	"call function in DLL"
);


//----------------------------------------------------------------------------
// Help
//----------------------------------------------------------------------------

const char * const CALL_HELP = {
	"perform actions on blocks of CSV records\n"
	"usage: csvfix block  [flags] [file ...]\n"
	"where flags are:\n"
	"  -be expr\texpression marking start of block\n"
	"  -ee expr\texpression marking end of block\n"
	"  -r\t\tremove block from output\n"
	"  -k\t\tkeep block in output\n"
	"  -m mark\tmark block and non-block records\n"
	"  -x\t\texclude begin/end records from block\n"
	"#ALL"
};

//----------------------------------------------------------------------------
// The block command
//----------------------------------------------------------------------------

CallCommand :: CallCommand( const string & name, const string & desc )
				: Command( name, desc, CALL_HELP ) {

	AddFlag( ALib::CommandLineFlag( FLAG_DLL, true, 1 ) );
	AddFlag( ALib::CommandLineFlag( FLAG_FUNC, true, 1 ) );
	AddFlag( ALib::CommandLineFlag( FLAG_ARG, false, 1 ) );
	AddFlag( ALib::CommandLineFlag( FLAG_COLS, false, 1 ) );
}


int CallCommand :: Execute( ALib::CommandLine & cmd ) {
	return 0;
}



//----------------------------------------------------------------------------


} // namespace

