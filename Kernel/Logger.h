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

#ifndef LOGGER_H
#define LOGGER_H

#include <string>
#include <sstream>
#include <functional>

#ifndef LOG_LEVEL
#define LOG_LEVEL Log::DEFAULT
#endif

/*!
 * \brief
 */
enum class Log : signed char {
  FATAL =   -20,
  ERROR =   -15,
  WARN  =   -10,
  INFO  =   -5,
  DEFAULT = 0,
  VERBOSE = 5,
  DEBUG =   10,
};

/*!
 * \brief 
 */
constexpr bool operator<=(const Log rhs, const Log lhs) {
  return ((static_cast<signed char>(rhs)) <= (static_cast<signed char>(lhs)));
}
 
/*!
 * \class LoggerOutput
 * \brief
 */
class LoggerOutput {
  public:
    std::function<void(std::string,std::string)> onFatal;
    std::function<void(std::string,std::string)> onError;
    std::function<void(std::string,std::string)> onWarn;
    std::function<void(std::string,std::string)> onInfo;
    std::function<void(std::string,std::string)> onVerbose;
    std::function<void(std::string,std::string)> onDebug;
};

/*!
* \brief 
*/
extern LoggerOutput* loggerOutput;

template<Log L = Log::DEFAULT> class Logger;

/*!
 * \class Logger
 * \brief 
 */

template<Log L>
class Logger {
  private:
    /*!
     * \brief 
     */
    const std::string module;
    
  public:

    /*!
     * \brief constructor
     */
     Logger(const std::string name) : module(name) { }
    /*!
     * \brief destructor
     */
     ~Logger() {}

    /*!
     * \brief 
     */     
     template<typename... Args>
     void log(const Log loglevel, const std::string& format, Args&&... arg) {
       if (loglevel <= L || loglevel < LOG_LEVEL) {
         std::stringstream msgstream;
         createMessage(msgstream, format.c_str(), arg...);
         if        (loglevel <= Log::FATAL) {
           loggerOutput->onFatal(module, msgstream.str());
         } else if (loglevel <= Log::ERROR) {
           loggerOutput->onError(module, msgstream.str());
         } else if (loglevel <= Log::WARN) {
           loggerOutput->onWarn(module, msgstream.str());
         } else if (loglevel <= Log::INFO) {
           loggerOutput->onInfo(module, msgstream.str());
         } else if (loglevel <= Log::VERBOSE) {
           loggerOutput->onVerbose(module, msgstream.str());
         } else {
           loggerOutput->onDebug(module, msgstream.str());
         }
       }
     }
  private:
    /*!
     * \brief 
     */
     //Put all the messages 
     template<typename Arg1, typename... Args>
     void createMessage(std::stringstream& msg, const char* fmt, 
                                                  Arg1&& arg, Args&&... args) {
       bool doSkipNext = false;
       while (*fmt != '%' && !doSkipNext) {
         doSkipNext = false;
         //Make sure we're not running past the end of our formatting string.
         if (*fmt == '\0')
           return;
         if (*fmt == '\\') { //Escape for the %sign
           doSkipNext = true;
         } else {
           msg << *fmt;
           fmt++;
         }
       }

       fmt++; //Consume the % sign
       msg << arg;
       createMessage(msg, fmt, args...);
     }
     
    /*!
     * \brief 
     */
     template<typename Arg1>
     void createMessage(std::stringstream& msg, const char* fmt, Arg1&& arg) {
       bool doSkipNext = false;
       while (*fmt != '%' && !doSkipNext) {
         doSkipNext = false;
         if (*fmt == '\0')
           return;
         if (*fmt == '\\') { //Escape for the %sign
           doSkipNext = true;
         } else {
           msg << *fmt;
           fmt++;
         }
       }
       fmt++; //Consume the % sign
       msg << arg;
       while (*fmt != '\0') {
         msg << *fmt;
         fmt++;
       }
     }
     

    /*!
     * \brief 
     */
     //No arg case.
     void createMessage(std::stringstream& msg, const char* message) {
       msg << message;
     }
};


#endif
