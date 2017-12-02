/* 
 * File:   MonitorSeismograph.cpp
 * Author: wmk
 * 
 * Created on 2. Dezember 2017, 21:57
 */

#include "MonitorSeismograph.h"

MonitorSeismograph::MonitorSeismograph()
: m_devGyroscope(0x68)
{
}

MonitorSeismograph::~MonitorSeismograph()
{
}

void MonitorSeismograph::processData()
{
  //*** acquire data from gyroscope ***
  auto acceleration = m_devGyroscope.getAcceleration();
  m_diffCounter_Acc.SetValue(acceleration.X);

  //*** feed fft if collected data is adequate ***
  if (m_diffCounter_Acc.m_counter >= 2)
  {
    m_analyserFFT.add(m_diffCounter_Acc.GetDiff());
    std::cout << "Analysing Gyroscope acceleration:" << FF(6,2,'0') << m_diffCounter_Acc.GetDiff()  << " " <<  cCommonTools::ROTATECURSOR() << "\r";
  }

  //*** print result if is time due ***
  if (m_ageForPrinting.GetAgeMS().count() > 1000)
  {
    m_analyserFFT.processSamples();
    m_ageForPrinting.reset();
  }

}