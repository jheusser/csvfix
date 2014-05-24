//---------------------------------------------------------------------------
// csved_pivot.h
//
// Simple pivot table stuff
//
// Copyright (C) 2014 Neil Butterworth
//---------------------------------------------------------------------------

#ifndef INC_CSVED_PIVOT_H
#define INC_CSVED_PIVOT_H

#include "a_base.h"
#include "csved_command.h"

namespace CSVED {

//---------------------------------------------------------------------------

class PivotCommand : public Command {

	public:

		PivotCommand( const std::string & name,
						const std::string & desc );

		int Execute( ALib::CommandLine & cmd );

        enum class Action {
            Sum, Count, Average
        };


	private:

        void ProcessFlags( const ALib::CommandLine & cmd );

        Action mAction;
        unsigned int mCol, mRow;

};


//------------------------------------------------------------------------

}	// end namespace

#endif

