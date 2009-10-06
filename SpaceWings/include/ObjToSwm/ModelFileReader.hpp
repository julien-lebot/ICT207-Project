/*
*	ModelFileReader.hpp
*
*	base class for model file readers
*
*	Created by Terje Loe
*/

#ifndef MODELFILEREADER_HPP
#define MODELFILEREADER_HPP

#include <Phoenix\NonCopyable.hpp>
#include <Phoenix\model.hpp>
#include <Phoenix\Material.hpp>
#include <string>

namespace Phoenix
{
	class ModelFileReader : public NonCopyable
	{
	public:
		virtual void readFile( const std::string& fileName , Phoenix::Model& model) {}
	
	};
}

#endif