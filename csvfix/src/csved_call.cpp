//---------------------------------------------------------------------------
// csved_call.cpp
//
// call function in dll from csvfix
// this is windows specific code
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

#include <windows.h>

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
// Fixed sized output buffer for DLL functions
//----------------------------------------------------------------------------

char CallCommand :: mOutBuf[ CallCommand::OUTBUF_SIZE ];

//----------------------------------------------------------------------------
// Help
//----------------------------------------------------------------------------

const char * const CALL_HELP = {
	"call function in DLL\n"
	"usage: csvfix call  [flags] [file ...]\n"
	"where flags are:\n"
	"  -fnc name\tname of function to call\n"
	"  -dll name\tfilename of DLL containing function\n"
	"  -arg mode\thow to pass data - allowed values are 'field' and 'row' \n"
	"  -f fields\tfields to pass to mthe function\n"
	"  -r \t\treplace fields/rows - default is to append to existing data\n"

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
	AddFlag( ALib::CommandLineFlag( FLAG_REPLACE, false, 0 ) );

}



//----------------------------------------------------------------------------


int CallCommand :: Execute( ALib::CommandLine & cmd ) {

	ProcessFlags( cmd );

	HMODULE dll = LoadLibrary( mDLL.c_str() );
	if ( dll == NULL ) {
		CSVTHROW( "LoadLibrary call on " << mDLL << " failed" );
	}
	mFunc = (FuncType) GetProcAddress( dll, mFuncName.c_str() );
	if ( mFunc == NULL ) {
		CSVTHROW( "Cannot load function " << mFuncName << "from DLL " << mDLL );
	}

	IOManager io( cmd );
	CSVRow row;
	int rv;
	while( io.ReadCSV( row ) ) {
		if ( mArgType == ArgType::Field ) {
			rv = CallOnFields( row );
		}
		else {
			rv = CallOnRow( row );
		}
		io.WriteRow( row );
	}


	return 0;
}

int CallCommand :: CallOnFields( CSVRow & row ) {
	string fields;
	for ( unsigned int i = 0; i < row.size(); i++ ) {
		if ( mFields.size() == 0 || ALib::Contains( mFields, i ) ) {
			fields += row[i];
			fields += '\0';
		}
	}
	int rv = mFunc( (char*)fields.c_str(), mOutBuf, OUTBUF_SIZE );
	return rv;
}

int CallCommand :: CallOnRow( CSVRow & row ) {
	return 0;
}

//----------------------------------------------------------------------------
// Handle command line options
//----------------------------------------------------------------------------

const char * const FIELD_MODE = "field";
const char * const ROW_MODE = "row";

void CallCommand :: ProcessFlags( const ALib::CommandLine & cmd ) {
	mDLL = cmd.GetValue( FLAG_DLL );
	mFuncName = cmd.GetValue( FLAG_FUNC );
	string mode = cmd.GetValue( FLAG_ARG, FIELD_MODE );
	if ( mode == FIELD_MODE ) {
		mArgType = ArgType::Field;
	}
	else if ( mode == ROW_MODE ) {
		mArgType = ArgType::Row;
	}
	else {
		CSVTHROW( "Invalid argument mode: " << mode );
	}
	ALib::CommaList cl( cmd.GetValue( FLAG_COLS ) );
	CommaListToIndex( cl, mFields );
	mReplace = cmd.HasFlag( FLAG_REPLACE );
}


//----------------------------------------------------------------------------


} // namespace

