/* 
 * File:   cCommonTools.h
 * Author: juia
 *
 * Created on 17. April 2016, 17:12
 */

#ifndef CCOMMONTOOLS_H
#define	CCOMMONTOOLS_H

#include <iostream>
#include <iomanip>
#include <functional>
#include <string>
#include <exception>

#define STREAMSTRING(STRMSG)        [&]()->std::string{using namespace std; stringstream oStr023974tcnw0re; oStr023974tcnw0re << STRMSG; return oStr023974tcnw0re.str();}()
#define COUTSTRSTR(STRMSG)          std::cout << STREAMSTRING(STRMSG) << std::flush
#define EXCEPTSTR(STRMSG)           STREAMSTRING(__FUNCTION__ << " " << STRMSG)
#define FORMATINTEGER(WIDTH, FILLED)      std::setw(WIDTH) << std::setfill(FILLED)
#define FI(WIDTH, FILLED)           FORMATINTEGER(WIDTH, FILLED)
#define FORMATFLOAT(WIDTH, PRESICISION, FILLED)      std::fixed << std::setw(WIDTH) << std::setprecision(PRESICISION) << std::setfill(FILLED)
#define FF(WIDTH, PRESICISION, FILLED)           FORMATFLOAT(WIDTH, PRESICISION, FILLED)
#define LOGSTR(STRFUNCTION, STRMSG) STREAMSTRING(STRFUNCTION << " " << STRMSG)
#define LOG_ERROR(STRMSG)           std::cerr << "  ERROR: " << LOGSTR(__FUNCTION__, STRMSG) << std::endl
#define LOG_FATAL(STRMSG)           std::cerr << "  FATAL: " << LOGSTR(__FUNCTION__, STRMSG) << std::endl
#define LOG_WARNING(STRMSG)         std::cerr << "WARNING: " << LOGSTR(__FUNCTION__, STRMSG) << std::endl
#define LOG_DEBUG(STRMSG)           std::cerr << "  DEBUG: " << LOGSTR(__FUNCTION__, STRMSG) << std::endl
#define LOG_TRACE(STRMSG)           std::cerr << "  TRACE: " << LOGSTR(__FUNCTION__, STRMSG) << std::endl

namespace nTimeTypes{
  typedef long long MilliSecs;
}

class cCommonTools
{
public:
  static void Sleep(nTimeTypes::MilliSecs p_oTimeMS);
private:

};

#endif	/* CCOMMONTOOLS_H */

