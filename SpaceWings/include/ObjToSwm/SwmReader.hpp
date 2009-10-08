/*
*	SwmReader.hpp
*
*	Space Wings Model (swm) file reader class
*
*	Created by Terje Loe
*/

#ifndef SWMREADER_HPP
#define SWMREADER_HPP

#include <iostream>
#include <fstream>
#include <string>
#include <ObjToSwm\Token.hpp>
#include <ObjToSwm\ModelFileReader.hpp>

/*
 * @breif Class that reads swm files into a Model class
 *
 * The class is given a filename string and a instance of the Model class through the readFile function
 * wich parse the file and stores it in the Model object
 */
namespace Phoenix
{
	class SwmReader : public ModelFileReader
	{
	public:
		/*
		 * read a file into Model class
		 * @Param fileName filename to be read
		 * @Param model object to store model
		 */
		void readFile( std::string fileName, Phoenix::Model& model );
	private:
		/* reading tag and calling the right parse function
		 * @param inFile ifstream object to parse from
		 * @param model object to store in
		 */
		void parseFile ( std::ifstream& inFile, Phoenix::Model& model );
		
		/*
		 * parse the possition section of the file
		 * @param inFile ifstream object to parse from
		 * @param model object to store in
		 */
		void parsePos ( std::ifstream& inFile, Phoenix::Model& model );
		
		/*
		 * parse the collision section of the file
		 * @param inFile ifstream object to parse from
		 * @param model object to store in
		 */
		void parseCol ( std::ifstream& inFile, Phoenix::Model& model );
		
		/*
		 * parse the vertice section of the file
		 * @param inFile ifstream object to parse from
		 * @param model object to store in
		 */
		void parseVertice( std::ifstream& inFile, Phoenix::Model& model );

		/*
		 * parse the vertice texture section of the file
		 * @param inFile ifstream object to parse from
		 * @param model object to store in
		 */
		void parseVTexture ( std::ifstream& inFile, Phoenix::Model& model );

		/*
		 * parse the vertice normal section of the file
		 * @param inFile ifstream object to parse from
		 * @param model object to store in
		 */
		void parseVNormal ( std::ifstream& inFile, Phoenix::Model& model );

		/*
		 * parse the offset section of the file
		 * @param inFile ifstream object to parse from
		 * @param model object to store in
		 */
		void parseOffset ( std::ifstream& inFile, Phoenix::Model& model );
		
		/*
		 * parse the material section of the file
		 * @param inFile ifstream object to parse from
		 * @param model object to store in
		 */
		void parseMaterial ( std::ifstream& inFile, Phoenix::Model& model );

		/*
		 * gets a new line from the file and tokenizes it
		 * @param inFile ifstream object get new line from
		 * @param skipLine how many lines that should be skiped
		 */
		void setNextTokens( std::ifstream& inFile, unsigned skipLine = 0);

		/* check if the current line is a end tag
		 * @return bool value - true if line is end tag
		 */
		const bool isEndTag() const { return ( ( m_line.size () >= 2 ) && (m_line.at(0) == '<' && m_line.at(1) == '/' ) ); }

		/// stores the current line tokens
		Token m_token;

		/// stores the current line
		std::string m_line;
	};
}

#endif