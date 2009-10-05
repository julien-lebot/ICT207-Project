/*
*	Console.hpp
*
*	An abstract console class based on Facundo Matias Carreiro's console class.
*	http://www.gamedev.net/reference/programming/features/consoleSTL1/
*
*  Created by Julien Lebot on 07/09/09.
*  Copyright 2009 Julien Lebot. All rights reserved.
*
*/

#include <Phoenix/Console.hpp>

using namespace Phoenix;

Console::Console(std::size_t maxCmdBufSize,
				 std::size_t maxTxtBufSize,
				 bool echo,
				 ConsoleFunction defaultFunction,
				 int commandlineIndex)
: mMaxCmdBufSize(maxCmdBufSize),
  mMaxTxtBufSize(maxTxtBufSize),
  mCmdEcho(echo),
  mDefaultCmd(defaultFunction),
  mLineIndex(commandlineIndex)
{

}

Console::~Console()
{

}

void Console::addItem(const std::string &strName,
					 void *pointer,
					 ItemType type)
{
	Item it;

	// fill item properties
	it.name = strName;
	it.type = type;

	// address
	if(type != FUNCTION_T)
	{
		it.variablePointer = pointer;
	}
	else
	{
		it.function = static_cast<ConsoleFunction> (pointer);
	}

	// add item
	mItemList.push_back(it);

}

void Console::deleteItem(const std::string &strName)
{
	ItemList::iterator it;

	// find item
	for (it = mItemList.begin(); it != mItemList.end(); ++it)
	{
		if (it->name == strName)
		{
			mItemList.erase(it);
			break;
		}
	}

}

void Console::print(const std::string &strText)
{
	// push it
	mTxtBuf.push_back(strText);

	// if we are out of bounds
	if(mTxtBuf.size() > mMaxTxtBufSize)
		mTxtBuf.pop_front();

}

void Console::setCommandBufferSize(std::size_t size)
{
	mMaxCmdBufSize = size;

	// remove any extra line
	if(mCmdBuf.size() > mMaxCmdBufSize)
	{
		std::vector<std::string>::iterator it;
		it = mCmdBuf.end() - mMaxCmdBufSize;
		mCmdBuf.erase(mCmdBuf.begin(), it);
	}

}

void Console::setOutputBufferSize(std::size_t size)
{
	mMaxTxtBufSize = size;

	// remove any extra line
	if(mTxtBuf.size() > mMaxTxtBufSize)
	{
		std::list<std::string>::iterator it;
		int diff = mTxtBuf.size() - mMaxTxtBufSize;
		for(int i = 0; i < diff; i++)
		{
			mTxtBuf.erase(mTxtBuf.begin());
		}
	}

}

std::string Console::getPrevCommand(int index)
{
	// increment line index
	mLineIndex += index;

	// error correct line index
	if(mLineIndex < 0)
	{
		mLineIndex = 0;
	}
	else if(mLineIndex >= mTxtBuf.size())
	{
		mLineIndex = mTxtBuf.size() - 1;
	}

	// set command line
	if(!mCmdBuf.empty())
		return mCmdBuf[mLineIndex];
	else return "";

}

void Console::parseCommandQueue()
{
	std::list<std::string>::iterator it;

	for(it = mCmdQueue.begin(); it != mCmdQueue.end(); ++it)
	{
		parseCommandLine(*it);
	}
	mCmdQueue.clear();
}

void Console::sendMessage(std::string command)
{
	// If you are using multiple threads this is where you 
	// should grab the semaphore to prevent data corruption.
	mCmdQueue.push_back(command);
	// Release the semaphore here.
}

