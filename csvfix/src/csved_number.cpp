//---------------------------------------------------------------------------
// csved_number.cpp
//
// convert fields possibly containing strange thousands/decimal point
// separators to numeric values
//
// Copyright (C) 2013 Neil Butterworth
//---------------------------------------------------------------------------

#include "a_base.h"
#include "a_collect.h"
#include "csved_cli.h"
#include "csved_number.h"
#include "csved_strings.h"

#include <string>
using std::string;

namespace CSVED {


//---------------------------------------------------------------------------
// Register number command
//---------------------------------------------------------------------------

static RegisterCommand <NumberCommand> rc1_(
	CMD_NUMBER,
	"convert formatted numeric fields to ordinary numeric"
);


//----------------------------------------------------------------------------
// Help
//----------------------------------------------------------------------------

const char * const NUMBER_HELP = {
	"convert formatted numeric fields to ordinary numeric\n"
	"usage: csvfix number  [flags] [file ...]\n"
	"where flags are:\n"
	"  -f fields\tfields to convert - default is all\n"
	"  -fmt fmt\tformat of input fields - one of EU or EN (default)\n"
	"  -es str\treplace fields that cannot be read with this string\n"
	"  -ec \t\tconversion failure is an error\n"
	"#ALL"
};

//----------------------------------------------------------------------------
// The number command
//----------------------------------------------------------------------------

NumberCommand :: NumberCommand( const string & name, const string & desc )
				: Command( name, desc, NUMBER_HELP )  {

	AddFlag( ALib::CommandLineFlag( FLAG_COLS, false, 1 ) );
	AddFlag( ALib::CommandLineFlag( FLAG_FMT, false, 1 ) );
	AddFlag( ALib::CommandLineFlag( FLAG_ERRSTR, false, 1 ) );
	AddFlag( ALib::CommandLineFlag( FLAG_ERRCODE, false, 0 ) );
}


//----------------------------------------------------------------------------
//----------------------------------------------------------------------------

int NumberCommand :: Execute( ALib::CommandLine & cmd ) {

	ProcessFlags( cmd );
	IOManager io( cmd );
	CSVRow row;
	return 0;
}

//----------------------------------------------------------------------------
// Get command line options and report any problems  with them.
//----------------------------------------------------------------------------

void NumberCommand :: ProcessFlags( const ALib::CommandLine & cmd ) {
}


//----------------------------------------------------------------------------

} // namespace





