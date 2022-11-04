#include "Handle/handle_input.h"
#include "Platform/platform.hpp"

#include <filesystem>
#include <fstream>
#include <regex>

namespace Eset
{
	///\brief Searches for stringToSearch in path file
	///\param path Path where we search
	///\param stringToSearch The string that we search for
	static void SearchString( const std::string& path, const std::string &stringToSearch ) noexcept;

	///\brief Convert a string to a raw string
	///\param string String that will become raw
	///\return The raw string
	static std::string ToRawString(std::string string) noexcept;

	///\brief The mutex that ensures sync of output
	static std::mutex logMutex;

	HandleInput::HandleInput( const int32 argc, const char **argv )
		:		m_worker( 2 )
		,		m_argc( argc )
		,		m_argv( argv )
	{ }

	RStatus HandleInput::HandleInputFiles( ) noexcept
	{
		if( m_argc != 3 ) UNLIKELY
		{
			m_worker.Done();
			return RInvalidNumberOfArguments;
		}

		const std::string path			 = m_argv[ 1 ];
		const std::string stringToSearch = m_argv[ 2 ];

		GetAllFileNames( path );

		for( const auto &localPath : m_fileRoutes )
		{
			m_worker.Push( [ localPath, stringToSearch ]
			{
				SearchString( localPath, stringToSearch );
			} );
		}
		m_worker.Done( );

		return RSucces;
	}

	RStatus HandleInput::GetAllFileNames( const std::string &path ) noexcept
	{
		if( !std::filesystem::exists( path ) ) UNLIKELY
		{
			Log( "Invalid path name." );
			m_worker.Done( );
			return RInvalidPathName;
		}

		for( const auto &entry : std::filesystem::recursive_directory_iterator( path ) )
		{
			const auto &genericPath = entry.path( );
			if( !genericPath.has_extension( ) )
				continue;
			m_fileRoutes.emplace_back( genericPath.string( ).c_str( ) );
		}

		return RSucces;
	}

	std::string ToRawString( std::string string ) noexcept
	{
		std::string raw = std::move( string );
		size_t		pos = 0;
		while( ( pos = raw.find_first_of( "\t\n\\", pos ) ) != std::string::npos )
		{
			switch( raw[ pos ] )
			{
				case '\t':
					raw.replace( pos, 1, "\\t" );
					pos += 2;
					break;
				case '\n':
					raw.replace( pos, 1, "\\n" );
					pos += 2;
					break;
				case '\\':
					raw.replace( pos, 1, "\\\\" );
					pos += 4;
					break;
				default:
					break;
			}
		}
		return raw;
	}

	void SearchString( const std::string &path, const std::string &stringToSearch ) noexcept
	{
		std::ifstream stream( path );

		std::string line;
		while( std::getline( stream, line ) )
		{
			if( line.length( ) == 0 ) UNLIKELY
				break;
			int32 pos = line.find( stringToSearch );
			while( pos >= 0 )
			{
				const int32 prefixStart = std::max( 0, pos - 3 );
				int32 prefixEnd	  = 3;
				if( prefixStart == 0 )
					prefixEnd = 0;
				const int32 suffixStart = pos + stringToSearch.length( );
				int32 suffixEnd	  = 3;

				const auto len = pos + stringToSearch.length( ) + 3;
				if( len > line.length( ) )
					suffixEnd = len - line.length( );

				const auto prefix = line.substr( prefixStart, prefixEnd );
				const auto suffix = line.substr( suffixStart, suffixEnd );

				// Get last slash of path and extract a sub string begining with it.
				const auto lastSlash = path.find_last_of( R"(\)" );
				const auto fileName	 = path.substr( lastSlash + 1 );
				AsyncLog( logMutex, "<{}>({})<{}>...<{}>", fileName, pos, ToRawString( prefix ), ToRawString( suffix ) );

				pos = line.find( stringToSearch, pos + 1 );
			}
		}
	}
}