// TODO: rewrite that with boost::spirit, J.L.
bool Console::parseCommandLine(std::string commandLine)
{
	std::ostringstream out;
	std::string::size_type index = 0;
	std::vector<std::string> arguments;
	std::list<Item>::const_iterator iter;

	// add to text buffer
	if(mCmdEcho)
	{
		print(commandLine);
	}

	// add to command line buffer
	mCmdBuf.push_back(commandLine);
	if(mCmdBuf.size() > mMaxCmdBufSize)
		mCmdBuf.erase(mCmdBuf.begin());

	// tokenize
	while(index != std::string::npos)
	{
		// push word
		std::string::size_type nextSpace = commandLine.find(' ', index);
		arguments.push_back(commandLine.substr(index, nextSpace));

		// increment index
		if(nextSpace != std::string::npos)
			index = nextSpace + 1;
		else
			break;
	}

	// execute
	for(iter = mItemList.begin(); iter != mItemList.end(); ++iter)
	{
		if(iter->name == arguments[0])
		{
			switch(iter->type)
			{
			case UCHAR_T:
				if(arguments.size() > 2)
				{
					return true;
				}
				else if(arguments.size() == 1)
				{
					out.str("");
					out << (*iter).name << " = " << *(static_cast<unsigned char *>((*iter).variablePointer));
					print(out.str());
					return true;
				}
				else if(arguments.size() == 2)
				{
					*(static_cast<unsigned char *>((*iter).variablePointer)) = static_cast<unsigned char>(atoi(arguments[1].c_str()));
					return true;
				}
				break;

			case CHAR_T:
				if(arguments.size() > 2)
				{
					return true;
				}
				else if(arguments.size() == 1)
				{
					out.str("");
					out << (*iter).name << " = " << *(static_cast<char *>((*iter).variablePointer));
					print(out.str());
					return true;
				}
				else if(arguments.size() == 2)
				{
					*(static_cast<char *>((*iter).variablePointer)) = static_cast<char>(atoi(arguments[1].c_str()));
					return true;
				}
				break;

			case UINT_T:
				if(arguments.size() > 2)
				{
					return true;
				}
				else if(arguments.size() == 1)
				{
					out.str("");
					out << (*iter).name << " = " << *(static_cast<unsigned int *>((*iter).variablePointer));
					print(out.str());
					return true;
				}
				if(arguments.size() == 2)
				{
					*(static_cast<unsigned int *>((*iter).variablePointer)) = static_cast<unsigned int>(atoi(arguments[1].c_str()));
					return true;
				}
				break;

			case INT_T:
				if(arguments.size() > 2)
				{
					return true;
				}
				else if(arguments.size() == 1)
				{
					out.str("");
					out << (*iter).name << " = " << *(static_cast<int *>((*iter).variablePointer));
					print(out.str());
					return true;
				}
				if(arguments.size() == 2)
				{
					*(static_cast<int *>((*iter).variablePointer)) = static_cast<int>(atoi(arguments[1].c_str()));
					return true;
				}
				break;

			case FLOAT_T:
				if(arguments.size() > 2)
				{
					return true;
				}
				else if(arguments.size() == 1)
				{
					out.str("");
					out << (*iter).name << " = " << *((float *)(*iter).variablePointer);
					print(out.str());
					return true;
				}
				if(arguments.size() == 2)
				{
					*(static_cast<float *>((*iter).variablePointer)) = static_cast<float>(atof(arguments[1].c_str()));
					return true;
				}
				break;

			case STRING_T:
				if(arguments.size() == 1)
				{
					out.str("");
					out << (*iter).name << " = " << *static_cast<std::string *>((*iter).variablePointer);
					print(out.str());
					return false;
				}
				else if(arguments.size() > 1)
				{
					// reset variable
					*(static_cast<std::string *>((*iter).variablePointer)) = "";

					// add new string
					for(unsigned int i = 1; i < arguments.size(); ++i)
					{
						*(static_cast<std::string *>((*iter).variablePointer)) += arguments[i];
					}
					return true;
				}
				break;

			case FUNCTION_T:
				(*iter).function(arguments);
				return true;
				break;

			default:
				mDefaultCmd(arguments);
				return false;
				break;
			}
		}
	}

	// Couldn't match the command
	mDefaultCmd(arguments);
	return false;

}
