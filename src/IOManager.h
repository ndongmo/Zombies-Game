/*!
* \file IOManager.h
* \brief Simple IO manager.
* \author Ndongmo Silatsa Fabrice
* \date 27-10-2016
* \version 1.0
*/

#pragma once

#include <vector>

namespace NS2 
{
	/*!
	* \brief File structure.
	*/
	struct DirEntry 
	{
		std::string path;
		bool isDirectory;
	};

	class IOManager
	{
	public:
		/*!
		* \brief Load file into buffer.
		* \param filePath: file path
		* \param buffer: output file buffer
		* \return true for no error, otherwise return false
		*/
		static bool readFileToBuffer(const std::string filePath, std::vector<unsigned char>& buffer);
		/*!
		* \brief Load file into string buffer.
		* \param filePath: file path
		* \param buffer: output file buffer
		* \return true for no error, otherwise return false
		*/
		static bool readFileToBuffer(const std::string filePath, std::string& buffer);
		/*!
		* \brief Gets all directory entries in the directory specified by path and stores in rvEntries.
		* \return false if path is not a directory
		*/
		static bool getDirectoryEntries(const char* path, std::vector<DirEntry>& rvEntries);
		/*!
		* \brief Create directory.
		* \param path: path of directory to make
		* \return true for no error, otherwise return false 
		*/
		static bool makeDirectory(const char* path);
	};
}