//----------------------------------------------------------------------------
// a_expr.h
//
// simple expression eveluation
//
// Copyright (C) 2009 Neil Butterworth
//----------------------------------------------------------------------------

#ifndef INC_A_EXPR_H
#define INC_A_EXPR_H

#include "a_base.h"
#include "a_dict.h"
#include <stack>
#include <iosfwd>
#include <deque>

namespace ALib {

//----------------------------------------------------------------------------
// Tokens have type and value (and prcedence for operator tokens)
//----------------------------------------------------------------------------

class ExprToken {

	public:

		enum ETType {
			etNone, etOp, etNum, etStr, etVar, etFunc, etError, etDone
		};

		ExprToken() : mType( etNone ), mValue( "" ), mPrec(-1) {}

		ExprToken( ETType type, const std::string & value, int prec = -1 )
			: mType( type ), mValue( value ), mPrec( prec ) {}

		ETType Type() const {
			return mType;
		}

		const std::string & Value() const {
			return mValue;
		}

		int Precedence() const {
			return mPrec;
		}

		bool IsSep() const;
		static ExprToken MakeSep();
		bool operator == ( const ExprToken & et ) const;

		void DumpOn( std::ostream & os ) const;

	private:

		ETType mType;
		std::string mValue;
		int mPrec;
};

//----------------------------------------------------------------------------
/// Simple arithmetic & string expression evaluator
//----------------------------------------------------------------------------

class Expression {

	public:

		// reverse polish representation
		typedef std::vector <ExprToken> RPNRep;

		// type for functions within expression
		typedef std::string (*FuncImpl)( const std::deque <std::string> & );

		/// create empty expression - evaluation will fail
		Expression();
		~Expression();

		/// Compile expression into internal format. Returns string containing
		/// error message, or empty string on success.
		std::string Compile( const std::string & expr );

		/// Evaluate compiled expression returning string containg result
		std::string Evaluate();

		/// Wrapper to botyh compile and evaluate
		std::string Evaluate( const std::string & expr );

		/// Clear any positional ($1, $2 etc.) parameters
		void ClearPosParams();

		/// Add possitional parameter. Firsty is $1, second $2 and so on.
		void AddPosParam( const std::string & s );

		/// Clear all named variables
		void ClearVars();

		/// Add named variable with value
		void AddVar( const std::string & name, const std::string & val );

		struct AddFunc {
			FuncImpl mFunc;
			unsigned int mParamCount;

			AddFunc( const std::string & name, FuncImpl fi, unsigned int np )
				: mFunc( fi ), mParamCount( np ) {
				Expression::AddFunction( name, * this );
			}
		};

		/// Add anmed function with function descriptor struct
		static void AddFunction( const std::string & name, const AddFunc & f );
		static bool ToBool( const std::string & s );

	private:

		std::string PopStr();
		double PopNum();
		std::string GetVar( const std::string & var ) const;
		void DoCompare( const std::string & op );
		void DoAndOr( const std::string & op );
		void PushBool( bool b );
		void ExecOp( const ExprToken & tok );
		bool EvalSingleExpr( unsigned int & ei, std::string & result );
		std::string CallFunction( const std::string & name );

		std::vector <std::string> mPosParams;
		std::stack <std::string> mStack;
		Dictionary <std::string> mVars;

		RPNRep mRPN;

		static Dictionary <AddFunc> mFuncs;


};

//----------------------------------------------------------------------------

}	// namespace

#endif
