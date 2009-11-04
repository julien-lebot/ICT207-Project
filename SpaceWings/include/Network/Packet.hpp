/*
 *	Packet.hpp
 *
 *	Packet class.
 *
 *	Created by Terje Loe
 */

#ifndef PACKET_HPP
#define PACKET_HPP

#include <string>
#include <ObjToSwm/Token.hpp>
#include <sstream>

namespace Network
{
	const unsigned packetSize = 256;
	
	class Packet
	{
	public:
		enum DataType
		{
			ENTITYCREATE,
			ENTITYDESTROY,
			ENTITYSTATE,
			INPUT
		};

		Packet()
		{ 
			data.resize(packetSize - 7);
		}
		
		std::string toString();
		void fromString(std::string str);

		DataType getDataType() { return datatype; }
		std::string getData() { return data; }
		unsigned getSize() { return packetSize; }

		void setDataType(DataType dt) { datatype = dt; }
		void setData(std::string str) { data = str; }
	private:
		DataType datatype;
		std::string data;
	};
}

#endif