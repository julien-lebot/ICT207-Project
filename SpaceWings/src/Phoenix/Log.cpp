/**
* @file Log.cpp
*
* @author Joshua Wilding
* modified on 07/10/2009 by Julien Lebot: modified order of namespace inclusion (after includes, not before), added namespace Phoenix.
* @date 06/10/2009
*/

#include <fstream>
#include <string>
#include <ctime>

#include <Phoenix/Log.hpp>

using namespace std;
using namespace Phoenix;

Log::Log(const std::string fileName)
: mFileName(fileName)
{

}

void Log::write(string str)
{
	char timestr [12];

	ofstream file(mFileName.c_str(), ios::app);
	time_t rawtime = time(NULL);

	strftime(timestr, 12, "[%I:%M:%S] ", localtime ( &rawtime ));
	file << timestr << str << endl;

	file.close();
}