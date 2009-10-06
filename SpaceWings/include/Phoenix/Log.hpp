/**
 * @file Log.hpp
 * @class Log
 *
 * @brief This class is a logging class, it is used for debugging.
 *
 * @author Joshua Wilding
 * modified on 07/10/2009 by Julien Lebot: modified order of namespace inclusion (after includes, not before), added namespace Phoenix.
 * @date 06/10/2009
 */

#ifndef __LOG_HPP
#define __LOG_HPP

namespace Phoenix
{
	class Log
	{
	public:
		/**
		* write This function saves a given string into 'logname'.log.
		*
		* @param logname This parameter defines which log to put str into.
		* @param str This parameter contains the string to be saved into a file.
		*/
		void write(std::string);

		Log(const std::string fileName);
	private:

		std::string mFileName;
	/**
	* Default constructor: blank constructor.
	*/ 
		Log() {}
	};
}
#endif // __LOG_HPP

