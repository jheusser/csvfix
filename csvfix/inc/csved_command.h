//------------------------------------------------------------------------
// csved_command.h
//
// Base class for CSVED commands
//
// Copyright (C) 2008 Neil Butterworth
//------------------------------------------------------------------------

#ifndef INC_CSVED_COMMAND_H
#define INC_CSVED_COMMAND_H

#include "a_base.h"
#include "a_env.h"

#include "csved_ioman.h"
#include "csved_types.h"

namespace CSVED {

//------------------------------------------------------------------------
// Base class for all commands - handles flags, help etc.
//----------------------------------------------------------------------------

class Command {

	public:

		enum class SkipType { None, Skip, Pass };

		Command( const std::string & name,
				  const std::string & desc,
				  SkipType s = SkipType::None );

		Command( const std::string & name,
				  const std::string & desc,
				  const std::string & help,
				  SkipType s = SkipType::None );

		virtual ~Command();

		virtual int Execute( ALib::CommandLine & cmd ) = 0;

		virtual std::string Name() const;
		virtual std::string Desc() const;
		virtual std::string Help() const;

		void AddHelp( const std::string & help );

		virtual void CheckFlags( ALib::CommandLine & cmd );
		int CountNonGeneric(  const ALib::CommandLine & cmd ) const;

		void AddFlag( const ALib::CommandLineFlag & f );

		bool Skip( const CSVRow & r ) const;
		bool Pass( const CSVRow & r ) const;

	private:

		std::string mName, mDesc;
		std::vector <ALib::CommandLineFlag> mFlags;
		std::string mHelp;
		SkipType mSkipType;
};

//------------------------------------------------------------------------

}	// namespace


#endif


