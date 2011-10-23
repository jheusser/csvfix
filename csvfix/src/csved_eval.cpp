//---------------------------------------------------------------------------
// csved_eval.cpp
//
// eval command for csvfix - does expression evaluation
// this doesn't do much - the heavy lifting is in alib
//
// Copyright (C) 2009 Neil Butterworth
//---------------------------------------------------------------------------

#include "a_collect.h"
#include "csved_cli.h"
#include "csved_eval.h"
#include "csved_strings.h"

using std::string;

namespace CSVED {

//----------------------------------------------------------------------------
// Register eval command
//----------------------------------------------------------------------------

static RegisterCommand <EvalCommand> rc1_(
	CMD_EVAL,
	"expression evaluation"
);

//----------------------------------------------------------------------------
// Help text
//----------------------------------------------------------------------------

const char * const EVAL_HELP = {
	"performs expression evaluation on fields, adding results as new fields\n"
	"usage: csvfix eval [flags] [file ...]\n"
	"where flags are:\n"
	"  -e expr\texpression to evaluate\n"
	"#ALL"
};

//----------------------------------------------------------------------------
// Standard command ctor
//----------------------------------------------------------------------------

EvalCommand ::	EvalCommand( const string & name,
									 const string & desc )
		: Command( name, desc, EVAL_HELP ) {
		AddFlag( ALib::CommandLineFlag( FLAG_EXPR, true, 1, true ) );
}

//----------------------------------------------------------------------------
// compile all expressions and then evaluate for eaxh input row
//----------------------------------------------------------------------------

int EvalCommand ::	Execute( ALib::CommandLine & cmd ) {

	IOManager io( cmd );
	CSVRow row;

	GetExpressions( cmd );

	while( io.ReadCSV( row ) ) {
		SetParams( row, io );
		Evaluate( row );
		io.WriteRow( row );
	}

	return 0;
}

//----------------------------------------------------------------------------
// evaluate expressions, appending them to row
//----------------------------------------------------------------------------

void EvalCommand ::	Evaluate( CSVRow & row ) {
	for ( unsigned int i = 0; i < mExprs.size() ; i++ ) {
		string r = mExprs[i].Evaluate();
		row.push_back( r );
	}
}

//----------------------------------------------------------------------------
// Set positional parameters (each such parameter is a field in the CSV input)
// and special named constants
//----------------------------------------------------------------------------

const char * const LINE_VAR = "line";	// var containing current line no
const char * const FILE_VAR = "file";	// var containing current file name
const char * const FIELD_VAR = "fields"; // var containing CSV field count

void EvalCommand ::	SetParams( const CSVRow & row, IOManager & iom ) {
	for ( unsigned int i = 0; i < mExprs.size(); i++ ) {
		mExprs[i].ClearPosParams();
		mExprs[i].AddVar( LINE_VAR, ALib::Str( iom.CurrentLine() ));
		mExprs[i].AddVar( FILE_VAR, ALib::Str( iom.CurrentFileName()));
		mExprs[i].AddVar( FIELD_VAR, ALib::Str( row.size()));
		for ( unsigned int j = 0; j < row.size(); j++ ) {
			mExprs[i].AddPosParam( row.at( j ) );

		}
	}
}

//----------------------------------------------------------------------------
// get expressions specified by -e expr and compile them
//----------------------------------------------------------------------------

void EvalCommand ::	GetExpressions( ALib::CommandLine & cmd ) {
	int i = 2;	// skip exe name and command name
	while( i < cmd.Argc() ) {
		if ( cmd.Argv( i ) == FLAG_EXPR ) {
			if ( i + 1 >= cmd.Argc() ) {
				CSVTHROW( "Missimg expression" );
			}
			i++;
			string expr = cmd.Argv( i );
			ALib::Expression ex;
			ex.Compile( expr );
			mExprs.push_back( ex );
		}
		i++;
	}
}

//------------------------------------------------------------------------

} // namespace

// end

