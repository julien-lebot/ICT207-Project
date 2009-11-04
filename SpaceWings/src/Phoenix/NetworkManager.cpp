#include <Phoenix\NetworkManager.hpp>

using namespace Network;
using namespace Phoenix;

bool NetworkManager::setup()
{
	if ( !InitializeSockets() )
	{
		std::cout << "ERROR: Failed to initialize socket." << std::endl;
		return false;
	}
		
	std::cout << "Do you want to connect to another player?(y/n)" << std::endl;
	
	char input = ' ';
	while ( input != 'y' && input != 'n' )
		std::cin >> input;
	
	connection = new Connection(ProtocolId, TimeOut);
	
	if(input == 'y')
	{
		if ( !(*connection).Start( ClientPort ) )
		{
			std::cout << "ERROR: could not start connection on port: " << ClientPort <<  std::endl;
			return false;
		}
		
		std::string addressInput;
		std::cout << "Please enter ip to server (eg. 192.168.0.1), 'l' for 127.0.0.1" << std::endl;
		std::cin >> addressInput;
		if(addressInput == "l")
		{
			(*connection).Connect( Address(	127, 0, 0, 1, ServerPort ) );
		}
		else
		{
			Token token;
			token.tokenizeStr(addressInput,".");

			(*connection).Connect( Address(	( unsigned )( atoi( token.getElement(0).c_str() ) )
										, ( unsigned )( atoi( token.getElement(1).c_str() ) )
										, ( unsigned )( atoi( token.getElement(2).c_str() ) ) 
										, ( unsigned )( atoi( token.getElement(3).c_str() ) ) 
										, ServerPort ) );
		}
		
		bool connected = false;
		
		if((*connection).ConnectFailed())
		{
			std::cout << "connection failed" << std::endl;
			return false;
		}
	}
	else
	{
		if ( !(*connection).Start( ServerPort ) )
		{
			std::cout << "could not start connection on port %d\n", ServerPort;
			return false;;
		}

		(*connection).Listen();
	}
	return true;
}	

void NetworkManager::send_recv()
{
	std::string packetString;
	Packet packet;
	
	if(sendPacketList.empty())
		(*connection).SendPacket( "KEEPALIVE" );
	
	while(!sendPacketList.empty())
	{
		(*connection).SendPacket( sendPacketList.back().toString() );
		sendPacketList.pop_back();
	}
	
	int bytes_received = -1;
	do
	{
		bytes_received = (*connection).ReceivePacket( packetString );
		if (bytes_received > 0)
		{
			if (packetString == "KEEPALIVE")
				std::cout << "Keep alive received!" << std::endl;
			else
			{
				packet.fromString(packetString);
				recvPacketList.push_back(packet);
			}
		}
	} while( bytes_received != 0);
	//(*connection).Update( DeltaTime );
}

void NetworkManager::processIncoming(Phoenix::EntityManager & em)
{
	for(std::vector<Network::Packet>::iterator iter = recvPacketList.begin(); iter != recvPacketList.end(); ++iter)
	{
		switch((*iter).getDataType())
		{
			case (Network::Packet::ENTITYCREATE):
				em.createEntity ((*iter).getData());
				break;
			case (Network::Packet::ENTITYDESTROY):
				break;	
			case (Network::Packet::ENTITYSTATE):
				break;
			case (Network::Packet::INPUT):
				break;
		}
	}
}

void NetworkManager::sendinput(Input inp)
{
	



}