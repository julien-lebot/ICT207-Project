#include <Network/Connection.hpp>

using namespace Network;

bool Connection::Start ( int port )
{
	assert( !running );
	std::cout << "start connection on port:" << port << std::endl;
	if ( !socket.Open( port ) )
		return false;
	running = true;
	return true;
}
		
void Connection::Stop()
{
	assert( running );
	std::cout << "stop connection" << std::endl;
	ClearData();
	socket.Close();
	running = false;
}
		
void Connection::Listen()
{
	std::cout << "server listening for connection" << std::endl;
	ClearData();
	mode = Server;
	state = Listening;
}
		
void Connection::Connect( const Address & address )
{
	std::cout << "client connecting to"
			  << address.GetA() << "." 
			  << address.GetB() << "."
			  << address.GetC() << "."
			  << address.GetD() << ":"
			  << address.GetPort();
	ClearData();
	mode = Client;
	state = Connecting;
	this->address = address;
}

void Connection::Update( float deltaTime )
{
	assert( running );
	timeoutAccumulator += deltaTime;
	if ( timeoutAccumulator > timeout )
	{
		if ( state == Connecting )
		{
			std::cout << "connect timed out." << std::endl;
			ClearData();
			state = ConnectFail;
		}
		else if ( state == Connected )
		{
			std::cout << "connection timed out" << std::endl;
			ClearData();
			if ( state == Connecting )
				state = ConnectFail;
		}
	}
}
		
bool Connection::SendPacket( const std::string data)
{
	assert( running );
	if ( address.GetAddress() == 0 )
		return false;
	std::string packet;
	packet.append(protocolId);
	packet.append(data);

	return socket.Send( address, packet );
}

int Connection::ReceivePacket( std::string & data )
{
	assert( running );
	std::string packet;
	Address sender;
	int bytes_read = socket.Receive( sender, packet );
	
	if ( bytes_read == 0 )
		return 0;
	if ( bytes_read <= 4 )
		return 0;
	if ( packet.substr(0,4) != protocolId )
		return 0;
	if ( mode == Server && !IsConnected() )
	{
		std::cout << "server accepts connection from client "
				  << sender.GetA() << "."
				  << sender.GetB() << "."
				  << sender.GetC() << "."
				  << sender.GetD() << ":"
				  << sender.GetPort() << std::endl;
		state = Connected;
		address = sender;
	}
	if ( sender == address )
	{
		if ( mode == Client && state == Connecting )
		{
			std::cout <<  "client completes connection with server" << std::endl;;
			state = Connected;
		}
		timeoutAccumulator = 0.0f;
		data = &packet[4];
		
		return data.capacity();
	}
	return 0;
}