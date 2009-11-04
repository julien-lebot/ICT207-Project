#include <Network/Packet.hpp>

using namespace Network;

std::string Packet::toString()
{
	std::ostringstream ss;
	ss << " " << datatype << " " << data;
	return ss.str();
}

void Packet::fromString(std::string str)
{
	Phoenix::Token tokens;
	tokens.tokenizeStr(str, " ");
	
	datatype = (DataType)atoi ( tokens.getNext().c_str() );
	
	data = tokens.getRest();
}