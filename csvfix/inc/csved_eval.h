//---------------------------------------------------------------------------
// csved_eval.h
//
// evaluation command
//
// Copyright (C) 2009 Neil Butterworth
//---------------------------------------------------------------------------

#ifndef INC_CSVED_TRIM_H
#define INC_CSVED_TRIM_H

#include "a_base.h"
#include "a_expr.h"
#include "csved_command.h"

namespace CSVED {

//---------------------------------------------------------------------------

class EvalCommand : public Command {

	public:

		EvalCommand( const std::string & name,
						const std::string & desc );

		int Execute( ALib::CommandLine & cmd );

	private:

		void SetParams( const CSVRow & row, class IOManager & iom );
		void Evaluate( CSVRow & row );
		void GetExpressions( ALib::CommandLine & cmd );
		std::vector <ALib::Expression> mExprs;

};


//------------------------------------------------------------------------

}	// end namespace

#endif

