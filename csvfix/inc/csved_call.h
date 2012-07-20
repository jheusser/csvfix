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

class CallCommand : public Command {


	public:

		CallCommand( const std::string & name,
					const std::string & desc );

		int Execute( ALib::CommandLine & cmd );


	private:

		void ProcessFlags( const ALib::CommandLine & cmd );

};

//----------------------------------------------------------------------------

} // namespace

#endif
