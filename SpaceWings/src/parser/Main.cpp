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

	SwmReader swmR;
	std::cout << argc;
	for ( int i = 1; i < argc; i++ )
	{
		ObjReader objR;
		SwmWriter swmW;

		objR.readFile ( argv[i], model );
		//model.setCollision();
		//model.setposition();
		
		//Convert model to non indexed model before saving it.
		model.convertObjModel();
		if ( !model.isEmpty () )
			swmW.writeFile ( genOutFileStr ( argv[i] ), model );
	}
}
