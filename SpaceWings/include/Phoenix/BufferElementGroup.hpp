/*
*	BufferElementGroup.hpp
*
*	A class representing a group of elements in a vertex buffer: e.g. vertex positions, vertex normals, texture coordinates etc...
*
*  Created by Julien Lebot on 07/10/09.
*  Copyright 2009 Julien Lebot. All rights reserved.
*
*/

#ifndef __BufferElementGroup_hpp__
#define __BufferElementGroup_hpp__

#include <Phoenix/Base.h>
#include <list>

namespace Phoenix
{
	/// The meaning of the various group in the buffer
	enum BufferElementSemantic
	{
		POSITION = 1,
		//BLEND_WEIGHTS = 2,
		//BLEND_INDICES = 3,
		NORMAL = 4,
		DIFFUSE = 5,
		SPECULAR = 6,
		TEXTURE_COORDINATES = 7,
	};

	/// The type of the elements in the buffer
	enum BufferElementType
	{
		FLOAT1,
		FLOAT2,
		FLOAT3,
		FLOAT4,
		COLOUR,
		SHORT1,
		SHORT2,
		SHORT3,
		SHORT4,
		UBYTE4
	};

	/**
	 * @brief A class representing a group of elements in a vertex buffer.
	 */
	class BufferElementGroup
	{
	public:

		/// A class representing one element in the buffer
		class BufferElement
		{
		public:
			/**
			 * @brief Constructs a buffer element. Should not be used directly, use BufferElementGroup.addElement.
			 * @param source The bound buffer source.
			 * @param offset The offset in bytes in the source buffer.
			 * @param semantic The meaning of that element.
			 * @param type The type of that element.
			 * @param index The index of that element in case of multiple similar elements.
			 */
			BufferElement(unsigned short source,
				std::size_t offset,
				BufferElementSemantic semantic,
				BufferElementType type,
				unsigned short index=0)
				: mSourceID(source), mIndex(index), mOffset(offset), mSemantic(semantic), mType(type)
			{}

			/**
			 * Returns the id of the source buffer.
			 */
			unsigned short getSource() const { return mSourceID; }

			/**
			 * Returns the index of that element.
			 */
			unsigned short getIndex() const { return mIndex; }

			/**
			 * Get the offset in the source buffer of that element.
			 */
			std::size_t getOffset() const { return mOffset; }

			/**
			 * Get the semantic of that element.
			 */
			BufferElementSemantic getSemantic() const { return mSemantic; }

			/**
			 * Get the type of that element.
			 */
			BufferElementType getType() const { return mType; }

		protected:
			unsigned short mSourceID,			/// The id of the buffer this element belongs to.
						   mIndex;				/// For texture coordinates, we can have multiple elements for multi-texturing.
			std::size_t mOffset;				/// Offset in the buffer of this element.
			BufferElementSemantic mSemantic;	/// The meaning of this element.
			BufferElementType mType;			/// The type of this element.
		};

		/// List of elements in that group.
		typedef std::list<BufferElement> ElementList;

		/**
		 * @brief Adds an element in the list.
		 * @param source The buffer source.
		 * @param offset The offset in the source buffer.
		 * @param semantic The meaning of that element.
		 * @param type The type of that element.
		 * @param index The index of that element in case of multiple similar elements.
		 */
		void addElement(unsigned short source,
			std::size_t offset,
			BufferElementSemantic semantic,
			BufferElementType type,
			unsigned short index=0)
		{
			mElementList.push_back(BufferElement(source, offset, semantic, type, index));
		}

		/// Retrieves the list of elements
		const ElementList& getElements() { return mElementList; }
	protected:

		/// The list of elements.
		ElementList mElementList;
	};

}

#endif // __BufferElementGroup_hpp__