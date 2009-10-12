#include <Phoenix/Model.hpp>

using namespace Phoenix;

void Model::setPosition ()
{
	if(getBoolInput ( "Will this be a static object?" ) )
	{	
		setStatic ( true );
		pos.x = 0; pos.y = 0; pos.z = 0;
	}
	else
	{
		setStatic ( false );
		pos.x = ( float ) atof ( ( getStr ( "Enter position on x axis" ) ).c_str () );
		pos.y = ( float ) atof ( ( getStr ( "Enter position on y axis" ) ).c_str () );
		pos.z = ( float ) atof ( ( getStr ( "Enter position on z axis" ) ).c_str () );
	}
}
void Model::setCollision ()
{
	std::string inputStr = getStr ( "Collision detection with aabb/sphere/other?" );
	if ( inputStr == "aabb" )
	{
		col.aabbWidth =  ( float ) atof ( ( getStr ( "Enter aabb width" ) ).c_str () );
		col.aabbHight = ( float ) atof ( ( getStr ( "Enter aabb heigth" ) ).c_str () );
	}
	else if ( inputStr == "sphere" )
		col.radius = ( float ) atof ( ( getStr ( "Enter sphere radius" ) ).c_str () );
	else
		std::cout << "Edit file manualy";
}

const bool Model::getBoolInput ( const std::string& query ) const
{
	std::cout << query << " (y/n)";
	char input = ' ';
	while ( input != 'y' && input != 'n' )
		std::cin >> input;

	if ( input == 'y' )
		return true;
	else
		return false;
}

const std::string Model::getStr ( const std::string& requestStr ) const
{
	std::string returnStr;

	std::cout << requestStr << ": ";
	std::cin >> returnStr;

	return returnStr;
}

void Model::loadTextures()
{
	for (std::vector<Phoenix::Material>::iterator iter = mtlList.begin(); iter != mtlList.end(); ++iter)
	{
		(*iter).loadTextures();
	}
}

void Model::convertObjModel()

{
	std::vector<int> tempV,tempVn,tempVt;
	std::vector<int> returnVec;

	for(std::vector<FaceGroup>::const_iterator groupIter = faceGroups.begin(); groupIter != faceGroups.end(); ++groupIter)
	{
		for(std::vector<FaceCollection>::const_iterator faceIter = (*groupIter).faces.begin(); faceIter != (*groupIter).faces.end(); ++faceIter )
		{
			mtlOffsets.push_back(tempV.size());
			mtlOffsetNames.push_back((*faceIter).mtlName);
			tempV.insert(tempV.end(), (*faceIter).v.begin(), (*faceIter).v.end());
			tempVt.insert(tempVt.end(), (*faceIter).vt.begin(), (*faceIter).vt.end());
			tempVn.insert(tempVn.end(), (*faceIter).vn.begin(), (*faceIter).vn.end());
			mtlOffsets.push_back(tempV.size());
		}
	}

	std::vector<float> tempVertice,tempVTexture, tempVNormals;
	for(unsigned i = 0; i < tempV.size(); i++)
	{
		tempVertice.push_back(vertices[tempV[i] * FLOATS_PER_LINE]);
		tempVertice.push_back(vertices[tempV[i] * FLOATS_PER_LINE + 1]);
		tempVertice.push_back(vertices[tempV[i] * FLOATS_PER_LINE + 2]);
	}

	vertices = tempVertice;

	for(unsigned i = 0; i < tempVn.size(); i++)
	{
		tempVNormals.push_back(vNormals[tempVn[i] * FLOATS_PER_LINE]);
		tempVNormals.push_back(vNormals[tempVn[i] * FLOATS_PER_LINE + 1]);
		tempVNormals.push_back(vNormals[tempVn[i] * FLOATS_PER_LINE + 2]);
	}

	vNormals = tempVNormals;

	for(unsigned i = 0; i < tempVt.size()  ; i++)
	{
		tempVTexture.push_back(vTextures[tempVt[i] * FLOATS_PER_LINE]);
		tempVTexture.push_back(vTextures[tempVt[i] * FLOATS_PER_LINE + 1]);
		tempVTexture.push_back(0);
	}
	
	vTextures = tempVTexture;
}

