/*
*	SwmWriter.hpp
*
*	swm (Space Wings Model) file write class
*
*	Created by Terje Loe
*/

#ifndef SWMWRITER_HPP
#define SWMWRITER_HPP

#include <vector>
#include <fstream>
#include <string>
#include <iostream>
#include <Phoenix\model.hpp>
#include <ObjToSwm\ModelFileWriter.hpp>

/*
 * @brief Class that write a Model object to a swm file
 *
 * The class is given a filename string and a instance of the Model class through the writeFile function.
 */

namespace Phoenix
{
	class SwmWriter : public ModelFileWriter
	{
	public:		
		/* read file into Model class
		 * @param fileName filename to write to
		 * @param Model object write from 	 
		 */
		void writeFile ( const std::string& fileName, const Model& model );

	private:
		/* returns a string with a start tag
		 * @param str content of start tag
		 * @return start tag string
		 */
		const std::string startTag ( const std::string& str ) const { return "<" + str + ">"; }

		/* returns a string with a end tag
		 * @param str content of end tag
		 * @return end tag string
		 */
		const std::string endTag ( const std::string& str ) const { return "</" + str + ">"; }
		
		/* returns a tab string
		 * @param n how many tabs the string should contain.
		 * @return tabed string
		 */
		const std::string tab ( const unsigned n ) const;

		/* writes possition part of file
		 * @param pos a Possition structure
		 */
		void writePossition ( const Phoenix::Possition pos );
		
		/* writes collision part of file
		 * @param col Collision struct
		 */
		void writeCollision ( const Collision col );
		
		/* writes the vectice vector to file
		 * @param verticeVec float vector
		 */
		void writeVertice ( const std::vector<float>& verticeVec );
		
		/* writes the vectice textures vector to file
		 * @param vTextureVec float vector
		 */
		void writeVTexture (const std::vector<float>& vTextureVec);
		
		/* writes the vectice normal vector to file
		 * @param vNormalVec float vector
		 */
		void writevNormal ( const std::vector<float>& vNormalVec );

		/* writes a FaceGroup vector to file
		 * @param groupVec FaceGroup vector
		 */
		void writeFaceGroup( const std::vector<Phoenix::FaceGroup>& groupVec );
		
		/*	writes a face collection vector to the file
		 * @param faceVec FaceCollection vector
		 */
		void writeFaces( const std::vector<Phoenix::FaceCollection>& faceVec );
		
		/* writes a material vector to the line
		 * @param mtlVec Material vector
		 */
		void writeMaterial ( const std::vector<Phoenix::Material>& mtlVec );
		
		/* writes out a float vector to the outFile
		 * @param vec float vector
		 * @param lineCount how many floats that should be printed to each line
		 * @param tabN how many tabs should be put in front of each line
		 */
		void writeFloatVec ( const std::vector<float>& vec, const unsigned lineCount, const unsigned tabN );
		
		/* writes out a integer vector to the outFile
		 * @param vec integer vector
		 * @param tabN how many tabs should be put in front of line
		 */
		void writeIntVec ( const std::vector<int>& vec, const unsigned tabN);
		
		/// stores the ofstream object ot write to
		std::ofstream outFile;
	};
}

#endif