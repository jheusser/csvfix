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



//------------------------------------------------------------------------
// Standard command ctor
//---------------------------------------------------------------------------

CheckCommand :: CheckCommand( const string & name,
								const string & desc )
		: Command( name, desc, CHECK_HELP ) {

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

	}
	return errors == 0 ? 0 : 1;
}



//---------------------------------------------------------------------------
// Handle all user options with error checking
//---------------------------------------------------------------------------

void CheckCommand :: ProcessFlags( const ALib::CommandLine & cmd ) {
}


} // end namespace

// end

