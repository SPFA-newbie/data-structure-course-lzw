#pragma once
#ifndef LZW_EXCEPTION
#define LZW_EXCEPTION
#ifndef NULL
#define NULL 0
#endif

#include<exception>
#include<string>

class LzwCoreException : public std::exception
{
};
//------------------

class InvalidNavDataLen : public LzwCoreException
{
	const char* what()const throw()
	{
		return "Input a invalid navDataLen";
	}
};

class InvalidMapCodeLen : public LzwCoreException
{
	const char* what()const throw()
	{
		return "Input a invalid mapCodeLen";
	}
};

class InvalidDocSizeLen : public LzwCoreException
{
	const char* what()const throw()
	{
		return "Input a invalid docSizeLen";
	}
};
//------------------------------------------

class InvalidFileNumber : public LzwCoreException
{
	const char* what()const throw()
	{
		return "The number of file is invalid (=0)";
	}
};

class InvalidFileHeader : public LzwCoreException
{
	const char* what()const throw()
	{
		return "File Header is invalid";
	}
};

#endif
