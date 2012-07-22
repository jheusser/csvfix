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

//----------------------------------------------------------------------------

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
	"  -f fields\tfields to pass to the function\n"
	"#ALL"
};

//----------------------------------------------------------------------------
// The block command
//----------------------------------------------------------------------------

CallCommand :: CallCommand( const string & name, const string & desc )
				: Command( name, desc, CALL_HELP ) {

	AddFlag( ALib::CommandLineFlag( FLAG_DLL, true, 1 ) );
	AddFlag( ALib::CommandLineFlag( FLAG_FUNC, true, 1 ) );
	AddFlag( ALib::CommandLineFlag( FLAG_COLS, false, 1 ) );
}

//----------------------------------------------------------------------------
// Load function from DLL and then call it on fields. A non-zero return value
// from function of means either skip this row on output ( for
// negative return values), or report an error (positive values).
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
		int rv = CallOnFields( row );
		if ( rv == 0 ) {
			io.WriteRow( row );
		}
		else if ( rv > 0 ) {
			CSVTHROW( mFuncName << " returned error code " << rv );
		}
		else {
			// do nothing - negative values just mean skip output
		}
	}
	return 0;
}

//----------------------------------------------------------------------------
// Helper to extract single field  from output data
//----------------------------------------------------------------------------

static string ExtractField( const char * s, int & pos ) {
	string field;
	while( s[pos] ) {
		field += s[pos++];
	}
	return field;
}

//----------------------------------------------------------------------------
// Call DLL function on specified fields. If none are specified, call it on
// all fields. Fields returned from DLL are appended to the existing data on
// output, provided the function returned zero.
//----------------------------------------------------------------------------

int CallCommand :: CallOnFields( CSVRow & row ) {
	string fields;
	int fc = 0;
	for ( unsigned int i = 0; i < row.size(); i++ ) {
		if ( mFields.size() == 0 || ALib::Contains( mFields, i ) ) {
			fields += row[i];
			fields += '\0';
			fc++;
		}
	}
	int ofc = 0;
	int rv = mFunc( fc, fields.c_str(), &ofc, mOutBuf, OUTBUF_SIZE );
	if ( rv == 0  ) {
		int pos = 0;
		while( ofc-- ) {
			string field = ExtractField( mOutBuf, pos );
			pos++;
			row.push_back( field );
		}
	}
	return rv;
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
	ALib::CommaList cl( cmd.GetValue( FLAG_COLS ) );
	CommaListToIndex( cl, mFields );
}


//----------------------------------------------------------------------------


} // namespace

