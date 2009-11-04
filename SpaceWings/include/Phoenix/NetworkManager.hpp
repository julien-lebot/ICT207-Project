/*
*	NetworkManager.hpp
*
*	NetworkManager class.
*
*	Created by Terje Loe
*/

#ifndef NETWORKMANAGER_HPP
#define NETWORKMANAGER_HPP

#include <Phoenix/NonCopyable.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/numeric/conversion/cast.hpp>
#include <Network/Connection.hpp>
#include <Network/Packet.hpp>
#include <ObjToSwm/Token.hpp>
#include <Phoenix/EntityManager.hpp>
#include <Phoenix/Input.hpp>
#include <iostream>
#include <vector>

namespace Phoenix
{
	const int ServerPort = 30000;
	const int ClientPort = 30001;
	const std::string ProtocolId = "SpWi";
	const float TimeOut = 1000.0f;
	const float DeltaTime = 0.25f;

	class NetworkManager : public NonCopyable
	{
	public:
		NetworkManager (){ setup (); }
		~NetworkManager(){ Network::ShutdownSockets(); }
		
		bool setup ();
		void sendPackets(std::vector<Network::Packet> & list) { sendPacketList.insert(sendPacketList.end(), list.begin(), list.end()); }
		void sendPacket ( Network::Packet packet ) { sendPacketList.push_back( packet ); }
		void send_recv();
		void sendinput(Input inp);
		void processIncoming(Phoenix::EntityManager & em);

		Network::Connection::Mode getMode () const { return (*connection).GetMode(); }

		bool isConnected () const { return (*connection).IsConnected(); }
		bool isServer() const { return getMode() == Network::Connection::Server; }
	private:
		Network::Connection * connection;
		std::vector<Network::Packet> sendPacketList;
		std::vector<Network::Packet> recvPacketList;
	};
}

#endif