/*
*	Model.hpp
*
*	Model class used to store models.
*
*	Created by Terje Loe
*/

#ifndef MODEL_HPP
#define MODEL_HPP

#include <vector>
#include <iostream>
#include <string>
#include <Phoenix\Color.hpp>
#include <Phoenix\Material.hpp>

namespace Phoenix
{
	struct Collision
	{
	public:
		float aabbWidth;
		float aabbHight;
		float radius;
	};

	struct Possition
	{
	public:
		bool staticObj;
		float x;
		float y;
		float z;
	};

	struct FaceCollection
	{
	public:	
		std::string mtlName;
		std::vector<int> v;
		std::vector<int> vn;
		std::vector<int> vt;
	};

	struct FaceGroup
	{
	public:
		std::string groupName;
		std::vector<Phoenix::FaceCollection> faces;
	};

	/*
	* @breif Class to store models
	*
	* Class is used to store models used in the game.
	*/
	class Model
	{
	public:
		/// Default constructor
		Model ()
		{
			pos.staticObj = false;
			pos.x = 0;
			pos.y = 0;
			pos.z = 0;
			col.aabbWidth = 0;
			col.aabbHight = 0;
			col.radius = 0;
		}

		/// Copy constructor
		Model ( const Phoenix::Model& rhs )
		: vertices ( rhs.vertices ), vTextures ( rhs.vTextures ), vNormals ( rhs.vNormals ), faceGroups ( rhs.faceGroups ), mtlList ( rhs.mtlList )
		{
			col = rhs.col;
			pos.staticObj = rhs.pos.staticObj;
			pos.x = rhs.pos.x;
			pos.y = rhs.pos.y;
			pos.z = rhs.pos.z;
		}

		/// Assignment operator
		Phoenix::Model& operator= ( const Phoenix::Model& rhs )
		{
			vertices = rhs.vertices;
			vTextures = rhs.vTextures;
			vNormals = rhs.vNormals;
			faceGroups = rhs.faceGroups;
			mtlList = rhs.mtlList;
			col = rhs.col;
			pos.staticObj = rhs.pos.staticObj;
			pos = rhs.pos;
			return *this;
		}

		/// Destructor
		~Model () {}

		/// Sets the possition part of object from user input
		void setPossition ();

		/* Get a instance of Possition structure and saves it in the object
		 * @param p Possition object to store
		 */
		void setPossition ( const Phoenix::Possition& p ) { pos = p; }

		/// sets the collision part of object from user input
		void setCollision ();

		/* Get a instance of Collision structure and saves it in the object 
		 * @param c Collision object to store
		 */
		void setCollision ( const Phoenix::Collision& c ) { col = c; }

		/* sets object static value
		 * @param b value to save
		 */
		void setStatic( const bool b ) { pos.staticObj = b; }
		
		/* adds a vertice to vertice vector
		 * @param f vertice to be added
		 */
		void addVertice ( const float f ) { vertices.push_back(f); }

		/* adds a vertice texture to vertice texture vector
		 * @param f vertice to be added	 
		 */
		void addVTexture ( const float f ) { vTextures.push_back(f); }
		
		/* adds a vertice normal to vertice normal vector
		 * @param f vertice to be added
		 */
		void addvNormals ( const float f ) { vNormals.push_back(f); }
		
		/* adds a FaceGroup to the FaceGroup vector
		 * @param fg FaceGroup to add
		 */
		void addFaceGroup ( const Phoenix::FaceGroup& fg ) { faceGroups.push_back(fg); }
		
		/* adds a Material to the material list
		 * @param m Material to be added
		 */
		void addMaterial ( const Phoenix::Material& m ) { mtlList.push_back(m); }

		/* gets the vertice vector
		 * @return reference to  vertice vector
		 */
		const std::vector<float>& getVerticeVec () const  { return vertices; }
		
		/* gets the vertice texture vector
		 * @return reference to  vertice texture vector
		 */
		const std::vector<float>& getVTextureVec () const { return vTextures; }

		/* gets the vertice normal vector
		 * @return reference to vertice normal vector
		 */
		const std::vector<float>& getVNormalVec () const { return vNormals; }

		/* gets the face group vector
		 * @return reference to face group vector
		 */
		const std::vector<Phoenix::FaceGroup>& getFaceGroupVec () const { return faceGroups; }
		
		/* gets the material vector
		 * @return reference to material vector
		 */
		const std::vector<Phoenix::Material>& getMtlListVec () const { return mtlList; }
		
		/* gets the collision object
		 * @return collision object
		 */
		const Phoenix::Collision getCollision () const { return col; }

		/* gets the vertice texture vector
		 * @return vertice texture vector
		 */
		const Phoenix::Possition getPossition () const { return pos; }

		/* gets the size of the vertice vector
		 * @return the size of the vertice vector
		 */
		const unsigned getVerticeSize() { return vertices.size(); }

		/* returns if object is static
		 * @return bool value
		 */
		const bool isStatic () const { return pos.staticObj; }

		/* returns if object is empty
		 * @return bool value
		 */
		const bool isEmpty () const { return (vertices.size() == 0) || (faceGroups.size() == 0); }

		/// Loads the textures
		void loadTextures();

		std::vector<int> genIndices();
		std::vector<int> genIndices2();
	private:
		/// vertice vector
		std::vector<float> vertices;
		
		/// vertice normal vector
		std::vector<float> vNormals;

		/// vertice texture vector
		std::vector<float> vTextures;

		/// facegroup vector
		std::vector<Phoenix::FaceGroup> faceGroups;
		
		/// material list vector
		std::vector<Phoenix::Material> mtlList;

		/// collisoin structure
		Phoenix::Collision col;

		/// possition structure
		Phoenix::Possition pos;

		/* gets bool input from the user
		 * @param query - question to ask user
		 * @return bool value - answere from user
		 */
		const bool getBoolInput( const std::string& query ) const;

		/* Request a string from the user
		 * @param requestStr - question to ask user 
		 * @return answere from user
		 */
		const std::string getStr( const std::string& requestStr ) const;
	};
}
#endif