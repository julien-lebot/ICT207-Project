#include <ObjToSwm\SwmReader.hpp>

using namespace Phoenix;

void SwmReader::readFile ( std::string fileName, Model& model )
{
	std::ifstream inFile;

	inFile.open ( fileName.c_str (), std::ios::in );

	if( inFile.is_open() )
	{
		std::cout << "Reading " << fileName << ".." << std::endl;
		parseFile ( inFile, model );
	}
	else
		std::cout << fileName << " could not be opened." << std::endl; 
	
	inFile.close ();
}

void SwmReader::parseFile ( std::ifstream& inFile, Model& model )
{
	std::string tag;

	while ( !inFile.eof () )
	{
		setNextTokens ( inFile  );

		tag = m_token.getElement ( 0 );

		if ( tag == "<possition>")
			parsePos ( inFile, model );
		else if ( tag == "<collision>" )
			parseCol ( inFile, model );
		else if ( tag == "<vertice>" )
			parseVertice( inFile, model );
		else if ( tag == "<vtexture>" )
			parseVTexture ( inFile, model );
		else if ( tag == "<vnormal>" )
			parseVNormal ( inFile, model );
		else if ( tag == "<group>" )
			parseGroup ( inFile, model );
		else if ( tag == "<material>" )
			parseMaterial ( inFile, model );
	}
}

void SwmReader::parsePos ( std::ifstream& inFile, Model& model )
{
	Possition tempPos;
	
	setNextTokens ( inFile  );
	if (m_token.getElement ( 1 ) == "true" )
		tempPos.staticObj = true;
	else
		tempPos.staticObj = false;

	setNextTokens ( inFile  );
	tempPos.x = ( float ) atof( m_token.getNext ().c_str () );
	tempPos.y = ( float ) atof( m_token.getNext ().c_str () );
	tempPos.z = ( float ) atof( m_token.getNext ().c_str () );

	model.setPossition ( tempPos );
}

void SwmReader::parseCol ( std::ifstream& inFile, Model& model )
{
	Collision tempCol;

	setNextTokens ( inFile  );

	tempCol.aabbWidth = ( float ) atof(m_token.getElement ( 1 ).c_str () );
	tempCol.aabbHight = ( float ) atof(m_token.getElement ( 2 ).c_str () );

	setNextTokens ( inFile , 0);

	tempCol.radius = ( float ) atof ( m_token.getElement ( 1 ).c_str () );

	model.setCollision(tempCol);
}

void SwmReader::parseVertice( std::ifstream& inFile, Model& model )
{
	setNextTokens( inFile , 0);

	do
	{
		for(int i = 0; i < 3; i++)
			model.addVertice ( ( float ) atof ( m_token.getNext ().c_str () ) );
		setNextTokens( inFile , 0);
	}while ( !isEndTag() );
}

void SwmReader::parseVTexture ( std::ifstream& inFile, Model& model )
{
	setNextTokens(inFile , 0);

	do
	{
		for(int i = 0; i < 3; i++)
			model.addVTexture ( ( float ) atof ( m_token.getNext ().c_str () ) );
		setNextTokens ( inFile  );
	}while ( !isEndTag() );
}

void SwmReader::parseVNormal ( std::ifstream& inFile, Model& model )
{
	setNextTokens(inFile , 0);
	
	do
	{
		for(int i = 0; i < 3; i++)
			model.addvNormals ( ( float ) atof ( m_token.getNext ().c_str () ) );
	
		setNextTokens(inFile , 0);
	}while ( !isEndTag() );
}

void SwmReader::parseGroup ( std::ifstream& inFile, Model& model )
{
	FaceGroup tempFG;
	
	setNextTokens( inFile , 0);
	tempFG.groupName = m_token.getNext();

	parseFace( inFile, tempFG );

	model.addFaceGroup(tempFG);
}

void SwmReader::parseFace ( std::ifstream& inFile, FaceGroup& fg )
{
	setNextTokens(inFile , 1);

	do
	{
		FaceCollection tempFC;
		tempFC.mtlName = m_token.getNext ();
		setNextTokens ( inFile ,0 );
		for ( unsigned i = 0; i < m_token.size(); i++ )
			tempFC.v.push_back( atoi( m_token.getNext ().c_str () ) );
		setNextTokens ( inFile ,0 );
		for ( unsigned i = 0; i < m_token.size(); i++ )
			tempFC.vn.push_back( atoi( m_token.getNext ().c_str () ) );
		setNextTokens ( inFile ,0 );
		for ( unsigned i = 0; i < m_token.size(); i++ )
			tempFC.vt.push_back( atoi( m_token.getNext ().c_str () ) );
		setNextTokens ( inFile , 1 );
		fg.faces.push_back(tempFC);
		if ( m_token.getNext () == "<face>" )		
			setNextTokens ( inFile  );
	}while ( (m_token.getElement ( 0 ) != "</group>") );
}

void SwmReader::parseMaterial ( std::ifstream& inFile, Model& model )
{
	Phoenix::Material tempMtl;
	Phoenix::Color tempColor;
	
	setNextTokens ( inFile , 0);
	tempMtl.setName( m_token.getNext () );
	
	setNextTokens ( inFile  );
	tempColor.r = ( float ) atof ( ( ( m_token.getElement (1) ).c_str () ) );
	tempColor.g = ( float ) atof ( ( ( m_token.getElement (2) ).c_str () ) );
	tempColor.b = ( float ) atof ( ( ( m_token.getElement (3) ).c_str () ) );
	tempMtl.setMaterialAmbient(tempColor);
	
	setNextTokens ( inFile  );
	tempColor.r = ( float ) atof ( ( ( m_token.getElement (1) ).c_str () ) );
	tempColor.g = ( float ) atof ( ( ( m_token.getElement (2) ).c_str () ) );
	tempColor.b = ( float ) atof ( ( ( m_token.getElement (3) ).c_str () ) );
	tempMtl.setMaterialDiffuse(tempColor);

	setNextTokens ( inFile );
	tempColor.r = ( float )atof ( ( ( m_token.getElement (1) ).c_str () ) );
	tempColor.g = ( float )atof ( ( ( m_token.getElement (2) ).c_str () ) );
	tempColor.b = ( float )atof ( ( ( m_token.getElement (3) ).c_str () ) );
	tempMtl.setMaterialSpecular(tempColor);

	setNextTokens ( inFile );
	tempMtl.setTransparency ( ( float ) atof ( ( ( m_token.getElement ( 1 ) ).c_str () ) ) );

	setNextTokens ( inFile );
	tempMtl.setShininess ( ( float ) atof ( ( ( m_token.getElement ( 1 ) ).c_str () ) ) );

	setNextTokens ( inFile );
	tempMtl.setIllum( atoi ( ( ( m_token.getElement( 1 ) ).c_str() ) ) );

	setNextTokens ( inFile );
	tempMtl.setAmbientMap ( m_token.getElement ( 1 ) );

	setNextTokens ( inFile );
	tempMtl.setDiffuseMap ( m_token.getElement ( 1 ) );

	setNextTokens ( inFile );
	tempMtl.setSpecularMap ( m_token.getElement ( 1 ) ); 

	setNextTokens ( inFile );
	tempMtl.setBumpMap ( m_token.getElement ( 1 ) );

	model.addMaterial(tempMtl);
}

void SwmReader::setNextTokens( std::ifstream& inFile, unsigned skipLine)
{
	for(unsigned i = 0; i < skipLine + 1; i++)
	{
		std::getline ( inFile, m_line );
		m_token.tokenizeStr ( m_line, " " );
	}
}