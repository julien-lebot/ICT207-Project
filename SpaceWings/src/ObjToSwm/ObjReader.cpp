#include <ObjToSwm/ObjReader.hpp>

using namespace Phoenix;

void ObjReader::readFile( const std::string& fileName, Model& model )
{
	std::ifstream inFile;
	
	inFile.open ( fileName.c_str(), std::ios::in );
	
	if ( inFile.is_open() )
	{
		std::cout << "Reading " << fileName << ".." << std::endl;
		readLines ( inFile, model );
	}
	else
		std::cout << fileName << " could not be opened." << std::endl; 
	
	inFile.close ();
}

void ObjReader::readLines( std::ifstream& inFile, Model& model )
{
	std::string line, prefix;
	Token t;
	
	while ( !inFile.eof () )
	{
		std::getline ( inFile, line );
		t.tokenizeStr ( line, " " );
	
		prefix = t.getNext ();

		if ( prefix == "v" )
			while( !t.allFetched () )
				model.addVertice ( ( float ) atof ( ( t.getNext () ).c_str () ) );
		else if ( prefix == "vt" )
			while( !t.allFetched () )
				model.addVTexture( ( float )atof( ( t.getNext () ).c_str() ) );
		else if (prefix == "vn" )
			while( !t.allFetched () )
				model.addvNormals ( ( float ) atof ( ( t.getNext () ).c_str () ) );
		else if ( prefix == "g" )
		{	
			if ( m_activeGroup )
			{
				addFaceCol( (*m_activeCollection) );
				delete m_activeCollection;
				m_activeCollection = new FaceCollection;
				model.addFaceGroup(*m_activeGroup);
				delete m_activeGroup;
			}
			m_activeGroup = new FaceGroup;
			( *m_activeGroup ).groupName = t.getNext ();
		}
		else if ( prefix == "mtllib" )
		{
			inFileIsMtl = true;
			readFile( t.getNext (), model );
			inFileIsMtl = false;
		}
		else if ( prefix == "newmtl" )
			model.addMaterial ( readNewMaterial ( inFile, t ) );
		else if ( prefix == "usemtl" )
		{
			if ( m_activeCollection )
			{
				if ( ( *m_activeCollection ).mtlName != "" )
				{	
					addFaceCol( (*m_activeCollection) );
					delete m_activeCollection;
				}
			}
			m_activeCollection = new FaceCollection;
			( *m_activeCollection ).mtlName = t.getNext ();
		}
		else if ( prefix == "f" )
			readFaceLine(t);
	}

	if(!inFileIsMtl)
	{
		addFaceCol( (*m_activeCollection) );
		delete m_activeCollection;
		model.addFaceGroup(*m_activeGroup);
		delete m_activeGroup;
	}
}

void ObjReader::readFaceLine ( Token& t ) const
{
	Token t2;
	std::vector<int> tempV, tempVn, tempVt;

	while( !t.allFetched () )
	{
		t2.tokenizeStr( t.getNext (), "/" );
		while( !t2.allFetched () )
		{
			tempV.push_back ( ( atoi ( t2.getNext ().c_str () ) ) - 1 );
			tempVn.push_back ( ( atoi ( t2.getNext ().c_str () ) ) - 1 );
			tempVt.push_back ( ( atoi ( t2.getNext ().c_str() ) ) - 1 );
		}
	}
	
	//if quad seperate to triangles
	if ( tempV.size () == 3 )
	{
		( *m_activeCollection ).v.insert ( ( *m_activeCollection ).v.end (),tempV.begin (),tempV.end () );
		( *m_activeCollection ).vn.insert ( ( *m_activeCollection ).vn.end (),tempVn.begin (),tempVn.end () );
		( *m_activeCollection ).vt.insert ( ( *m_activeCollection ).vt.end (),tempVt.begin (),tempVt.end () );
	}
	else if ( tempV.size () == 4 )
	{
		std::vector<int> trianglesFromQuadV, trianglesFromQuadVn,trianglesFromQuadVt;
	
		trianglesFromQuadV = quadToTriangle ( tempV );
		trianglesFromQuadVn = quadToTriangle ( tempVn );
		trianglesFromQuadVt = quadToTriangle ( tempVt );

		( *m_activeCollection ).v.insert ( ( *m_activeCollection ).v.end (), trianglesFromQuadV.begin (), trianglesFromQuadV.end () );
		( *m_activeCollection ).vn.insert ( ( *m_activeCollection ).vn.end (), trianglesFromQuadVn.begin (), trianglesFromQuadVn.end () );
		( *m_activeCollection ).vt.insert ( ( *m_activeCollection ).vt.end (), trianglesFromQuadVt.begin (), trianglesFromQuadVt.end () );
	}
}

