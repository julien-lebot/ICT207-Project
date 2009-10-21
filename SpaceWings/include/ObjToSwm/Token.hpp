/*
*	Token.hpp
*
*	Simple string tokenizer class
*
*	Created by Terje Loe
*/

#ifndef TOKEN_HPP
#define TOKEN_HPP

#include <Phoenix/Base.h>
#include <vector>
#include <string>

/*
 * @brief Tokenizer
 *
 * Tokenize and store tokens
 */
class _PhoenixExport Token
{
public:
	/// Default constructor
	Token (): current ( 0 ) {}
	
	/* construct object with a string
	 * @param &str string reference to tokenize
	 */
	Token ( const std::string &str ) :
	current ( 0 )
	{ tokenizeStr ( str, " " ); }
	
	///Destructor
	~Token() {}
	
	/// Copy constructor
	Token ( const Token& rhs )
	: current( rhs.current ), tokens ( rhs.tokens )
	{}
	
	/// Assignment operator
	Token& operator= ( const Token& rhs )
	{
		current = rhs.current;
		tokens = rhs.tokens;
		return *this;
	}

	/// clears object
	void clear ();

	/// returns next string
	const std::string getNext ();

	/// returns the rest of the string
	const std::string getRest ();

	/* returns element number n or a blank element if vector i empty
	 * @param n element index
	 * @return the n'th element in the Token object
	 */
	const std::string getElement ( unsigned n ) const { return ( tokens.size () <= n) ? " " : tokens[n]; }
	
	/* returns element count
	 *  @return the size of the tokens vector
	 */
	unsigned size (){ return tokens.size(); }

	/* returns true if all elements have been fetched
	 * @return bool value
	 */
	const bool allFetched() { return current == tokens.size (); }

	/* tokenize string using the delim as delimiter
	 * @param str string to tokenize
	 * @param delim string containing chars to be used for tokenize
	 */
	void tokenizeStr ( std::string str, const std::string delim );
private:
	/// vectore storeing tokens
	std::vector<std::string> tokens;
	
	/// the element that was last returned by getElement()
	int current;
};

#endif