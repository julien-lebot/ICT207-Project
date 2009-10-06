/*
*	ModelFilewriter.hpp
*
*	base class for model file writers
*
*	Created by Terje Loe
*/

#ifndef MODELFILEWRITER_HPP
#define MODELFILEWRITER_HPP

#include <Phoenix\NonCopyable.hpp>
#include <Phoenix\Material.hpp>
#include <Phoenix\model.hpp>
#include <string>

namespace Phoenix
{
	class ModelFileWriter : public NonCopyable
	{
	public:
		virtual void writeFile ( const std::string& fileName, const Model& model ) {}
	};
}

#endif