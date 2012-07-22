//---------------------------------------------------------------------------
// csved_call.h
//
// call dll function from csvfix
//
// Copyright (C) 2012 Neil Butterworth
//---------------------------------------------------------------------------

#ifndef INC_CSVED_CALL_H
#define INC_CSVED_CALL_H

#include "a_base.h"
#include "a_expr.h"
#include "csved_command.h"

namespace CSVED {


//----------------------------------------------------------------------------
// Function in DLL needs to look like this
//----------------------------------------------------------------------------

extern "C" {
	typedef int (*FuncType)( int ifc, const char * input, int * ofc, char * output, int outsize  );
}

//----------------------------------------------------------------------------
// Call command
//----------------------------------------------------------------------------

class CallCommand : public Command {

	public:

		CallCommand( const std::string & name,
					const std::string & desc );

		int Execute( ALib::CommandLine & cmd );


	private:

		void ProcessFlags( const ALib::CommandLine & cmd );
		int CallOnFields( CSVRow & row );
		std::string mDLL, mFuncName;
		FieldList mFields;
		FuncType mFunc;

		static const unsigned int OUTBUF_SIZE = 2048;
		static char mOutBuf[ OUTBUF_SIZE ];

};

//----------------------------------------------------------------------------

} // namespace

#endif
