//server

#include <iostream>
#include <string>
#include <vector>

#include <Network/Connection.hpp>
#include <Network/Packet.hpp>

const int ServerPort = 30000;
const int ClientPort = 30001;
const std::string ProtocolId = "SpWi";
const float DeltaTime = 0.25f;
const float SendRate = 0.25f;
const float TimeOut = 1000.0f;

using namespace Network;

int main( int argc, char * argv[] )
{
	if ( !InitializeSockets() )
	{
		std::cout << "failed to initialize socket" << std::endl;
		return 1;
	}

	Connection serverConnection(ProtocolId, TimeOut);

	if ( !serverConnection.Start( ServerPort ) )
	{
		std::cout << "could not start connection on port" << ServerPort << std::endl;
		return 1;
	}

	serverConnection.Listen();
	
	while ( true )
	{
		if ( serverConnection.IsConnected() )
		{
			Packet packet;
			packet.setData("Server to client!");
			packet.setDataType(Packet::ENTITYCREATE);

			std::string packetString = packet.toString();

			serverConnection.SendPacket( packetString );
		}
		
		while ( true )
		{
			std::string packetString;
			int bytes_read = serverConnection.ReceivePacket( packetString );
			if ( bytes_read == 0 )
				break;
			std::cout << "received packet from client:" << packetString << std::endl;
			break;
		}
		
		serverConnection.Update( DeltaTime );
		Sleep( DeltaTime );
	}

	ShutdownSockets();

	return 0;
}
