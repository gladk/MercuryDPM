/// This file is used for generating defitions that give access to CMakeVaribles from within a cpp file (defintions have only been added as required
#ifndef CMAKEDEFINTIONS_H
#define CMAKEDEFINTIONS_H

#include <string>

const std::string xballsSupportOn();
const std::string getMercurySourceDir();
const std::string getMercuryBuildDir();
constexpr int getMajorVersionNumber();
constexpr int getMinorVersionNumber();
const std::string getVersion();

/*
* This maps the CMake variables to defines used in the Logger.
* Because CMake uses ON and OFF opposed to 0 and 1, we have to
* use some witchery to make it work too.
* We'll clean up the mess afterwards by undefining ON and OFF...
*/
#define ON 1
#define OFF 0
#if @Mercury_BACKTRACE_ENABLE@ == ON
// This symbol is only defined as true, when the stacktrace code should be compiled in.
#define MERCURY_STACKTRACE_SHOW 1
#else
#define MERCURY_STACKTRACE_SHOW 0
#endif

#if @Mercury_BACKTRACE_DEMANGLE@ == ON
// This symbol is only defined as true, when every system required for demangling is present.
#define MERCURY_STACKTRACE_DEMANGLE 1
#else
#define MERCURY_STACKTRACE_DEMANGLE 0
#endif
// Cleaning up our symbols.
#undef ON
#undef OFF

#endif
