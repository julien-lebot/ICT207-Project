/*
*	Console.hpp
*
*	An abstract console class based on Facundo Matias Carreiro's console class.
*	http://www.gamedev.net/reference/programming/features/consoleSTL1/
*
*  Created by Julien Lebot on 06/09/09.
*  Copyright 2009 Julien Lebot. All rights reserved.
*
*/

#ifndef __Console_hpp__
#define __Console_hpp__

#include <Phoenix/Base.h>
#include <vector>
#include <list>
#include <sstream>
#include <assert.h>

namespace Phoenix {
	class Console
	{
	public:
		enum ItemType
		{
			UCHAR_T,
			CHAR_T,
			UINT_T,
			INT_T,
			FLOAT_T,
			STRING_T,
			FUNCTION_T
		};

		typedef void (*ConsoleFunction)(const std::vector<std::string> &);

		struct Item
		{
			std::string name;
			ItemType type;
			union
			{
				ConsoleFunction function;
				void *variablePointer;
			};
		};

		typedef std::list<Item> ItemList;

		Console(std::size_t maxCmdBufSize, std::size_t maxTxtHistory, bool echo, ConsoleFunction defaultFunction, int commandlineIndex);
		virtual ~Console();

		void addItem(const std::string &strName, void *pointer, ItemType type);
		void deleteItem(const std::string &strName);
		void print(const std::string &strText);

		void setCommandBufferSize(std::size_t size);
		std::size_t getCommandBufferSize() { return mMaxCmdBufSize; }
		void clearCommandBuffer() { mCmdBuf.clear(); }

		void setOutputBufferSize(std::size_t size);
		std::size_t getOutputBufferSize() { return mMaxTxtBufSize; }
		void clearOutputBuffer() { mTxtBuf.clear(); }

		std::string getPrevCommand(int index);
		void setDefaultCommand(ConsoleFunction def) { mDefaultCmd = def; }

		void parseCommandQueue();
		void sendMessage(std::string command);
		void sendImmediateMessage(std::string command) { parseCommandLine(command); }

		virtual void render() = 0;

	protected:
		std::list<std::string> mTxtBuf;		// Text buffer that is actually rendered
		std::vector<std::string> mCmdBuf;	// Commands buffer

	private:
		bool parseCommandLine(std::string commandLine);

		bool mCmdEcho;						// Whether to echo the commands typed in the text buffer or not
		unsigned int mLineIndex;
		std::size_t mMaxCmdBufSize;
		std::size_t mMaxTxtBufSize;
		ConsoleFunction mDefaultCmd;		// The default command to execute if nothing is matched
		ItemList mItemList;					// The list of items that has been registered with the CL
		std::list<std::string> mCmdQueue;	// Buffers the commands to be processed
	};
}

#endif // __Console_hpp__