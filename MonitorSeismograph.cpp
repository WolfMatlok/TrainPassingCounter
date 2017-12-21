/* 
 * File:   MonitorSeismograph.cpp
 * Author: wmk
 * 
 * Created on 2. Dezember 2017, 21:57
 */

#include "MonitorSeismograph.h"

MonitorSeismograph::MonitorSeismograph()
: m_analyserFFT()
, m_devGyroscope(0x68)
, m_filterAcceleration(5)
{
}

MonitorSeismograph::~MonitorSeismograph()
{
}

void MonitorSeismograph::processData()
{
  //*** acquire data from gyroscope ***
  auto acceleration = m_devGyroscope.getAcceleration();
  

  m_analyserFFT.add(m_filterAcceleration.SetValue(acceleration.X).GetMedian());
  std::cout << "Analysing Gyroscope acceleration. AVG:" << FFVALUE(m_filterAcceleration.GetAvg()) << " MEDIAN:" << FFVALUE(m_filterAcceleration.GetMedian()) << " " << cCommonTools::ROTATECURSOR() << "\r";
  

  //*** print result if is time due ***
  if (m_ageForPrinting.GetAgeMS().count() > 1000)
  {
    m_analyserFFT.processSamples();
    m_ageForPrinting.reset();
  }

}