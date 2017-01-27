/* 
 * File:   FFTAnalyser.h
 * Author: wmk
 *
 * Created on 23. Januar 2017, 16:52
 */

#pragma once

#include "TimeServer.h"
#include "TimeServerClock.h"

class FFTAnalyser
{
public:

  FFTAnalyser(uint32_t p_maxSamples);

  virtual ~FFTAnalyser();;

  bool add(double p_sample, helper::TimeServerUnix::ms&& p_timestamp);
  bool add(double p_sample);

  void processSamples();
  void processSamples(helper::TimeServerUnix::secAsDouble samplingDuration);

private:

  const uint32_t m_maxSampels;

  struct sample
  {
    double m_sample;
    helper::TimeServerUnix::ms m_timestamp;
  };

  boost::circular_buffer<sample> m_samples;

};