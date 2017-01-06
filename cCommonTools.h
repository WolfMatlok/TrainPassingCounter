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

#include <boost/date_time/gregorian/gregorian.hpp>


#define STREAMSTRING(STRMSG)        [&]()->std::string{using namespace std; stringstream oStr023974tcnw0re; oStr023974tcnw0re << STRMSG; return oStr023974tcnw0re.str();}()
#define STMSTR(STREAMMSG) [&]()->std::string{std::stringstream oStrStr_4AE80E30_FBCE_4FCC_A15C_D9F26679E350; oStrStr_4AE80E30_FBCE_4FCC_A15C_D9F26679E350 << STREAMMSG; return oStrStr_4AE80E30_FBCE_4FCC_A15C_D9F26679E350.str();}()
#define COUTSTRSTR(STRMSG)          std::cout << STREAMSTRING(STRMSG) << std::flush
#define EXCEPTSTR(STRMSG)           STREAMSTRING(__FUNCTION__ << " " << STRMSG)
#define FORMATINTEGER(WIDTH, FILLED)      std::setw(WIDTH) << std::setfill(FILLED)
#define FI(WIDTH, FILLED)           FORMATINTEGER(WIDTH, FILLED)
#define FORMATFLOAT(WIDTH, PRESICISION, FILLED)      std::fixed << std::setw(WIDTH) << std::setprecision(PRESICISION) << std::setfill(FILLED)
#define FF(WIDTH, PRESICISION, FILLED)           FORMATFLOAT(WIDTH, PRESICISION, FILLED)
#define LOGSTR(STRFUNCTION, STRMSG) STREAMSTRING(STRFUNCTION << " " << STRMSG)
#define LOG_ERROR(STRMSG)           std::cout << "  ERROR: " << LOGSTR(__FUNCTION__, STRMSG) << std::endl
#define LOG_FATAL(STRMSG)           std::cout << "  FATAL: " << LOGSTR(__FUNCTION__, STRMSG) << std::endl
#define LOG_WARNING(STRMSG)         std::cout << "WARNING: " << LOGSTR(__FUNCTION__, STRMSG) << std::endl
#define LOG_DEBUG(STRMSG)           std::cout << "  DEBUG: " << LOGSTR(__FUNCTION__, STRMSG) << std::endl
#define LOG_TRACE(STRMSG)           std::cout << "  TRACE: " << LOGSTR(__FUNCTION__, STRMSG) << std::endl

#define EPOCHE_START_2014_1_1 boost::gregorian::date(2014,1,1)
#define EPOCHE_START_1970_1_1 boost::gregorian::date(1970,1,1)


namespace nTimeTypes
{
  typedef long long MilliSecs;
}

class cCommonTools
{
public:
  static int _kbhit();
  static void Sleep(nTimeTypes::MilliSecs p_oTimeMS);

  static bool Break(const char p_cKey);

  /** defines epoche start for all internal used timestamps
   * default EPOCHE_START_2014_1_1
   * @author wmk
   * @date 2016/03/23
   */
  const static boost::gregorian::date m_oEpocheStart;


  static nTimeTypes::MilliSecs GetCurrentTimeStamp();

private:

};

#endif	/* CCOMMONTOOLS_H */

