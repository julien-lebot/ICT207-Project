#include <Phoenix\model.hpp>

using namespace Phoenix;

void Model::setPossition ()
{
	if(getBoolInput ( "Will this be a static object?" ) )
	{	
		setStatic ( true );
		pos.x = 0; pos.y = 0; pos.z = 0;
	}
	else
	{
		setStatic ( false );
		pos.x = ( float ) atof ( ( getStr ( "Enter possition on x axis" ) ).c_str () );
		pos.y = ( float ) atof ( ( getStr ( "Enter possition on y axis" ) ).c_str () );
		pos.z = ( float ) atof ( ( getStr ( "Enter possition on z axis" ) ).c_str () );
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
{/*
	for (std::vector<Phoenix::Material>::iterator iter = mtlList.begin(); iter != mtlList.end(); ++iter)
	{
		(*iter).loadTextures();
	}*/
}

std::vector<int> Model::genIndices()
{
	std::vector<int> tempV,tempVn,tempVt, returnVec;

	for(std::vector<FaceGroup>::const_iterator groupIter = faceGroups.begin(); groupIter != faceGroups.end(); ++groupIter)
	{
		for(std::vector<FaceCollection>::const_iterator faceIter = (*groupIter).faces.begin(); faceIter != (*groupIter).faces.end(); ++faceIter )
		{
			tempV.insert(tempV.end(), (*faceIter).v.begin(), (*faceIter).v.end()); 
			//materialOffsets.push_back(indices.size());
			tempVn.insert(tempVn.end(), (*faceIter).vn.begin(), (*faceIter).vn.end()); 
			tempVt.insert(tempVt.end(), (*faceIter).vt.begin(), (*faceIter).vt.end()); 
		}
	}
	
	returnVec.insert(returnVec.end(), tempV.begin(), tempV.end()); 
	returnVec.insert(returnVec.end(), tempVn.begin(), tempVn.end()); 
	returnVec.insert(returnVec.end(), tempVt.begin(), tempVt.end()); 

	return returnVec;
}

std::vector<int> Model::genIndices2()
{
	std::vector<int> tempV,tempVn,tempVt, returnVec;

	for(std::vector<FaceGroup>::const_iterator groupIter = faceGroups.begin(); groupIter != faceGroups.end(); ++groupIter)
	{
		for(std::vector<FaceCollection>::const_iterator faceIter = (*groupIter).faces.begin(); faceIter != (*groupIter).faces.end(); ++faceIter )
		{
			for(std::vector<int>::const_iterator vIter = (*faceIter).v.begin(); vIter != (*faceIter).v.end(); )
			{
				tempV.push_back(*vIter++);
				tempV.push_back(*vIter++);
				tempV.push_back(*vIter++);
			}
			
			for(std::vector<int>::const_iterator vnIter = (*faceIter).vn.begin(); vnIter != (*faceIter).vn.end(); )
			{
				tempVn.push_back(*vnIter++);
				tempVn.push_back(*vnIter++);
				tempVn.push_back(*vnIter++);
			}
			for(std::vector<int>::const_iterator vtIter = (*faceIter).vt.begin(); vtIter != (*faceIter).vt.end(); )
			{
				tempVt.push_back(*vtIter++);
				tempVt.push_back(*vtIter++);
				tempVt.push_back(*vtIter++);
			}
		}
	}
	
	returnVec.insert(returnVec.end(), tempV.begin(), tempV.end()); 
	returnVec.insert(returnVec.end(), tempVn.begin(), tempVn.end()); 
	returnVec.insert(returnVec.end(), tempVt.begin(), tempVt.end()); 

	return returnVec;
}