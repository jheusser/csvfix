//---------------------------------------------------------------------------
// csved_timestamp.cpp
//
// add timestamps to CSV input
//
// Copyright (C) 2012 Neil Butterworth
//---------------------------------------------------------------------------

#include "a_base.h"
#include "a_collect.h"
#include "csved_cli.h"
#include "csved_timestamp.h"
#include "csved_strings.h"

#include <iomanip>
#include <sstream>
#include <string>
using std::string;

//----------------------------------------------------------------------------

namespace CSVED {

//---------------------------------------------------------------------------
// Register timestamp command
//---------------------------------------------------------------------------

static RegisterCommand <TimestampCommand> rc1_(
	CMD_TSTAMP,
	"add timestamp to CSV data"
);

//----------------------------------------------------------------------------
// Help
//----------------------------------------------------------------------------

const char * const TSTAMP_HELP = {
	"adds a timestamp field to CSV input data\n"
	"usage: csvfix timestam  [flags] [file ...]\n"
	"where flags are:\n"
	"  -rt \t\tubdate stamp in real time as records are written\n"
	"#ALL"
};

//----------------------------------------------------------------------------
// The timestamp command
//----------------------------------------------------------------------------

TimestampCommand :: TimestampCommand( const string & name, const string & desc )
						: Command( name, desc, TSTAMP_HELP ) {

	AddFlag( ALib::CommandLineFlag( FLAG_RTIME, false, 0 ) );
}


//----------------------------------------------------------------------------
// Format timestamp depending on user options. std::put_time not yet
// available for gcc, so we do the formatting ourselves.
//----------------------------------------------------------------------------

string TimestampCommand :: FormatStamp( std::time_t t ) const {

    std::tm tm = * std::localtime( & t );
    std::ostringstream os;

    os << tm.tm_year + 1900 << "-"
       << std::setw(2) << std::setfill( '0' ) <<  tm.tm_mon + 1<< "-"
       << std::setw(2) << std::setfill( '0' ) <<  tm.tm_mday + 1
       << " "
       << std::setw(2) << std::setfill( '0' ) <<  tm.tm_hour << ":"
       << std::setw(2) << std::setfill( '0' ) <<  tm.tm_min << ":"
       << std::setw(2) << std::setfill( '0' ) <<  tm.tm_sec
	;

	return os.str();
}

//----------------------------------------------------------------------------
// If not using the real-time flag, only get time stamp once and stamp
// all records with the same value. Otherwise, the stamp is updated
// on each record read.
//----------------------------------------------------------------------------

int TimestampCommand :: Execute( ALib::CommandLine & cmd ) {

	ProcessFlags( cmd );

	IOManager io( cmd );
	CSVRow row;

	string stamp = FormatStamp( std::time(0) );

	while( io.ReadCSV( row ) ) {
		CSVRow out;
		out.push_back( stamp );
		ALib::Append( out, row );
		io.WriteRow( out );
	}

	return 0;
}


//----------------------------------------------------------------------------
// Handle command line options
//----------------------------------------------------------------------------

void TimestampCommand :: ProcessFlags( const ALib::CommandLine & cmd ) {
}


//----------------------------------------------------------------------------


} // namespace

