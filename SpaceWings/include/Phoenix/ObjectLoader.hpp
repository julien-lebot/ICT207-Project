#ifndef
#define

#include <fstream>
#include <vector>
#include <string>

Class ObjectLoader
{
Public:
	ObjectLoader(){}
	ReadFile(string fileName);
Private:
	fstream file;
	vector<float> vertices;
	vector<int> faces;
};

#endif