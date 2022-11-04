// =====================================================================
//   @ Author: Cucorianu Eusebiu Adelin                                                                                      
//   @ Create Time: 22-10-2022 8:03 PM                                                                                                                                                
//   @ Contact: cucorianu.adelin@protonmail.com                                                                                                                          
//   @ Modified time: 23-10-2022 2:33 PM                                                                                                                                    
//   @ Description: This source code is the core of the program. It
//	   handles the input using a multithreading worker pool. It has an
//	   constructor that receives data from command line input user.After
//     that it extracts all file names recursively and saves them in a
//     queue.Then the worker starts to search async the string that is
//     required.Lastly the program prints the expected output.
// =====================================================================

#ifndef HANDLE_INPUT
#define HANDLE_INPUT

#include <string>

#include "Platform/platform.hpp"
#include "Worker/worker.h"

namespace Eset
{
	class HandleInput
	{
	public:
		HandleInput( ) noexcept = delete;

		///\brief Constructor that takes the argc and argv from the user input
		///\param argc number of arguments from the user input
		///\param argv arguments of user input(path where we search and string that search for)
		HandleInput( int32 argc, const char **argv );

		// cant copy
		HandleInput( const HandleInput &handleInput ) = delete;
		HandleInput &operator=( const HandleInput &handleInput ) = delete;

		// cant move
		HandleInput( HandleInput &&handleInput ) = delete;
		HandleInput &operator=( HandleInput &&handleInput ) = delete;

		///\brief Checks the given params from user input
		///\return Status of execution
		RStatus HandleInputFiles( ) noexcept;

		~HandleInput( ) noexcept = default;

	private:
		///\brief The path where we do the search
		std::string m_path;

		///\brief The string that we have to search for
		std::string m_toSearch;

		///\brief The worker that executes the task async
		Worker m_worker;

		///\brief The queue that contains all of the file names where we search for the string
		std::vector<std::string> m_fileRoutes;

		///\brief Argc comes from user input
		int32 m_argc{ 0 };

		///\brief Argv comes from user input
		const char **m_argv{ nullptr };

		///\brief Get all the file names from a given path recursively
		///\param path The path where we search
		///\return Status of execution
		RStatus GetAllFileNames( const std::string &path ) noexcept;
	};
} // namespace Eset
#endif