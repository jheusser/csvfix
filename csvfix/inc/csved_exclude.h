//---------------------------------------------------------------------------
// csved_exclude.h
//
// exclude specified columns from output
//
// Copyright (C) 2009 Neil Butterworth
//---------------------------------------------------------------------------

#ifndef INC_CSVED_EXCLUDE_H
#define INC_CSVED_EXCLUDE_H

#include "a_base.h"
#include "csved_command.h"
#include "csved_types.h"

namespace CSVED {

//---------------------------------------------------------------------------

class ExcludeCommand : public Command {

	public:

		ExcludeCommand( const std::string & name,
						const std::string & desc );

		int Execute( ALib::CommandLine & cmd );

	private:

		void ProcessFlags( const ALib::CommandLine & cmd );
		CSVRow Exclude( const CSVRow & r ) const;

		FieldList mFields;

};

//------------------------------------------------------------------------

}	// end namespace

#endif

