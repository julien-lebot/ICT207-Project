#include <ObjToSwm/token.hpp>

using namespace Phoenix;

void Token::tokenizeStr ( std::string str,const std::string delim )
{
	clear();
	
	if(str[0] == 9)
	{
		str.erase(str.begin());
	}

	std::string::size_type startPos = str.find_first_not_of ( delim,0 );
	std::string::size_type endPos = str.find_first_of ( delim, startPos );

	while ( std::string::npos != endPos || std::string::npos != startPos )
	{
		tokens.push_back ( str.substr ( startPos, endPos - startPos ) );
        startPos = str.find_first_not_of ( delim, endPos );
        endPos = str.find_first_of ( delim, startPos );
	}
}

void Token::clear ()
{
	tokens.clear ();
	current = 0;
}

const std::string Token::getNext ()
{
	if ( allFetched () == true )
		return " ";
	else
		return tokens[current++];
}

const std::string Token::getRest ()
{
	std::string str = "";
	
	while ( !allFetched () ) {
		str.append ( tokens[current++] );
		if ( !allFetched () )
			str.append ( " " );
	}

	return str;
}