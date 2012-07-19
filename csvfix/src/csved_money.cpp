//---------------------------------------------------------------------------
// csved_money.cpp
//
// money formatting for csvfix
//
// Copyright (C) 2009 Neil Butterworth
//---------------------------------------------------------------------------

#include "csved_cli.h"
#include "csved_money.h"
#include "csved_except.h"
#include "csved_ioman.h"
#include "csved_strings.h"

using std::string;

namespace CSVED {

//---------------------------------------------------------------------------
// Register money  command
//---------------------------------------------------------------------------

static RegisterCommand <MoneyCommand> rc1_(
	CMD_MONEY,
	"format fields as money/currency values"
);

//----------------------------------------------------------------------------
// Help text
//----------------------------------------------------------------------------

const char * const MONEY_HELP = {
	"format fields as money/currency values\n"
	"usage: csvfix money [flags] [file ...]\n"
	"where flags are:\n"
	"  -f fields\tfields to apply edits to - default is all fields\n"
	"  -dp chr\tuse character chr as decimal point symbol - default is full-stop\n"
	"  -ts chr\tuse character chr as thousands separator - default is comma\n"
	"  -cs sym\tuse string sym as currency symbol - default is none\n"
	"  -r\t\treplace fields with new format - default is to append fields\n"
	"#ALL"
};

//---------------------------------------------------------------------------
// Standard command ctor
//---------------------------------------------------------------------------

MoneyCommand :: MoneyCommand( const string & name,
							const string & desc )
	: Command( name, desc, MONEY_HELP ) {
	AddFlag( ALib::CommandLineFlag( FLAG_DPOINT, false, 1 ) );
	AddFlag( ALib::CommandLineFlag( FLAG_KSEP, false, 1 ) );
	AddFlag( ALib::CommandLineFlag( FLAG_COLS, false, 1 ) );
	AddFlag( ALib::CommandLineFlag( FLAG_CURSYM, false, 1 ) );
	AddFlag( ALib::CommandLineFlag( FLAG_REPLACE, false, 0 ) );
}

//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

int MoneyCommand :: Execute( ALib::CommandLine & cmd ) {

	IOManager io( cmd );
	CSVRow row;

	while( io.ReadCSV( row ) ) {
		io.WriteRow( row );
	}

	return 0;
}

//---------------------------------------------------------------------------

void MoneyCommand :: ProcessFlags( ALib::CommandLine & cmd ) {
}

//----------------------------------------------------------------------------

}	// end namespace

// end
