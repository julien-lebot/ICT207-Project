/*
*	ObjReader.hpp
*
*	obj 3d model file reader
*
*	Created by Terje Loe
*/
#ifndef OBJREADER_HPP
#define OBJREADER_HPP

#include <Phoenix/Base.h>
#include <iostream>
#include <fstream>
#include <string>
#include <ObjToSwm/Token.hpp>
#include <ObjToSwm/ModelFileReader.hpp>

/*
 * @brief Class reading obj 3d files into a Model Class
 *
 * The class is given a filename string and a instance of the Model class through the readFile function
 * wich parse the file and stores it in the Model object
 */
namespace Phoenix 
{
	class _PhoenixExport ObjReader : public ModelFileReader
	{
	public:
		/// Default constructor
		ObjReader ()
		: m_activeGroup (0), m_activeCollection (0), inFileIsMtl(false)
		{}

		/*
		 * read file into Model class
		 * @param fileName filename to be read
		 * @param Model object to store model
		 */
		void readFile ( const std::string& fileName, Phoenix::Model& model );
	private:
		/*
		 * reads a line from the file 
		 * @param inFole ifstream object to read from
		 * @param model object to store model
		 */
		void readLines ( std::ifstream& inFile, Phoenix::Model& model );

		/*
		 * read f lines from the obj file
		 * @param t Token object with current line
		 */	
		void readFaceLine ( Token& t ) const;
		
		/*
		 * adds a FaceCollection to the active group
		 * @param fc FaceCollection to add
		 */
		void addFaceCol(Phoenix::FaceCollection& fc) const;

		/*
		 * given a quad face and returns a vector with two triangle faces
		 * @param quad vector with a quad face
		 * @return triangle vector
		 */	
		std::vector<int> quadToTriangle ( std::vector<int> quad ) const;
		
		/*
		 * reads a new material from mtl files
		 * @param inFile ifstream object to read from
		 * @param t Token object with current line
		 * @return Material structure
		 */
		Phoenix::Material readNewMaterial ( std::ifstream& inFile, Token& t ) const;

		/// stores the active group
		Phoenix::FaceGroup* m_activeGroup;

		/// stores the active face collection (faces in a group with the same material)
		Phoenix::FaceCollection* m_activeCollection;

		/// stores if the file being read is a Mtl file.
		bool inFileIsMtl;
	};
}
#endif