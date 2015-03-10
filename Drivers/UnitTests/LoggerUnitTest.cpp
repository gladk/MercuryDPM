//Copyright (c) 2013-2014, The MercuryDPM Developers Team. All rights reserved.
//For the list of developers, see <http://www.MercuryDPM.org/Team>.
//
//Redistribution and use in source and binary forms, with or without
//modification, are permitted provided that the following conditions are met:
//  * Redistributions of source code must retain the above copyright
//    notice, this list of conditions and the following disclaimer.
//  * Redistributions in binary form must reproduce the above copyright
//    notice, this list of conditions and the following disclaimer in the
//    documentation and/or other materials provided with the distribution.
//  * Neither the name MercuryDPM nor the
//    names of its contributors may be used to endorse or promote products
//    derived from this software without specific prior written permission.
//
//THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
//ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
//WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
//DISCLAIMED. IN NO EVENT SHALL THE MERCURYDPM DEVELOPERS TEAM BE LIABLE FOR ANY
//DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
//(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
//LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
//ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
//(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
//SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

#include <iostream>
#include "Logger.h"

// --- Declaring a logger.
// --- This allows you to redefine LogLevels based on command line options.
#ifndef HG_LOGLEVEL_SELFTEST
#define HG_LOGLEVEL_SELFTEST Log::DEFAULT
#endif
Logger<HG_LOGLEVEL_SELFTEST> unitLogger("UnitTester");

/* Since we're testing the logger, lets not use the logger itself... */
void assertOrDie(bool condition, std::string message) {
  if (!condition) {
    std::cerr << "Assert failed: " << message << std::endl;
    std::exit(1);
  }
}

int main() {
    int x = 3;
    bool hasLogged = false;
    std::string lastMessage;
    std::string lastModule;
    auto tmpLogger = [&](std::string module, std::string msg) {
      lastMessage = msg;
      lastModule = module;
      hasLogged = true;
    };
    loggerOutput->onFatal   = tmpLogger;
    loggerOutput->onError   = tmpLogger;
    loggerOutput->onWarn    = tmpLogger;
    loggerOutput->onInfo    = tmpLogger;
    loggerOutput->onVerbose = tmpLogger;
    loggerOutput->onDebug   = tmpLogger;
    
    //Basic usage cases:
    unitLogger(ERROR, "Oopsie!"); //An error!
    assertOrDie(hasLogged, "No output detected!");
    hasLogged = false;
    
    unitLogger(FATAL, "Mweh. x = %", x);
    assertOrDie(hasLogged, "No output detected!");
    assertOrDie(lastMessage == "Mweh. x = 3", "Substitution gone wrong!");
    hasLogged = false;
    
    unitLogger(DEBUG, "You won't see me!");
    assertOrDie(!hasLogged, "Output detected!");
    
    unitLogger(WARN, "Escapes are possible! %\% sure!", 100.01f);
    assertOrDie(lastMessage == "Escapes are possible! 100.01% sure!", "Escape gone wrong!");
    //Usage case for redefining with an function

    unitLogger(FATAL, "Test");
    assertOrDie(lastModule == "UnitTester", "Module was incorrect!");
    logger(FATAL, "Test");
    assertOrDie(lastModule != "UnitTester", "Module was incorrect!");

    return 0;    
}
