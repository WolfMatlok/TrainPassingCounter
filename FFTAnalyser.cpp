/* 
 * File:   FFTAnalyser.cpp
 * Author: wmk
 * 
 * Created on 23. Januar 2017, 16:52
 */

#include "cCommonTools.h"
#include <iostream>
#include <eigen3/unsupported/Eigen/FFT>
#include <Eigen/Dense>
#include <iosfwd>
#include <sstream>
#include <bits/basic_ios.h>

#include "FFTAnalyser.h"

using namespace helper;

FFTAnalyser::FFTAnalyser()
: m_maxSampels(1000)
, m_samples(m_maxSampels)
, m_counterProcessSamples(0)
{
}

FFTAnalyser::~FFTAnalyser()
{
}

bool FFTAnalyser::add(double p_sample, TimeServerUnix::ms&& p_timestamp)
{
  m_samples.push_back(sample{p_sample, p_timestamp});
  return !m_samples.full();
}

bool FFTAnalyser::add(double p_sample)
{
  return add(p_sample, std::move(TimeServerUnix::getTimestampCurrent()));
}

void FFTAnalyser::processSamples()
{
  auto timepointStartMS = m_samples.begin()->m_timestamp;
  auto timepointEndMS = m_samples.rbegin()->m_timestamp;
  auto deltaTSec = boost::chrono::duration_cast<TimeServerUnix::secAsDouble>(timepointEndMS - timepointStartMS);
  processSamples(deltaTSec);
}

void FFTAnalyser::processSamples(helper::TimeServerUnix::secAsDouble samplingDuration)
{
  auto samplesOverTime = getSamples();
  auto&& deltaTSec = samplingDuration;
  auto N = double(samplesOverTime.size());
  auto samplingRate = N / deltaTSec.count();
  auto frequencyResolution = samplingRate / N;
  Eigen::FFT<double>::Index nFFT = 256;


  Eigen::FFT<double> oFFT;
  oFFT.SetFlag(Eigen::FFT<double>::Flag::HalfSpectrum);

  std::vector< std::complex<double> > samplesComplex;
  samplesComplex.resize( 256 );

  oFFT.fwd(&samplesComplex[0], &samplesOverTime[0], nFFT);

  {
    std::stringstream oStrStr;
    FORMATSTREAMFORCSV(oStrStr);
    oStrStr << "frequency;fft.magnitude;fft.real;fft.imag" << std::endl;

    double dFFTIndex = 0;
    for (auto oItComplex = samplesComplex.begin(); oItComplex != samplesComplex.end(); ++oItComplex)
    {
      auto frequency2Print = dFFTIndex * samplingRate / double(nFFT);
      if(frequency2Print>=51.)
        break;
      oStrStr << std::fixed << dFFTIndex * samplingRate / double(nFFT) << ";" << std::abs(*oItComplex) << ";" << oItComplex->real() << ";" << oItComplex->imag() << std::endl;      
      dFFTIndex+=1.;
    }
    cCommonTools::writeFile(STMSTR("/home/pi/images/fftdata."<< FI(8,'0') << m_counterProcessSamples <<".csv"), oStrStr.str());
  }


  {
    std::stringstream oStrStr; FORMATSTREAMFORCSV(oStrStr);
    int idx = 0;
    oStrStr << "idx;singal over time" << std::endl;
    std::for_each(samplesOverTime.begin(), samplesOverTime.end(), [&](double sample)
    {
      oStrStr << idx++ << ";" << sample << std::endl;
    });
    cCommonTools::writeFile(STMSTR("/home/pi/images/signal."<< FI(8,'0') << m_counterProcessSamples <<".csv"), oStrStr.str());
  }
  
  m_counterProcessSamples++;

}

std::vector<double> FFTAnalyser::getSamples()
{
  std::vector<double> vecOverTime(m_samples.size());
  auto samplesIt = m_samples.begin();
  std::generate(vecOverTime.begin(), vecOverTime.end(), [&]()
  {
    return (*samplesIt++).m_sample;
  });
  return std::move(vecOverTime);
}

std::vector<double> FFTAnalyser::getFrequencyIndex(double N, double frequencyResolution)
{
  std::vector<double> idxAmplitudeSpectrum;
  for (double idx = -(N / 2); idx <= (N / 2) - 1; idx++)
    idxAmplitudeSpectrum.push_back(idx * frequencyResolution);

  return std::move(idxAmplitudeSpectrum);
}
