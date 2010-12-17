//---------------------------------------------------------------------------
// csved_odbc.h
//
// odbc database stuff for csvfix
//
// Copyright (C) 2009 Neil Butterworth
//---------------------------------------------------------------------------

#ifndef INC_CSVED_ODBC_H
#define INC_CSVED_ODBC_H

#include "a_base.h"
#include "a_db.h"
#include "csved_command.h"

namespace CSVED {

//---------------------------------------------------------------------------
// Base command handles connectivity
//----------------------------------------------------------------------------

class ODBCCommand : public Command {

	public:

		ODBCCommand( const std::string & name,
						const std::string & desc );
		~ODBCCommand();

	protected:

		void Connect();
		void Exec( const std::string & sql );
		ALib::DbStatement * Stmt();
		void ProcessFlags( const ALib::CommandLine & cmd );

	private:

		ALib::DbConnection mConnection;
		ALib::DbStatement * mStatement;

		std::string mConnStr;
};

//----------------------------------------------------------------------------
// Currently only inplement get command
//----------------------------------------------------------------------------

class ODBCGetCommand : public ODBCCommand {

	public:

		ODBCGetCommand( const std::string & name,
						const std::string & desc );

		int Execute( ALib::CommandLine & cmd );

	private:

		void ProcessFlags( const ALib::CommandLine & cmd );

		std::string mSql, mNull;
};

//------------------------------------------------------------------------

}	// end namespace

#endif

