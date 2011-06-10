//---------------------------------------------------------------------------
// csved_merge.h
//
// merge multiple sorted CSV files
//
// Copyright (C) 2011 Neil Butterworth
//---------------------------------------------------------------------------

#ifndef INC_CSVED_FMERGE_H
#define INC_CSVED_FMERGE_H

#include "a_base.h"
#include "csved_command.h"

namespace CSVED {

class FMergeCommand : public Command {

	public:

		FMergeCommand( const std::string & name,
					const std::string & desc );

		int Execute( ALib::CommandLine & cmd );


	private:

		void ProcessFlags( ALib::CommandLine & cmd );

		FieldList mFields;
};

}

#endif

