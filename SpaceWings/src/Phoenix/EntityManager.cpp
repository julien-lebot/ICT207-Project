#include <Phoenix\EntityManager.hpp>

using namespace Phoenix;

void EntityManager::readFile (const std::string& fileName)
{
	std::ifstream inFile;
	std::string line;
	std::vector <int> parent;
	int depth;

	inFile.open (fileName.c_str(), std::ios::in);

	if (inFile.is_open ()) {
		std::cout << "Reading " << fileName << "..." << std::endl;

		while (!inFile.eof ())
		{
			std::getline (inFile, line);
			for (depth = 0; line [depth] == '\t'; depth++);
			if ((int) parent.size () == depth)
				parent.resize (parent.size () + 1);
			
			if (line [0] != '#')
			{
				parent [depth] = entityList.size ();
				createEntity (line, true, depth != 0 ? parent [depth - 1] : -1);
			}
		}
	}
	else
		std::cout << "ERROR: " << fileName << " could not be opened." << std::endl;

	inFile.close ();
}

void EntityManager::createEntity (std::string line, bool isFile = false, int parent = 0)
{
	EntityListElement newEntity;
	
	int index;
	char * temp;
	std::string token = strtok_s ((char *) line.c_str ()," \t<>", &temp);

	if (isFile)
	{
		std::string entityTypes [NUMBER] = {"STAR", "PLANET", "MOON", "ASTEROIDBELT", "ASTEROID", "STARBASE", "SPACESHIP"};
		for (index = 0; index < NUMBER && entityTypes [index].compare (token); index++);
		newEntity.entityType = (EntityType) (1 << index);
	}
	else
		newEntity.entityType = (EntityType) atoi (token.c_str ());

	newEntity.name = strtok_s (NULL, " \t<>", &temp);
	newEntity.size = (float) atof (strtok_s (NULL, " \t<>", &temp));

	if (isFile)
	{
		newEntity.orbit = (float) atof (strtok_s (NULL, " \t<>", &temp));
		newEntity.year = (float) atof (strtok_s (NULL, " \t<>", &temp));
		newEntity.day = (float) atof (strtok_s (NULL, " \t<>", &temp));
		newEntity.axis = (float) atof (strtok_s (NULL, " \t<>", &temp)) * Math::PI / 180.0;
		newEntity.state.mass = (float) atof (strtok_s (NULL, " \t<>", &temp));
		newEntity.state.inverseMass = (newEntity.state.mass ? 1.0 / newEntity.state.mass : 0);

		newEntity.parentIndex = parent;
	}
	else
	{
		index = atoi (strtok_s (NULL, " \t<>", &temp));
		if ((int) entityList.size () - 1 < index)
			entityList.resize (index + 1);
		newEntity.parentIndex = atoi (strtok_s (NULL, " \t<>", &temp));
	}

	if (newEntity.entityType & SPACESHIP)
		newEntity.gameEntity = new PlayerEntity ();
	else if (newEntity.entityType & (STAR | PLANET | MOON | ASTEROIDBELT | ASTEROID | STARBASE))
		newEntity.gameEntity = new OrbitalEntity ();
	
	token = strtok_s (NULL, " \t<>", &temp);
	newEntity.gameEntity->setModelFile (token);
	
	newEntity.gameEntity->load (swmR);

	if (isFile)
		entityList.push_back (newEntity);
	else
		entityList [index] = newEntity;
}

void EntityManager::printEntities ()
{
	int index;
	std::string entityTypes [NUMBER] = {"STAR", "PLANET", "MOON", "BELT", "ASTEROID", "STARBASE", "SPACESHIP"};

	for (int i = 0; i < (int) entityList.size (); i++)
	{
		for (index = 0; index < NUMBER && (1 << index) != (int) entityList [i].entityType; index++);
		std::cout << "NAME:\t" << entityList [i].name << std::endl;
		std::cout << "INDEX:\t" << i << std::endl;
		std::cout << "PARENT:\t" << entityList [i].parentIndex << std::endl;
		std::cout << "TYPE:\t" << entityTypes [index] << std::endl;
		std::cout << "SIZE:\t" << entityList [i].size << std::endl;
		std::cout << "ORBIT:\t" << entityList [i].orbit << std::endl;
		std::cout << "YEAR:\t" << entityList [i].year << std::endl;
		std::cout << "DAY:\t" << entityList [i].day << std::endl;
		std::cout << "AXIS:\t" << entityList [i].axis << std::endl << std::endl;
	}
}

void EntityManager::createConstructList( std::vector<Network::Packet> & list )
{
	if(!levelSent)
	{
		Network::Packet packet;
		std::ostringstream ss;
		packet.setDataType (Network::Packet::ENTITYCREATE);
		for(std::vector<EntityListElement>::iterator iter = entityList.begin(); iter != entityList.end(); ++iter)
		{
			ss.str ("");
			ss << (*iter).entityType << " " << (*iter).name << " " << (*iter).size << " " << distance (entityList.begin(), iter)  << " " << (*iter).parentIndex << " " << (*iter).gameEntity->getModelFile ();;
			packet.setData (ss.str ());
			list.push_back(packet);
		}
		levelSent = true;
	}
}

void EntityManager::renderAll()
{
	for(std::vector<EntityListElement>::iterator iter = entityList.begin(); iter != entityList.end(); ++iter)
	{
		if((*iter).gameEntity->isLoaded())
		{
			Math::Vector3f axis, pos;
			float angle = 0;
			(*iter).state.orientation.angleAxis (angle, axis);
			axis.normalize ();
			glPushMatrix ();
				getPosition (*iter, pos);
				glTranslatef(pos.x, pos.y, pos.z);
				glRotatef ((angle + Math::PI) * 180 / Math::PI, axis.x, axis.y, axis.z);
				glScalef((*iter).size, (*iter).size, (*iter).size);
				Renderer::instance().render((*iter).gameEntity->getRenderOperation());
			glPopMatrix ();
		}
	}
}