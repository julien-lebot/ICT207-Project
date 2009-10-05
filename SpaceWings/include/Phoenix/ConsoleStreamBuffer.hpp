
#ifndef __ConsoleStreamBuffer_hpp__
#define __ConsoleStreamBuffer_hpp__

#include <streambuf>
#include <istream>
#include <ostream>
#include <iostream>
#include <vector>

template
<
	class InputPolicy,
	class OutputPolicy
>
class ConsoleStreamBuffer: private InputPolicy,
						   private OutputPolicy,
						   public std::basic_streambuf<char, std::char_traits<char> >
{
		std::vector<char> mInputbuffer;
		char* mPBeg, *mPCur, *mGBeg, *mGCur;
		int mPLength, mGLength;

		typedef std::char_traits<char> char_traits;
protected:
	virtual int uflow()
	{
		char c;

		while((c = getInput()) != '\n')
			mInputbuffer.push_back(c);

		mInputbuffer.push_back(c);

		_Gninc();
		c = *gptr();

		mGLength =  &mInputbuffer[mInputbuffer.size()] - gptr();

		return c;
	}

	virtual int underflow()
	{
		if(gptr() && gptr() < egptr())
			return *_Gninc();

		mInputbuffer.clear();
		mInputbuffer.reserve(32);

		int c = uflow();
		_Gndec();

		return char_traits::not_eof(c);
	}

	virtual int overflow(int c = char_traits::eof())
	{
		if(c == char_traits::eof())
			return char_traits::not_eof(c);

		drawCharacter(c);
		return c;
	}

public:
	ConsoleStreamBuffer()
	: mPBeg(0),
	  mPCur(0),
	  mPLength(0),
	  mGBeg(0),
	  mGLength(0)
	{
		mInputbuffer.resize(32);
		mGCur = &mInputbuffer[0];
		_Init(&mGBeg, &mGCur, &mGLength, &mPBeg, &mPCur, &mPLength);
	}

	virtual ~ConsoleStreamBuffer() {}
};

template
<
	class InputPolicy,
	class OutputPolicy
>
class ConsoleInStream : public std::basic_istream<char, std::char_traits<char> >
{
	ConsoleStreamBuffer<InputPolicy, OutputPolicy> _streambuffer;
public:
	ConsoleInStream()
	:_streambuffer(),
	 std::basic_istream<char, std::char_traits<char> >(&_streambuffer)
	{
		clear();
	}
};

template
<
	class InputPolicy,
	class OutputPolicy
>
class ConsoleOutStream : public std::basic_ostream<char, std::char_traits<char> >
{
	ConsoleStreamBuffer<InputPolicy, OutputPolicy> _streambuffer;
public:
	ConsoleOutStream()
	:_streambuffer(),
	 std::basic_ostream<char, std::char_traits<char> >(&_streambuffer)
	{
		clear();
	}
};

template
<
	class InputPolicy,
	class OutputPolicy
>
class ConsoleIOStream : public std::basic_iostream<char, std::char_traits<char> >
{
	ConsoleStreamBuffer<InputPolicy, OutputPolicy> _streambuffer;
public:
	ConsoleIOStream()
	:_streambuffer(),
	 std::basic_iostream<char, std::char_traits<char> >(&_streambuffer)
	{
		clear();
	}
};

#endif // __ConsoleStreamBuffer_hpp__