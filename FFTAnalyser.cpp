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
: m_maxSampels(150)
, m_samples(m_maxSampels)
{
}

FFTAnalyser::FFTAnalyser(FFTAnalyserResultHandlerPtr handlerResults)
: m_maxSampels(150)
, m_samples(m_maxSampels)
, m_handlerResults(handlerResults)
, m_samplingRate(0)
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
  processSamples(getDurationOverallSamples());
}

void FFTAnalyser::processSamples(helper::TimeServerUnix::secAsDouble samplingDuration)
{
  auto samplesOverTime = getSamples();
  auto&& deltaTSec = samplingDuration;
  auto N = double(samplesOverTime.size());
  m_samplingRate = N / deltaTSec.count();
  auto frequencyResolution = m_samplingRate / N;
  Eigen::FFT<double>::Index nFFT = 256;


  Eigen::FFT<double> oFFT;
  oFFT.SetFlag(Eigen::FFT<double>::Flag::HalfSpectrum);

  std::vector< std::complex<double> > samplesOverFrequency;
  samplesOverFrequency.resize(256);

  oFFT.fwd(&samplesOverFrequency[0], &samplesOverTime[0], nFFT);

  if (nullptr != m_handlerResults && m_handlerResults->validate(samplesOverFrequency, m_samplingRate, double(nFFT)))
  {
    m_handlerResults->handleSamplesOverFrequecy(samplesOverFrequency, m_samplingRate, double(nFFT));
    m_handlerResults->handleSamplesOverTime(samplesOverTime).m_counterProcessSamples++;
    m_handlerResults->generateReport();
  }

}

helper::TimeServerUnix::secAsDouble FFTAnalyser::getDurationOverallSamples()
{
  auto timepointStartMS = m_samples.begin()->m_timestamp;
  auto timepointEndMS = m_samples.rbegin()->m_timestamp;
  return std::move(boost::chrono::duration_cast<TimeServerUnix::secAsDouble>(timepointEndMS - timepointStartMS));
}

double FFTAnalyser::getLastSamplingRate()
{
  return double(m_samples.size()) / double(getDurationOverallSamples().count());
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
