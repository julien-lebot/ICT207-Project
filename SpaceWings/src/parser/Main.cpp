#include <iostream>
#include <fstream>
#include <string>
#include <Phoenix/Model.hpp>
#include <ObjToSwm/ObjReader.hpp>
#include <ObjToSwm/SwmWriter.hpp>
#include <ObjToSwm/SwmReader.hpp>

using namespace Phoenix;

std::string genOutFileStr( std::string inFileStr )
{
	inFileStr.resize ( inFileStr.size () - 3 );

	std::string returnStr ( inFileStr + "swm" );

	return returnStr;
}

int main( int argc, char *argv[] )
{
	if ( argc == 1 )
	{
		std::cout << "No files in argument array!";
		exit ( 1 );
	}

	Model model;
	ObjReader objR;
	SwmWriter swmW;
	SwmReader swmR;

	for ( int i = 1; i < argc; i++ )
	{
		objR.readFile ( argv[i], model );
		model.setCollision();
		model.setPossition();
		if ( !model.isEmpty () )
			swmW.writeFile ( genOutFileStr ( argv[i] ), model );
	}

	Model model2;
	swmR.readFile ( genOutFileStr ( argv[1] ) , model2 );
}
