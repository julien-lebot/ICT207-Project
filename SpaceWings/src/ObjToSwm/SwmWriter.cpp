#include <ObjToSwm\swmWriter.hpp>

using namespace Phoenix;

void SwmWriter::writeFile ( const std::string& fileName, const Model& model )
{
	outFile.open ( fileName.c_str (), std::ios::out );

	std::cout << "Writing to " << fileName << std::endl;

	writePossition ( model.getPossition () );
	writeCollision ( model.getCollision() );
	writeVertice ( model.getVerticeVec () );
	writeVTexture ( model.getVTextureVec () );
	writevNormal ( model.getVNormalVec () );
	writeFaceGroup ( model.getFaceGroupVec () );
	writeMaterial ( model.getMtlListVec () );

	outFile.close ();
}

void SwmWriter::writeFloatVec ( const std::vector<float>& vec, const unsigned lineCount , const unsigned tabN )
{
	for ( std::vector<float>::const_iterator iter = vec.begin (); iter != vec.end ();)
	{
		outFile << tab ( tabN );
		for ( unsigned i = 0; i < lineCount; i++ )
			outFile << ( *iter++ ) << " ";
		outFile << std::endl;
	}
}

void SwmWriter::writeIntVec ( const std::vector<int>& vec, const unsigned tabN )
{
	outFile << tab ( tabN );
	for ( std::vector<int>::const_iterator iter = vec.begin (); iter != vec.end ();)
			outFile << (* iter++ ) << " ";
}

void SwmWriter::writePossition ( const Possition pos )
{
	outFile << startTag( "possition" ) << std::endl;

	outFile << tab ( 1 ) << "static ";
	if ( pos.staticObj )
		outFile << "true" << std::endl;
	else
		outFile << "false" << std::endl;
	outFile << tab ( 1 ) << pos.x << " " << pos.y << " " << pos.z << std::endl;
	
	outFile << endTag( "possition" ) << std::endl;
}

void SwmWriter::writeCollision( const Collision col )
{
	outFile << startTag( "collision" ) << std::endl;
	outFile << tab( 1 ) << "aabb " << col.aabbWidth << " " << col.aabbHight << std::endl;
	outFile << tab( 1 ) << "radius " << col.radius << std::endl; 
	outFile << endTag ( "collision" ) << std::endl;
}

void SwmWriter::writeVertice ( const std::vector<float>& verticeVec )
{
	outFile << startTag( "vertice" ) << std::endl;
	writeFloatVec( verticeVec , 3 , 1 );
	outFile << endTag( "vertice" ) << std::endl;
}

void SwmWriter::writeVTexture ( const std::vector<float>& vTextureVec )
{
	outFile << startTag( "vtexture" ) << std::endl;
	writeFloatVec( vTextureVec,3 , 1  );
	outFile << endTag( "vtexture" ) << std::endl;
}

void SwmWriter::writevNormal ( const std::vector<float>& vNormalVec )
{
	outFile << startTag( "vnormal" ) << std::endl;
	writeFloatVec( vNormalVec, 3 , 1);
	outFile << endTag( "vnormal" ) << std::endl;
}

void SwmWriter::writeFaces ( const std::vector<FaceCollection>& faceVec )
{
	for ( std::vector<FaceCollection>::const_iterator faceColIter = faceVec.begin (); faceColIter != faceVec.end (); faceColIter++ ){
		outFile << tab ( 1 ) << startTag ( "face" ) << std::endl;
		outFile << tab ( 2 ) << ( *faceColIter ).mtlName << std::endl;
		writeIntVec ( ( *faceColIter ).v, 2 );
		outFile << std::endl;
		writeIntVec ( ( *faceColIter ).vn, 2 );
		outFile << std::endl;
		writeIntVec ( ( *faceColIter ).vt, 2 );
		outFile << std::endl;
		outFile << tab ( 1 ) << endTag( "face" ) << std::endl;
	}
}

void SwmWriter::writeFaceGroup ( const std::vector<FaceGroup>& groupVec )
{
	for ( std::vector<FaceGroup>::const_iterator groupIter = groupVec.begin (); groupIter != groupVec.end (); groupIter++ )
	{
		outFile << startTag( "group" ) << std::endl;
		outFile << tab(1) << ( *groupIter ).groupName << std::endl;	
		writeFaces ( ( *groupIter ).faces );
		outFile << endTag( "group" ) << std::endl;
	}
}

void SwmWriter::writeMaterial ( const std::vector<Phoenix::Material>& mtlVec )
{
	for( std::vector<Phoenix::Material>::const_iterator mtlIter = mtlVec.begin (); mtlIter != mtlVec.end (); mtlIter++ )
	{
		outFile << startTag( "material" ) << std::endl;
		outFile << tab ( 1 ) << ( *mtlIter ).getName () << std::endl;
		outFile << tab ( 1 ) << "mtlAmbient " << ( ( *mtlIter ).getMaterialAmbient () ).b << " " << (( *mtlIter ).getMaterialAmbient () ).g << " " << ( ( *mtlIter ).getMaterialAmbient () ).b << std::endl;
		outFile << tab ( 1 ) << "mtlDiffuse " << ( ( *mtlIter ).getMaterialDiffuse () ).r << " "<< ( ( *mtlIter ).getMaterialDiffuse () ).g << " " << ( ( *mtlIter ).getMaterialDiffuse () ).b << std::endl;
		outFile << tab ( 1 ) << "mtlSpecular " << ( ( *mtlIter ).getMaterialSpecular () ).r << " "<< ( ( *mtlIter ).getMaterialSpecular () ).g << " " << ( ( *mtlIter ).getMaterialSpecular () ).b << std::endl;
		outFile << tab ( 1 ) << "transparency " << ( *mtlIter ).getTransparency () << std::endl;
		outFile << tab ( 1 ) << "shininess " << ( *mtlIter ).getShininess () << std::endl;
		outFile << tab ( 1 ) << "illumination " << ( *mtlIter ).getIllum () << std::endl;
		outFile << tab ( 1 ) << "ambientMap " << ( *mtlIter ).getAmbientMap () << std::endl;
		outFile << tab ( 1 ) << "diffuseMap " << ( *mtlIter ).getDiffuseMap () << std::endl;
		outFile << tab ( 1 ) << "specularMap " << ( *mtlIter ).getAmbientMap () << std::endl;
		outFile << tab ( 1 ) << "bumpMap " << ( *mtlIter ).getAmbientMap () << std::endl;
		outFile << endTag( "material" ) << std::endl;
	}
}

const std::string SwmWriter::tab ( const unsigned n ) const
{
	std::string returnStr;
	
	for( unsigned i = 0; i < n; i++ )
		returnStr += "        ";
	
	return returnStr;
};