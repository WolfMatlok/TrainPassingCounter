/* 
 * File:   FFTAnalyser.cpp
 * Author: wmk
 * 
 * Created on 23. Januar 2017, 16:52
 */

#include "cCommonTools.h"
#include <eigen3/unsupported/Eigen/FFT>
#include <iosfwd>
#include <sstream>
#include <bits/basic_ios.h>

#include "FFTAnalyser.h"

using namespace helper;

FFTAnalyser::FFTAnalyser()
: m_maxSampels(10000)
, m_samples(m_maxSampels)
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
  auto samplingFrequencyHZ = N / deltaTSec.count();
  auto frequencyResolution = samplingFrequencyHZ/N;
  

  Eigen::FFT<double> oFFT;
  //oFFT.SetFlag(Eigen::FFT<double>::Flag::HalfSpectrum);
  
  std::vector< std::complex<double> > samplesComplex;

  oFFT.fwd(samplesComplex, samplesOverTime);

  std::stringstream oStrStr;
  oStrStr << "frequency;fft.magnitude;fft.real;fft.imag" << std::endl;
  oStrStr.imbue(std::locale(""));

  auto idxAmplitudeSpectrum = getFrequencyIndex(N, frequencyResolution);
  auto idxAmplitudeSpectrumIt = idxAmplitudeSpectrum.begin();
  for (auto oItComplex = samplesComplex.begin(); oItComplex != samplesComplex.end(); ++oItComplex)
  {
    oStrStr << *(idxAmplitudeSpectrumIt++) << ";" << std::abs(*oItComplex) << ";" << oItComplex->real() << ";" << oItComplex->imag() << std::endl;
  }

  cCommonTools::writeFile(STMSTR("/home/pi/images/fftdata." << samplingFrequencyHZ << ".csv"), oStrStr.str());
  //LOG_TRACE("written to: /home/pi/images/fftdata." << std::setw(10) << std::setfill('0') << ".csv");

}

std::vector<double> FFTAnalyser::getSamples()
{
  std::vector<double> vecOverTime(m_samples.size());
  auto samplesIt = m_samples.begin();
  std::generate(vecOverTime.begin(), vecOverTime.end(), [&](){return (*samplesIt++).m_sample;});
  return std::move(vecOverTime);
}

std::vector<double> FFTAnalyser::getFrequencyIndex(double N, double frequencyResolution)
{
  std::vector<double> idxAmplitudeSpectrum;
  for(double idx = -(N/2); idx<=(N/2)-1; idx++)
    idxAmplitudeSpectrum.push_back(idx*frequencyResolution);
  
  return std::move(idxAmplitudeSpectrum);
}
