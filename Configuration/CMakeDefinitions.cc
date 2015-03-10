/// This file is used for generating defitions that give access to CMakeVaribles from within a cpp file (defintions have only been added as required

#include <CMakeDefinitions.h>

const std::string xballsSupportOn()
{
	return "@Mercury_Include_Xballs_Support@";
}

const std::string getMercurySourceDir()
{
	return "@Mercury_SOURCE_DIR@";
}

const std::string getMercuryBuildDir()
{
	return "@Mercury_BINARY_DIR@";
}

constexpr int getMajorVersionNumber()
{
	return @Mercury_VERSION_MAJOR@;
}

constexpr int getMinorVersionNumber()
{
	return @Mercury_VERSION_MINOR@;
}

const std::string getVersion()
{
	return "@Mercury_VERSION_MAJOR@.@Mercury_VERSION_MINOR@";
}

