/* 
 * File:   MonitorSeismograph.h
 * Author: wmk
 *
 * Created on 2. Dezember 2017, 21:57
 */

#pragma once

#include "cCommonTools.h"
#include "cGyroServer.h"
#include "FFTAnalyser.h"
#include "Age.h"

class MonitorSeismograph
{
public:
  MonitorSeismograph(double samplingRateExpectedHz = 100.);
  virtual ~MonitorSeismograph();

  void processData();  

  void waitForNextProcessingIteration(nTimeTypes::MilliSecs timeToReduceInternalWaitTime = 0);

private:
  cGyroServer m_devGyroscope;
  FILTERSIMPLE<> m_filterAcceleration;
  FFTAnalyser m_analyserFFT;
  helper::Age m_ageForPrinting;
  nTimeTypes::MilliSecs m_durationLastProcessingStepMS;
  const nTimeTypes::MilliSecs m_periodExpectedForProcessingMS;

  void _processData();
  void _selfTest();
  
  /**
   * realtime sinoid generator for selftesting
   */
  struct SinoidGenerator
  {

    SinoidGenerator(double amplitude, double frequencyHZ, double base)
    : m_amplitude(amplitude)
    , m_frequencyHZ(frequencyHZ)
    , m_base(base)
    {
    }

    double getValue()
    {
      return m_base + (m_amplitude * std::cos(m_frequencyHZ * m_age.GetAgeS().count() * (2. * cCommonTools::PI)));
    }

  private:
    helper::Age m_age;
    double m_base;
    double m_amplitude;
    double m_frequencyHZ;
  };

  /**
   * For selftest.
   */
  SinoidGenerator m_sinoidGenerator;

};
