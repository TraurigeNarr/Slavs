#pragma once

#include <string>

#include <boost\utility.hpp>

class FileUtilities : boost::noncopyable
{
private:
  FileUtilities();
  ~FileUtilities();

public:
  /** Gets the directory of the currently running program */
	static std::string  GetApplicationDirectory();

  static void					SplitFilename(const std::string& i_qualified_name, std::string& o_out_basename, std::string& o_out_path);

	/** Joins two files paths together, returning the result */
	static std::string  JoinPath(const std::string& i_first_path, const std::string& i_second_path);

	static void         MakeFullPath(std::string& i_path);

	/** Determines if the specified path is an absolute path */
	static bool         IsAbsolutePath(const std::string& i_path);

	static bool         IsSeparator(char i_symbol);

	static void         EnsureTrailingPathSeparator(std::string& io_path);
};