void ObjReader::addFaceCol(FaceCollection& fc) const
{
	std::vector<FaceCollection>::iterator faceLstIter = (*m_activeGroup).faces.begin();
	bool found = false;

	while ( faceLstIter != (*m_activeGroup).faces.end () && !found )
	{
		if ( (*faceLstIter).mtlName == fc.mtlName )
		{
			found = true;
			( *faceLstIter ).v.insert ( ( *faceLstIter ).v.end (), fc.v.begin (), fc.v.end() );
			( *faceLstIter ).vn.insert ( ( *faceLstIter ).vn.end (),fc.vn.begin (), fc.vn.end () );
			( *faceLstIter ).vt.insert ( ( *faceLstIter ).vt.end (),fc.vt.begin (), fc.vt.end () );
		}
		faceLstIter++;
	}
	
	if (found == false)
		(*m_activeGroup).faces.push_back(fc);
}

std::vector<int> ObjReader::quadToTriangle( std::vector<int> quad ) const
{
	std::vector<int> returnVec;

	returnVec.push_back( quad[0] );
	returnVec.push_back( quad[1] );
	returnVec.push_back( quad[3] );
	returnVec.push_back( quad[1] );
	returnVec.push_back( quad[2] );
	returnVec.push_back( quad[3] );

	return returnVec;
}

Phoenix::Material ObjReader::readNewMaterial ( std::ifstream& inFile, Token& t ) const
{
	Phoenix::Material tempMtl;
	Phoenix::Color tempColor;
	std::string line,prefix;

	tempMtl.setName ( t.getNext () );
	
	do
	{
		std::getline ( inFile, line );
		t.tokenizeStr ( line, " " );
		prefix = t.getNext ();

		if ( prefix == "Ka" )
		{
			tempColor.r = ( float )atof ( ( ( t.getNext () ).c_str () ) );
			tempColor.g = ( float ) atof ( ( (t.getNext () ).c_str () ) );
			tempColor.b = ( float )atof ( ( ( t.getNext () ).c_str () ) );
			tempMtl.setMaterialAmbient(tempColor);
		}
		else if ( prefix == "Kd" )
		{
			tempColor.r = ( float ) atof ( ( ( t.getNext () ).c_str () ) );
			tempColor.g = ( float ) atof ( ( ( t.getNext () ).c_str () ) );
			tempColor.b = ( float ) atof ( ( ( t.getNext () ).c_str () ) );
			tempMtl.setMaterialDiffuse(tempColor);
		}
		else if ( prefix == "Ks" )
		{
			tempColor.r = ( float ) atof ( ( ( t.getNext () ).c_str () ) );
			tempColor.g = ( float ) atof ( ( ( t.getNext () ).c_str () ) );
			tempColor.b = ( float ) atof ( ( ( t.getNext () ).c_str () ) );
			tempMtl.setMaterialSpecular(tempColor);
		}		
		else if ( prefix == "d" || prefix == "Tr" )
			tempMtl.setTransparency( ( float ) atof ( ( ( t.getNext () ).c_str () ) ) );
		else if ( prefix == "Ns" )
			tempMtl.setShininess( ( float ) atof ( ( ( t.getNext () ).c_str () ) ) );
		else if ( prefix == "illum" )
			tempMtl.setIllum ( atoi ( ( ( t.getNext() ).c_str() ) ) );
		else if ( prefix == "map_Ka" )
			tempMtl.setAmbientMap ( t.getNext () );
		else if ( prefix == "map_Kd" )
			tempMtl.setDiffuseMap ( t.getNext () );
		else if ( prefix == "map_Ks" )
			tempMtl.setSpecularMap ( t.getNext() );
		else if ( prefix == "map_bump" )
			tempMtl.setBumpMap ( t.getNext () );
	}while ( !inFile.eof () && prefix != "" );

	return tempMtl;
}