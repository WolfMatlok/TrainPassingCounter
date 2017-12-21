/* 
 * File:   FFTAnalyserResultHandler.h
 * Author: wmk
 *
 * Created on 11. Dezember 2017, 17:48
 */
#include <tr1/memory>
#include "cCommonTools.h"

#pragma once

class FFTAnalyserResultHandler
{
public:

  FFTAnalyserResultHandler(std::string name) : m_name(name), m_counterProcessSamples(0)
  {
  }
  
  virtual ~FFTAnalyserResultHandler(){}

  virtual FFTAnalyserResultHandler& handleSamplesOverFrequecy(std::vector< std::complex<double> >& samplesOverFrequency, double samplingRate, double nFFT)
  {
    std::stringstream oStrStr;
    FORMATSTREAMFORCSV(oStrStr);
    oStrStr << "frequency;fft.magnitude;fft.real;fft.imag" << std::endl;

    double dFFTIndex = 0;
    for (auto oItComplex = samplesOverFrequency.begin(); oItComplex != samplesOverFrequency.end(); ++oItComplex)
    {
      auto frequency2Print = dFFTIndex * samplingRate / nFFT;
      if (frequency2Print >= 51.)
        break;
      oStrStr << std::fixed << dFFTIndex * samplingRate / nFFT << ";" << std::abs(*oItComplex) << ";" << oItComplex->real() << ";" << oItComplex->imag() << std::endl;
      dFFTIndex += 1.;
    }
    cCommonTools::writeFile(STMSTR("/home/pi/images/fftdata." << FI(8, '0') << m_counterProcessSamples << ".csv"), oStrStr.str());

    return *this;
  }

  virtual FFTAnalyserResultHandler& handleSamplesOverTime(std::vector<double>& samplesOverTime)
  {
    std::stringstream oStrStr;
    FORMATSTREAMFORCSV(oStrStr);
    int idx = 0;
    oStrStr << "idx;singal over time" << std::endl;
    std::for_each(samplesOverTime.begin(), samplesOverTime.end(), [&](double sample)
    {
      oStrStr << idx++ << ";" << sample << std::endl;
    });
    cCommonTools::writeFile(STMSTR("/home/pi/images/signal." << FI(8, '0') << m_counterProcessSamples << ".csv"), oStrStr.str());

    return *this;
  }

public:
  uint32_t m_counterProcessSamples;

private:
  std::string m_name;
};

typedef std::tr1::shared_ptr<FFTAnalyserResultHandler> FFTAnalyserResultHandlerPtr;