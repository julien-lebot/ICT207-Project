/*
 *	Connection.hpp
 *
 *	Connection class. Based on Gaffers articles about networking (http://gafferongames.com)
 *
 *	Created by Terje Loe
 */

#ifndef CONNECTION_HPP
#define CONNECTION_HPP

#include <string>
#include <iostream>
#include <Network/Socket.hpp>
#include <Network/Address.hpp>

namespace Network
{
	class Connection
	{
	public:
		
		enum Mode
		{
			None,	
			Client,
			Server
		};
		
		Connection( std::string protocolId, float timeout )
		{
			this->protocolId = protocolId;
			this->timeout = timeout;
			mode = None;
			running = false;
			ClearData();
		}
		
		~Connection()
		{
			if ( running )
				Stop();
		}
		
		bool Start( int port );
		void Stop();
		void Listen();
		void Connect( const Address & address );
		bool IsConnecting() const {	return state == Connecting;	}
		bool ConnectFailed() const { return state == ConnectFail; }
		bool IsConnected() const { return state == Connected; }		
		bool IsListening() const { return state == Listening; }
		Mode GetMode() const { return mode;	}
		void Update( float deltaTime );	
		bool SendPacket( const std::string data );
		int ReceivePacket( std::string & data );
	protected:
		void ClearData()
		{
			state = Disconnected;
			timeoutAccumulator = 0.0f;
			address = Address();
		}
	private:
		enum State
		{
			Disconnected,
			Listening,
			Connecting,
			ConnectFail,
			Connected
		};

		std::string protocolId;
		float timeout;
		
		bool running;
		Mode mode;
		State state;
		Network::Socket socket;
		float timeoutAccumulator;
		Network::Address address;
	};
}

#endif