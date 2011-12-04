//----------------------------------------------------------------------------
// csvparse.h
//
// Simple CSV parser class.
//
// Copyright (C) 2011 Neil Butterworth
//----------------------------------------------------------------------------

#include <sstream>
#include <stdexcept>

#include "csved_sparse.h"
#include "csved_except.h"

using std::string;

namespace CSVED {

//----------------------------------------------------------------------------
// Special characters - only change if you want to parse something that is
// not really CSV!
//----------------------------------------------------------------------------

const char END_INPUT 	= 0;
const char REC_SEP		= '\n';
const char DQUOTE		= '"';

//----------------------------------------------------------------------------
// Create parser, specifying stream to parse. A field separator can also
// be specified - you can use ';' for example if you want to parse MS-flavour
// CSV data. Not all characters can be used as a field separator, but this
// is not checked. You can also specify if double-quotes should be treated
// as special characters as per the IETF RFC.
//----------------------------------------------------------------------------

CSVParser :: CSVParser( std::istream & src, char fieldsep, bool dqspecial )
	: mSrc( src ), mFieldSep( fieldsep ), mDQSpecial( dqspecial ), mLineNo( 1 ) {
	NextChar();
}

//----------------------------------------------------------------------------
// Attempt to read next record from the input stream, populating the
// CSVRecord parameter. If a record was read succesfully, returns true. If
// nothing was read (i.e. at end-of-stream), returns false. If a parsing
// error occurs, throws an exception.
//----------------------------------------------------------------------------

bool CSVParser :: NextRecord( CSVRow & r ) {
	r.clear();
	while ( ! AtEndRec() ) {
		if ( HaveQuote() && mDQSpecial ) {
			ReadQuotedField( r );
		}
		else {
			ReadField( r );
		}
	}
	return r.size() == 0 ? false : true;
}


//----------------------------------------------------------------------------
// Read an unquoted field. Encountering a double-quote in this mode is an
// error, unless quoting has been turned off. Adds the field to the record.
//----------------------------------------------------------------------------

void CSVParser :: ReadField( CSVRow & r ) {
	string field;
	while ( ! AtEndField() ) {
		if ( mNext == DQUOTE && mDQSpecial ) {
			CSVTHROW( "Unexpected double-quote"  );
		}
		field += mNext;
		NextChar();
	}
	r.push_back( field );
	if ( mNext == mFieldSep ) {
		NextChar();
	}
}

//----------------------------------------------------------------------------
// Read a quoted field. Quoted fields may contain double-quotes escaped
// as "" and field and record separators. However, encountering the end of
// input within a quoted field is always an error.
//----------------------------------------------------------------------------

void CSVParser :: ReadQuotedField( CSVRow & r ) {
	string field;
	NextChar();
	while( mNext != END_INPUT ) {
		if ( HaveQuote() ) {
			if ( Peek() == DQUOTE ) {
				field += DQUOTE;
				NextChar();
			}
			else if ( Peek() == mFieldSep || Peek() == REC_SEP
						|| Peek() == END_INPUT ) {
				r.push_back( field );
				if ( Peek() == mFieldSep ) {
					NextChar();
				}
				NextChar();
				return;
			}
			else {
				CSVTHROW( "Illegal quoting" );
			}
		}
		else {
			field += mNext;
		}
		NextChar();
	}
	CSVTHROW( "Unexpected end of input" );
}

//----------------------------------------------------------------------------
// Is the current character a double-quote?
//----------------------------------------------------------------------------

bool CSVParser :: HaveQuote() const {
	return mNext == DQUOTE;
}

//----------------------------------------------------------------------------
// For non-quoted fields, tests if we have reached the end of a record, and if
// so discards the record-end character.
//----------------------------------------------------------------------------

bool CSVParser :: AtEndRec()  {
	if ( mNext == END_INPUT || mNext == REC_SEP ) {
		NextChar();
		return true;
	}
	else {
		return false;
	}
}

//----------------------------------------------------------------------------
// Is the current character one of the possible field terminators?
//----------------------------------------------------------------------------

bool CSVParser :: AtEndField() const {
	return mNext == mFieldSep || mNext == REC_SEP || mNext == END_INPUT;
}

//----------------------------------------------------------------------------
// Read the next character from input, setting mNext accordingly. Discards
// any carriage returns, but if a newline is encountered bumps the line count
// used for error reporting and resets the accumulated line text, which is
// used for error reporting.
//----------------------------------------------------------------------------

void CSVParser :: NextChar() {

	if ( ! mSrc.get( mNext ) ) {
		mNext = END_INPUT;
	}
	else if ( mNext == '\r' ) {
		NextChar();
	}
	else if ( mNext == REC_SEP ) {
		mLineNo++;
		mLine = "";
	}

	if ( mNext != '\n' ) {
		mLine += mNext;
	}
}

//----------------------------------------------------------------------------
// Peek at the next character in the stream without advancing the scan or
// updating the mNext member.
//----------------------------------------------------------------------------

char CSVParser :: Peek() {
	int c = mSrc.peek();
	return c == std::char_traits<char>::eof() ? END_INPUT : c;
}

} // namespace

// end

