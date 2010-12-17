//---------------------------------------------------------------------------
// csved_odbc.cpp
//
// exec SQL commands agains database producing CSV output
// this is Windows only, at present
//
// Copyright (C) 2009 Neil Butterworth
//---------------------------------------------------------------------------

#include <assert.h>
#include <fstream>
#include "a_base.h"
#include "a_file.h"
#include "csved_cli.h"
#include "csved_odbc.h"
#include "csved_strings.h"

using std::string;
using std::vector;

namespace CSVED {

//----------------------------------------------------------------------------
// Register command
//----------------------------------------------------------------------------

static RegisterCommand <ODBCGetCommand> rc1_(
	CMD_ODBCGET,
	"get data from ODBC database"
);

//----------------------------------------------------------------------------
// Help text
//----------------------------------------------------------------------------

const char * const OGET_HELP = {
	"extract data from ODBC-connected database and format as CSV\n"
	"note that this command does not read input streams\n"
	"usage: csvfix odbc_get  [flags] [file ...]\n"
	"where flags are:\n"
	"  -cs cstr\tODBC connection string\n"
	"  -sql stmt\tSQL statement to execute to extract data\n"
	"  -tbl table\textract all data from table  rather than usie SQL\n"
	"  -ns null\tstring to use to represent nulls (default is empty string)\n"
	"#SMQ,OFL"
};

//----------------------------------------------------------------------------
// base command ctor
//----------------------------------------------------------------------------

ODBCCommand :: ODBCCommand( const string & name,
								const string & desc )
		: Command( name, desc, OGET_HELP ), mStatement( 0 ) {
}

//----------------------------------------------------------------------------
// Junk statement
//----------------------------------------------------------------------------

ODBCCommand :: ~ODBCCommand() {
	delete mStatement;
}

//----------------------------------------------------------------------------
// connect to database using connection string
//----------------------------------------------------------------------------

void ODBCCommand :: Connect() {
	delete mStatement;
	mStatement = 0;
	if ( ! mConnection.Connect( mConnStr ) ) {
		CSVTHROW( "Database connection failed: " << mConnection.Error() );
	}
	mStatement = new ALib::DbStatement( mConnection );
}

//----------------------------------------------------------------------------
// execute any SQL command
//----------------------------------------------------------------------------

void ODBCCommand :: Exec( const string & sql ) {
	assert( mStatement );
	if ( ! mStatement->Exec( sql ) ) {
		CSVTHROW( "SQL error: " << mStatement->Error() );
	}
}

//----------------------------------------------------------------------------
// get statement
//----------------------------------------------------------------------------

ALib::DbStatement * ODBCCommand :: Stmt() {
	return mStatement;
}

//----------------------------------------------------------------------------
// handle connection string params
//----------------------------------------------------------------------------

void ODBCCommand :: ProcessFlags( const ALib::CommandLine & cmd ) {
	mConnStr = cmd.GetValue( FLAG_CONSTR, "" );
	if ( mConnStr == "" ) {
		CSVTHROW( "Connection string cannot be empty" );
	}
}

//----------------------------------------------------------------------------
// get command reads data from database
//----------------------------------------------------------------------------

ODBCGetCommand :: ODBCGetCommand( const string & name,
									const string & desc )
	: ODBCCommand( name, desc ) {

	AddFlag( ALib::CommandLineFlag( FLAG_SQLQ, false, 1 ) );
	AddFlag( ALib::CommandLineFlag( FLAG_SQLTBL, false, 1 ) );
	AddFlag( ALib::CommandLineFlag( FLAG_CONSTR, true, 1 ) );
	AddFlag( ALib::CommandLineFlag( FLAG_NULLSTR, false, 1 ) );
}

//----------------------------------------------------------------------------
// use base to connect and then read all rows converting to csv
//----------------------------------------------------------------------------

int ODBCGetCommand :: Execute( ALib::CommandLine & cmd ) {
	ProcessFlags( cmd );

	ODBCCommand::Connect();;
	ODBCCommand::Exec( mSql );

	ALib::DbRow row;
	IOManager io( cmd );
	Stmt()->SetNull( mNull );
	while( Stmt()->Fetch( row ) ) {
		io.WriteRow( row );
	}
	return 0;
}

//----------------------------------------------------------------------------
// use base to process commnin flags then process sql command flag
// if -sql flag value prefixed by '@', treat as file containing command
//----------------------------------------------------------------------------

void ODBCGetCommand :: ProcessFlags( const ALib::CommandLine & cmd ) {
	ODBCCommand::ProcessFlags( cmd );

	if ( cmd.HasFlag( FLAG_SQLTBL ) && cmd.HasFlag( FLAG_SQLQ ) ) {
		CSVTHROW( "Cannot specify both " << FLAG_SQLTBL
						<< " and " << FLAG_SQLQ << " flags" );
	}

	if ( cmd.HasFlag( FLAG_SQLTBL ) ) {
		string table = cmd.GetValue( FLAG_SQLTBL, "" );
		if ( table == "" ) {
			CSVTHROW( "Table name required" );
		}
		mSql = "SELECT * FROM [" + table + "]";
	}
	else if ( cmd.HasFlag( FLAG_SQLQ ) ) {
		mSql = cmd.GetValue( FLAG_SQLQ, "" );
		if ( mSql == "" ) {
			CSVTHROW( "SQL statement cannot be empty" );
		}
		if ( mSql[0] == '@' ) {
			string file = mSql.substr(1);
			mSql = "";
			ALib::FileRead( file, mSql );
		}
	}
	else {
		CSVTHROW( "Need one of " << FLAG_SQLTBL
					<< " or " << FLAG_SQLQ << " flags" );
	}
	mNull = cmd.GetValue( FLAG_NULLSTR, "NULL" );
}

//----------------------------------------------------------------------------

} // end namespace

// end

