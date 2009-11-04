//client

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
	
	Connection clientConnection(ProtocolId, TimeOut);

	if ( !clientConnection.Start( ClientPort ) )
	{
		std::cout << "could not start connection on port:" << ClientPort <<  std::endl;
		return 1;
	}
	
	clientConnection.Connect( Address(127,0,0,1,ServerPort ) );
	
	bool connected = false;
	
	while ( true )
	{
		if ( !connected && clientConnection.IsConnected() )
		{
			std::cout << "client connected to server!" << std::endl ;
			connected = true;
		}
		
		if ( !connected && clientConnection.ConnectFailed() )
		{
			std::cout << "connection failed" << std::endl;
			break;
		}
		
		Packet packet;
		packet.setData("Client to server");
		packet.setDataType(Packet::ENTITYCREATE);
		
		std::string packetString = packet.toString();

		clientConnection.SendPacket( packetString );
		
		while ( true )
		{
			std::string packetString;
			int bytes_read = clientConnection.ReceivePacket( packetString );
			if ( bytes_read == 0 )
				break;
			std::cout << "received packet from server:" << packetString << std::endl;
			break;
		}
		
		clientConnection.Update( DeltaTime );
		Sleep( DeltaTime );
	}
	
	ShutdownSockets();

	return 0;
}
