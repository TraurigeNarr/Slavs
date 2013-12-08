#include "FileUtilities.h"

#include <algorithm>
#include <vector>

#include <Windows.h>
////////////////////////////////////////////

FileUtilities::FileUtilities()
{
}

FileUtilities::~FileUtilities()
{
}

std::string FileUtilities::GetApplicationDirectory()
{
	std::string path;    

#if defined(_WIN32)
	const DWORD maxLength = MAX_PATH + 1;
	char pathChars[maxLength];
	GetModuleFileNameA(GetModuleHandle(0), pathChars, maxLength);
	std::string baseName;
	SplitFilename(pathChars, baseName, path);
#else
	const unsigned int maxLength = PATH_MAX + 1;
	char pathChars[maxLength];
	getcwd(pathChars, maxLength);
	path = pathChars;
#endif

	EnsureTrailingPathSeparator(path);

	return path;
}

void FileUtilities::SplitFilename(const std::string& i_qualified_name, 
	std::string& o_out_basename, std::string& o_out_path)
{
	std::string path = i_qualified_name;
	// Replace \ with / first
	std::replace( path.begin(), path.end(), '\\', '/' );
	// split based on final /
	size_t i = path.find_last_of('/');

	if (i == std::string::npos)
	{
		o_out_path.clear();
		o_out_basename = i_qualified_name;
	}
	else
	{
		o_out_basename = path.substr(i+1, path.size() - i - 1);
		o_out_path = path.substr(0, i+1);
	}
}

std::string FileUtilities::JoinPath(const std::string& i_first_path, const std::string& i_second_path)
{
	//If path2 is an absolute path, it supercedes path1
	if (IsAbsolutePath(i_second_path))
		return i_second_path;

	//Path2 is not absolute
	std::string joined(i_first_path);
	EnsureTrailingPathSeparator(joined);

	//Skip leading separators in path2
	size_t charIndex = 0;
	while (charIndex < i_second_path.length() && IsSeparator(i_second_path[charIndex]))
		charIndex++;

	//If not at the end of path2, append it
	if (charIndex < i_second_path.length())
		joined += i_second_path.substr(charIndex, i_second_path.length() - charIndex);

	//Remove relative components
	MakeFullPath(joined);

	return joined;
}

void FileUtilities::MakeFullPath(std::string& i_path)
{
#if defined(_WIN32)
	DWORD maxLength = MAX_PATH + 1;

	std::vector<char> fullPath;
	fullPath.resize(maxLength);

	char* filePath;
	DWORD result = GetFullPathNameA(i_path.c_str(), maxLength, &fullPath[0], &filePath);
	if (result > maxLength)
	{
		fullPath.resize(result);
		result = GetFullPathNameA(i_path.c_str(), result, &fullPath[0], &filePath);
	}

	i_path = &fullPath[0];
#endif
}

bool FileUtilities::IsSeparator(char i_symbol)
{
	return i_symbol == '\\' || i_symbol == '/';
}

bool FileUtilities::IsAbsolutePath(const std::string& i_path)
{
if (i_path.empty())
	return false;
else if (i_path.length() > 1)
{
#if defined(_WIN32)
	if (i_path.length() == 1)
		return IsSeparator(i_path[0]);
	else
		return i_path[1] == ':' || (IsSeparator(i_path[0]) && IsSeparator(i_path[1]));
#else
	return IsSeparator(i_path[0]);
#endif
}
else
	return false;
}

void FileUtilities::EnsureTrailingPathSeparator(std::string& io_path)
{
	if (io_path.length() > 0)
	{
		char lastChar = io_path[io_path.size() - 1];
		if (!IsSeparator(lastChar))
			io_path += "/";
	}
}
