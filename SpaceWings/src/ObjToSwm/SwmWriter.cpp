#include <ObjToSwm\swmWriter.hpp>

using namespace Phoenix;

void SwmWriter::writeFile ( const std::string& fileName, const Model& model )
{
	outFile.open ( fileName.c_str (), std::ios::out );

	std::cout << "Writing to " << fileName << std::endl;

	writePosition ( model.getPosition () );
	writeCollision ( model.getCollision() );
	writeVertice ( model.getVerticeVec () );
	writeVTexture ( model.getVTextureVec () );
	writevNormal ( model.getVNormalVec () );
	writeMaterialOffsets( model.getMtlOffsets(), model.getMtlOffsetsNames());
	writeMaterial ( model.getMtlListVec () );

	outFile.close ();
}

void SwmWriter::writeFloatVec ( const std::vector<float>& vec, const unsigned numPerLine , const unsigned tabN )
{
	if ( vec.size() % numPerLine != 0 )
		std::cout << "Warning! Uneven float vector!"; 
	unsigned count = 0;
	std::vector<float>::const_iterator iter = vec.begin ();

	outFile << tab ( tabN );
	while ( iter != vec.end() )
	{
		if ( count == numPerLine )
		{
			outFile << std::endl << tab ( tabN );
			count = 0;
		}
		else if ( count != 0 )
			outFile << " ";

		outFile << ( *iter++ ) << " ";
		count++;
	}
	outFile << std::endl;
}

void SwmWriter::writePosition ( const Position pos )
{
	outFile << startTag( "position" ) << std::endl;

	outFile << tab ( 1 ) << "static ";
	if ( pos.staticObj )
		outFile << "true" << std::endl;
	else
		outFile << "false" << std::endl;
	outFile << tab ( 1 ) << pos.x << " " << pos.y << " " << pos.z << std::endl;
	
	outFile << endTag( "position" ) << std::endl;
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
	writeFloatVec( vTextureVec, 3 , 1  );
	outFile << endTag( "vtexture" ) << std::endl;
}

void SwmWriter::writevNormal ( const std::vector<float>& vNormalVec )
{
	outFile << startTag( "vnormal" ) << std::endl;
	writeFloatVec( vNormalVec, 3 , 1);
	outFile << endTag( "vnormal" ) << std::endl;
}

void SwmWriter::writeMaterialOffsets ( const std::vector<int> offSets, const std::vector<std::string> names )
{
	outFile << startTag( "offset" ) << std::endl;
	unsigned i = 0;
	std::vector<std::string>::const_iterator iter = names.begin ();
	while( i < offSets.size())
	{
		outFile << tab ( 1 ) << offSets[i++] << " ";
		outFile << offSets[i++];
		outFile << " " << (*iter++) << std::endl;
	}
	outFile << endTag( "offset" ) << std::endl;
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
		outFile << tab ( 1 ) << "specularMap " << ( *mtlIter ).getSpecularMap () << std::endl;
		outFile << tab ( 1 ) << "bumpMap " << ( *mtlIter ).getBumpMap () << std::endl;
		outFile << endTag( "material" ) << std::endl;
	}
}

const std::string SwmWriter::tab ( const unsigned n ) const
{
	std::string returnStr;
	
	for( unsigned i = 0; i < n; i++ )
		returnStr += "    ";
	
	return returnStr;
};