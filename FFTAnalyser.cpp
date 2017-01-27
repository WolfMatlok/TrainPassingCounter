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

FFTAnalyser::FFTAnalyser(uint32_t p_maxSamples)
: m_maxSampels(p_maxSamples)
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
  if(0==m_samples.size())
  {
    return;
  }
  
  auto timepointStartMS = m_samples.begin()->m_timestamp;
  auto timepointEndMS = m_samples.rbegin()->m_timestamp;
  auto deltaTSec = boost::chrono::duration_cast<TimeServerUnix::secAsDouble>(timepointEndMS - timepointStartMS);
  processSamples(deltaTSec);
}

void FFTAnalyser::processSamples(helper::TimeServerUnix::secAsDouble samplingDuration)
{
  if(0==m_samples.size())
  {
    return;
  }
  
  auto deltaTSec = samplingDuration;
  auto samplesPerSecond = double(m_samples.size()) / deltaTSec.count();
  auto maxFrequencyByFFT = samplesPerSecond/2.;

  Eigen::FFT<double> oFFT;
  oFFT.SetFlag(Eigen::FFT<double>::Flag::HalfSpectrum);

  std::vector<double> vecOverTime(m_samples.size());
  auto samples = m_samples.begin();
  std::generate(vecOverTime.begin(), vecOverTime.end(), [&](){return (*samples++).m_sample;});
  
  std::vector< std::complex<double> > vecFrequency;

  oFFT.fwd(vecFrequency, vecOverTime);

  std::stringstream oStrStr;
  oStrStr << "frequency;fft.magnitude;fft.real;fft.imag" << std::endl;
  oStrStr.imbue(std::locale(""));

  auto iFrequency = 0;
  for (auto oItComplex = vecFrequency.begin(); oItComplex != vecFrequency.end(); ++oItComplex)
  {
    oStrStr << double(iFrequency++) / maxFrequencyByFFT << ";" << std::abs(*oItComplex) << ";" << oItComplex->real() << ";" << oItComplex->imag() << std::endl;
  }

  cCommonTools::writeFile(STMSTR("/home/pi/images/fftdata." << samplesPerSecond << ".csv"), oStrStr.str());
  //LOG_TRACE("written to: /home/pi/images/fftdata." << std::setw(10) << std::setfill('0') << ".csv");

}
