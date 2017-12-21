/* 
 * File:   FFTAnalyserResultHandlerBitmap.h
 * Author: wmk
 *
 * Created on 21. Dezember 2017, 18:57
 */

#include "FFTAnalyserResultHandler.h"

#pragma once

class FFTAnalyserResultHandlerBitmap : public FFTAnalyserResultHandler
{
public:
  FFTAnalyserResultHandlerBitmap():FFTAnalyserResultHandler("FFTAnalyserResultHandlerBitmap"){}
  virtual ~FFTAnalyserResultHandlerBitmap(){}
  
  virtual FFTAnalyserResultHandler& handleSamplesOverFrequecy(std::vector< std::complex<double> >& samplesOverFrequency, double samplingRate, double nFFT) override
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

  virtual FFTAnalyserResultHandler& handleSamplesOverTime(std::vector<double>& samplesOverTime) override
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
  
  private:
  
};
