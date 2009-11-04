/*
*	Socket.hpp
*
*	Socket class. Based on Gaffers articles about networking (http://gafferongames.com)
*
*	Created by Terje Loe
*/

#ifndef SOCKET_HPP
#define SOCKET_HPP

#include <Network/Address.hpp>
#include <Network/Packet.hpp>
#include <winsock2.h>
#include <assert.h>
#include <iostream>
#include <string>

#pragma comment( lib, "wsock32.lib" )

namespace Network
{
	class Socket
	{
	public:
		Socket () { socket = 0; };
		~Socket () { Close(); }
		bool Open( unsigned short port ) ;
		void Close();
		bool IsOpen() const { return socket != 0; }
		bool Send( const Network::Address & destination, const std::string & data );
		int Receive(Network::Address & sender, std::string & data );

	private:
		int socket;
	};

	inline bool InitializeSockets()
	{
	    WSADATA wsaData;
		return WSAStartup( MAKEWORD(2,2), &wsaData ) == NO_ERROR;
	}

	inline void ShutdownSockets()
	{
		WSACleanup();
	}


}

#endif