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
#include <numeric>

#include <boost/date_time/gregorian/gregorian.hpp>
#include <boost/circular_buffer.hpp>


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
  
  static void writeFile( const std::string& p_strFileName, const std::string& p_oBytes );
  
  const static double_t PI;
  const static double_t TODEG;
  const static double_t TORAD;

private:

};

struct AVGDEVCOUNTER2
{
  AVGDEVCOUNTER2(){Reset();};
  
  ~AVGDEVCOUNTER2(){};
  
  void SetValue(double p_dNewValue)
  {
    m_dSum += p_dNewValue;
    m_dSumSquare += (p_dNewValue*p_dNewValue);
    m_dN++;
  };
  
  double GetAvg(){ return m_dSum/m_dN; };
  double GetStdDev(){ return sqrt( (m_dSumSquare/m_dN) - ((m_dSum*m_dSum)/(m_dN*m_dN)) ); };
  
  void Reset(){ m_dN = 1.; m_dSum=0.; m_dSumSquare=0.; };
  
  double m_dN;
  double m_dSum;
  double m_dSumSquare;
};

/**
 * Uses a defined size
 */
template<typename PDT = double>
struct AVGDEVCOUNTER3
{
  AVGDEVCOUNTER3(unsigned int p_uiMaxSize)
  : m_oBufferCircular(p_uiMaxSize)
  , m_oBufferCircularSquare(p_uiMaxSize)
  {
    Reset();
  };
  
  ~AVGDEVCOUNTER3(){};
  
  void SetValue(PDT p_dNewValue)
  {
    m_oBufferCircular.push_back(p_dNewValue);
    m_oBufferCircularSquare.push_back(p_dNewValue*p_dNewValue);
  };
  
  PDT GetAvg()
  {
    PDT dSum = GetSum();
    PDT dN = GetN();
    return dSum/dN;
  };
  
  PDT GetStdDev()
  {
    PDT m_dSum = GetSum();
    PDT m_dSumSquare = GetSumSquare();
    PDT m_dN = GetN();
    return sqrt( (m_dSumSquare/m_dN) - ((m_dSum*m_dSum)/(m_dN*m_dN)) );
  };
  
  PDT GetSum(){return std::accumulate(m_oBufferCircular.begin(), m_oBufferCircular.end(), 0.);};  
  PDT GetSumSquare(){return std::accumulate(m_oBufferCircularSquare.begin(), m_oBufferCircularSquare.end(), 0.);};
  
  void Reset(){ m_oBufferCircular.clear(); m_oBufferCircularSquare.clear(); };
  
  PDT GetN(){return m_oBufferCircular.size();};
  
  bool Full(){return m_oBufferCircular.full();};
  
  boost::circular_buffer<PDT> m_oBufferCircular;
  boost::circular_buffer<PDT> m_oBufferCircularSquare;
};

#endif	/* CCOMMONTOOLS_H */

