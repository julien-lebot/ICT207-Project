/*
*	Factory.hpp
*
*	A generic object factory based on Andrei Alexandrescu' Object Factories in Modern C++ Design.
*
*  Created by Julien Lebot on 12/09/09.
*  Copyright 2009 Julien Lebot. All rights reserved.
*
*/

#ifndef __Factory_hpp__
#define __Factory_hpp__

#include <map>
#include <exception>

namespace Phoenix
{
	/**
	 * @brief The default factory error policy that throws an exception.
	 */
	template
	<
		class IdentifierType,
		class AbstractProduct
	>
	class DefaultFactoryError
	{
	public:
		/**
		 * @brief A helper subclass of std::exception to describe the policy's exception.
		 */
		class Exception
		: public std::exception
		{
		public:
			/// Default constructor.
			Exception(const IdentifierType& unknownID)
			: mUnknownID(unknownID)
			{}

			/// Describes the exception
			virtual const char* what()
			{
				return "Unknown object type passed to Factory.";
			}

			/// Returns the unknown identifier
			const IdentifierType& getID()
			{
				return mUnknownID;
			}
		private:
			IdentifierType mUnknownID;
		};
	protected:
		static AbstractProduct* onUnknownType(const IdentifierType& id) // throws Exception
		{
			throw Exception(id);
		}
	};
	/**
	 * @brief A generic object factory.
	 * TODO: Support argument passing through type list or similar mechanism and overloading.
	 *
	 * @param AbstractProduct The abstract product type.
	 * @param IdentifierType The type of the identifier used to uniquely differentiate the concrete products.
	 * @param ProductCreator The type of the creators of concrete products. Must provide a method with the signature AbstractProduct* (*)().
	 * @param FactoryErrorPolicy The error policy. Must define a method with the following signature AbstractProduct *onUnknownType(const IdentifierType&).
	 */
	template
	<
		class AbstractProduct,
		class IdentifierType,
		class ProductCreator,
		template<typename, class> class FactoryErrorPolicy = DefaultFactoryError
	>
	class ObjectFactory
	: public FactoryErrorPolicy<IdentifierType, AbstractProduct>
	{
	public:
		/**
		 * @brief Registers a creator.
		 * @param identifier The identifier that describes the concrete product.
		 * @param creator The creator for that concrete product.
		 * @return Whether the operation was successful or not.
		 */
		bool Register(const IdentifierType &identifier, ProductCreator creator)
		{
			return mAssoc.insert(AssocMap::value_type(identifier, creator)).second;
		}

		/**
		 * @brief Unregisters a creator.
		 * @param identifier The identifier to remove from the factory.
		 * @return Whether the operation was successful or not.
		 */
		bool Unregister(const IdentifierType &identifier)
		{
			return mAssoc.erase(identifier) == 1;
		}

		/**
		 * @brief Creates an instance of IdentifierType's concrete type.
		 * @param identifier The identifier of the concrete type to create.
		 * @return A pointer to the newly instantiated object.
		 */
		AbstractProduct* create(const IdentifierType &identifier)
		{
			typename AssocMap::const_iterator cit = mAssoc.find(identifier);

			if (i != mAssoc.end())
			{
				return (i->second())();
			}
			else
				return onUnknownType(identifier);
		}
	private:
		/// Maps ids with creators
		typedef std::map<IdentifierType, ProductCreator> AssocMap;
		
		/// The map of ids and creators
		AssocMap mAssoc;
	};
}

#endif // __Factory_hpp__