/* 
 * File:   MonitorSeismograph.cpp
 * Author: wmk
 * 
 * Created on 2. Dezember 2017, 21:57
 */

#include "MonitorSeismograph.h"
#include "FFTAnalyserResultHandlerBitmap.h"

MonitorSeismograph::MonitorSeismograph(double samplingRateExpectedHz)
: m_analyserFFT(FFTAnalyserResultHandlerPtr(new FFTAnalyserResultHandlerBitmap()))
, m_devGyroscope(0x68)
, m_filterAcceleration(5)
, m_sinoidGenerator(2.0, 13, 0)
, m_periodExpectedForProcessingMS((1000. / samplingRateExpectedHz))
, m_durationLastProcessingStepMS(m_periodExpectedForProcessingMS)
{
}

MonitorSeismograph::~MonitorSeismograph()
{
}

void MonitorSeismograph::processData()
{
  static helper::Age ageProcessing;
  ageProcessing.reset();

  this->_processData();
  //this->_selfTest();

  //*** print result if is time due ***
  if (m_ageForPrinting.GetAgeMS().count() > 1000)
  {
    m_analyserFFT.processSamples();
    m_ageForPrinting.reset();
  }

  m_durationLastProcessingStepMS = ageProcessing.GetAgeMS().count();
}

void MonitorSeismograph::_processData()
{
  //*** acquire data from gyroscope ***
  auto acceleration = m_devGyroscope.getAcceleration().Z;

  m_filterAcceleration.SetValue(acceleration);
  m_analyserFFT.add(acceleration);

  std::cout << "Processing Gyroscope acceleration. LASTSAMPLINGRATE:" << FF(8, 4, '0') << m_analyserFFT.getLastSamplingRate() << " CURRVAL:" << FF(8, 4, '0') << acceleration << " AVG:" << FF(8, 4, '0') << m_filterAcceleration.GetAvg() << " MEDIAN:" << FF(8, 4, '0') << m_filterAcceleration.GetMedian() << " " << cCommonTools::ROTATECURSOR() << "\r";

}

void MonitorSeismograph::_selfTest()
{
  auto valueCurrent = m_sinoidGenerator.getValue();
  m_analyserFFT.add(valueCurrent);

  std::cout << "Selftest with own sinoid. LASTSAMPLINGRATE:" << m_analyserFFT.getLastSamplingRate() << " VALUE:" << FFVALUE(valueCurrent) << cCommonTools::ROTATECURSOR() << "\r";
}

void MonitorSeismograph::waitForNextProcessingIteration(nTimeTypes::MilliSecs timeToReduceInternalWaitTime /*=0*/)
{
  nTimeTypes::MilliSecs sleepTimeMS = m_periodExpectedForProcessingMS;
  timeToReduceInternalWaitTime += m_durationLastProcessingStepMS;

  /** reduce the sleepingtime by the time used to calculate fft and process fft results*/
  /** and **/
  /** reduce the sleeping by the external given correction */
  if (sleepTimeMS >= timeToReduceInternalWaitTime && timeToReduceInternalWaitTime >= 1)
  {
    sleepTimeMS -= timeToReduceInternalWaitTime;
  } 
  else if (timeToReduceInternalWaitTime >= sleepTimeMS)
  {
    sleepTimeMS = 0;
  }

  cCommonTools::Sleep(sleepTimeMS);
}
