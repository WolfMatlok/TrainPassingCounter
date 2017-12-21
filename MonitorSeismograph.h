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
  MonitorSeismograph();
  virtual ~MonitorSeismograph();
  
  void processData();
  
private:
  cGyroServer m_devGyroscope;
  FILTERSIMPLE<> m_filterAcceleration;
  FFTAnalyser m_analyserFFT;
  helper::Age m_ageForPrinting;

};
