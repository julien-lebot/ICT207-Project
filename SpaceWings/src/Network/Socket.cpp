#include <Network/Socket.hpp>

using namespace Network;

bool Socket::Open( unsigned short port )
{
	assert( !IsOpen() );

	//create socket
	socket = ::socket( AF_INET, SOCK_DGRAM, IPPROTO_UDP );

	if( socket <= 0 )
	{
		std::cout << "Failed to create socket!";
		socket = 0;
		return false;
	}
	
	// bind to port

	sockaddr_in address;
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons( (unsigned short) port );

	if ( bind( socket, (const sockaddr*) &address, sizeof(sockaddr_in) ) < 0 )
	{
		std::cout << "Failed to bind socket!" << std::endl;
		Close();
		return false;
	}

	// set non-blocking io
	DWORD nonBlocking = 1;
	if ( ioctlsocket( socket, FIONBIO, &nonBlocking ) != 0 )
	{
		std::cout << "failed to set non-blocking socket" << std::endl;
		Close();
		return false;
	}

	return true;
}

void Socket::Close()
{
	if ( socket != 0 )
	{
		closesocket( socket );
		socket = 0;
	}
}

bool Socket::Send ( const Network::Address & destination, const std::string & data)
{
	if ( socket == 0 )
		return false;

	sockaddr_in address;
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = htonl( destination.GetAddress() );
	address.sin_port = htons( (unsigned short) destination.GetPort() );

	int sent_bytes = sendto( socket, &data[0], data.capacity(), 0, (sockaddr*)&address, sizeof(sockaddr_in) );

	return sent_bytes == data.capacity();
}

int Socket::Receive( Network::Address & sender, std::string & data)
{
	data.resize(packetSize);

	if ( socket == 0 )
		return false;
	
	typedef int socklen_t;
	
	sockaddr_in from;
	socklen_t fromLength = sizeof( from );
	
	//unsigned char packet[packetSize];

	int received_bytes = recvfrom( socket, (char*)&data[0], data.capacity(), 0, (sockaddr*)&from, &fromLength );

	if ( received_bytes == 0 )
		return 0;

	unsigned int address = ntohl( from.sin_addr.s_addr );
	unsigned int port = ntohs( from.sin_port );

	sender = Address( address, port );
	
	//data[0] = packet;

	return received_bytes;
}
