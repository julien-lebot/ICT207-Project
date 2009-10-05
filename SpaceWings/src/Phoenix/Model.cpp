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