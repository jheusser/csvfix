//---------------------------------------------------------------------------
// csved_printf.cpp
//
// printf-formatting
//
// Copyright (C) 2009 Neil Butterworth
//---------------------------------------------------------------------------

#include "csved_printf.h"
#include "csved_cli.h"
#include "csved_strings.h"

using std::string;

namespace CSVED {

//----------------------------------------------------------------------------
// Register printf command
//----------------------------------------------------------------------------

static RegisterCommand <PrintfCommand> rc1_(
	CMD_PRINTF,
	"printf-style formatting"
);

//----------------------------------------------------------------------------
// Help text
//----------------------------------------------------------------------------

const char * const PRINTF_HELP = {
	"performs printf-style formatting on CSV input\n"
	"usage: csvfix printf [flags] [files ...]\n"
	"where flags are:\n"
	"  -fmt fmt\tspecify fields and printf-style formatters\n"
	"#SMQ,SEP,IBL,IFN,OFL,SEP"
};

//----------------------------------------------------------------------------
// Standard ctor
//----------------------------------------------------------------------------

PrintfCommand :: PrintfCommand( const string & name,
								const string & desc )
		: Command( name, desc, PRINTF_HELP ) {

	AddFlag( ALib::CommandLineFlag( FLAG_FMT, true, 1 ) );

}

//----------------------------------------------------------------------------
// Create formatters and then use them to  format each row
//----------------------------------------------------------------------------

int PrintfCommand :: Execute( ALib::CommandLine & cmd ) {

	string fmt = cmd.GetValue( FLAG_FMT );
	ParseFormat( fmt );
//	DumpFmt();

	CSVRow row;
	IOManager io( cmd );

	while( io.ReadCSV( row ) ) {
		string line = FormatRow( row );
		io.Out() << line << std::endl;
	}
	return 0;
}

//----------------------------------------------------------------------------
// format single row - if fields are missing, treat them as being empty
//----------------------------------------------------------------------------

string PrintfCommand :: FormatRow( const CSVRow & row ) {

	const string fdouble = "feEgG" ;	// double conversions
	const string fint = "idxXouc";     // int conversions

	string s;
	unsigned int fieldno = 0;

	for ( unsigned int i = 0; i < mFmtLine.size(); i++ ) {
		if ( mFmtLine[i].mType == Literal ) {
			s += mFmtLine[i].mText;
		}
		else if ( mFmtLine[i].mType == Ignore ) {
			fieldno++;
			continue;
		}
		else {
			string field = fieldno >= row.size()
							? ""
							: row[fieldno];
			fieldno++;
			char c = ALib::StrLast( mFmtLine[i].mText );
			if ( fdouble.find( c ) != std::string::npos  ) {
				double d = ALib::IsNumber( field )
							? ALib::ToReal( field )
							: 0.0;
				s += ALib::Format( mFmtLine[i].mText.c_str(), d );
			}
			else if ( fint.find( c ) != std::string::npos ) {
				int n = ALib::IsInteger( field )
							? ALib::ToInteger( field )
							: 0;
				s += ALib::Format( mFmtLine[i].mText.c_str(), n );
			}
			else {
				s += ALib::Format( mFmtLine[i].mText.c_str(), field.c_str() );
			}
		}
	}

	return s;
}

//----------------------------------------------------------------------------
// debug dump
//----------------------------------------------------------------------------

void PrintfCommand :: DumpFmt() {
	for ( unsigned int i = 0; i < mFmtLine.size(); i++ ) {
		if ( mFmtLine[i].mType == Literal ) {
			std::cerr << "LIT: ";
		}
		else if ( mFmtLine[i].mType == Ignore ) {
			std::cerr << "IGNORE\n";
			return;
		}
		else {
			std::cerr << "FMT: ";
		}
		std::cout << mFmtLine[i].mText << "\n";
	}
}

//----------------------------------------------------------------------------
// create array of literals and formatters from -fmt parameter
//----------------------------------------------------------------------------

void PrintfCommand :: ParseFormat( const std::string & fmt ) {
	unsigned int pos = 0;

	while(1) {
		Format f = GetFormat( pos, fmt );
		if ( f.mType == None ) {
			break;
		}
		mFmtLine.push_back( f );
	}

	if ( mFmtLine.size() == 0 ) {
		CSVTHROW( "Empty format string not allowed" );
	}
}

//----------------------------------------------------------------------------
// helper to look forward safely
//----------------------------------------------------------------------------

static char Peek( unsigned int pos, const string & fmt ) {
	if ( pos >= fmt.size() ) {
		return 0;
	}
	else {
		return fmt[pos];
	}
}

//----------------------------------------------------------------------------
// get format from format string
//----------------------------------------------------------------------------

PrintfCommand::Format PrintfCommand :: GetFormat( unsigned int & pos,
													const string & fmt ) {
	const char IGNORE_CHAR = '@';

	if ( pos >= fmt.size() ) {
		return Format( None, "" );
	}
	char c = fmt[pos];
	if ( c == '%' &&
			(Peek( pos + 1, fmt ) == '%' || Peek( pos + 1, fmt ) == 0 ) ) {
		pos += 2;
		return Format( Literal, "%" );
	}
	else if ( c == '%' && Peek( pos + 1, fmt ) == IGNORE_CHAR ) {
		pos += 2;
		return Format( Ignore, "" );
	}
	else if ( c == '%') {
		return GetFormatter( pos, fmt );
	}
	else {
		return GetLiteral( pos, fmt );
	}
}

//----------------------------------------------------------------------------
// literals are things not beginning with '%'
//----------------------------------------------------------------------------

PrintfCommand::Format PrintfCommand :: GetLiteral( unsigned int & pos,
														const string & fmt ) {
	string lit;
	while( pos < fmt.size() && fmt[pos] != '%' ) {
		lit += fmt[pos++];
	}
	return Format( Literal, lit );
}

//----------------------------------------------------------------------------
// printf formatters begin with % and end with an alpha
//----------------------------------------------------------------------------

PrintfCommand::Format PrintfCommand :: GetFormatter( unsigned int & pos,
														const string & fmt ) {
	string f = "%";
	pos++;
	while( pos < fmt.size() ) {
		char c = fmt[pos++];
		f += c;
		if ( isalpha( c ) ) {
			string ok = "dioxXucsfeEgG";
			if ( ok.find( c ) == std::string::npos  ) {
				CSVTHROW( "Invalid conversion type: " << c );
			}
			return Format( Formatter, f );
		}

	}
	CSVTHROW( "Unexpected end of format" );
}


//------------------------------------------------------------------------

} // end namespace

// end

