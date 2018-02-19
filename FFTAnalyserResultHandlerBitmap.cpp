/* 
 * File:   FFTAnalyserResultHandlerBitmap.cpp
 * Author: wmk
 * 
 * Created on 21. Dezember 2017, 18:57
 */

#include "FFTAnalyserResultHandlerBitmap.h"

FFTAnalyserResultHandlerBitmap::FFTAnalyserResultHandlerBitmap()
: FFTAnalyserResultHandler("FFTAnalyserResultHandlerBitmap")
, m_imgRaw            {120, 500, CV_8UC1, cv::Scalar(0)}
, m_imgFrequency      { m_imgRaw(cv::Rect(      9, 9, 256, 100))}
, m_imgSamplesOverTime{ m_imgRaw(cv::Rect(256 + 9, 9, 200, 100))}
{
}

FFTAnalyserResultHandlerBitmap::~FFTAnalyserResultHandlerBitmap()
{
}

FFTAnalyserResultHandler& FFTAnalyserResultHandlerBitmap::handleSamplesOverFrequecy(std::vector< std::complex<double> >& samplesOverFrequency, double samplingRate, double nFFT)
{
  using namespace cv;

  m_imgFrequency = Scalar(0);
  
  double idxXFFT = 0;
  unsigned int magnitude = 0;
  for (auto oItComplex = samplesOverFrequency.begin(); oItComplex != samplesOverFrequency.end(); ++oItComplex)
  {
    //auto frequency2Print = idxXFFT * samplingRate / nFFT;
    magnitude = std::abs(*oItComplex);

    if (magnitude >= 100)
    {
      m_imgFrequency.at<char>(99, idxXFFT) = 255; //frequency2Print << ";" << amplitude
    }
    else
    {
      m_imgFrequency.at<char>(magnitude, idxXFFT) = 255; //frequency2Print << ";" << amplitude
    }

    idxXFFT += 1.0;

    if (idxXFFT > 255.) break;
  }

  flip(m_imgFrequency, m_imgFrequency, 0);

  return *this;
}

FFTAnalyserResultHandler& FFTAnalyserResultHandlerBitmap::handleSamplesOverTime(std::vector<double>& samplesOverTime)
{
  using namespace cv;

  m_imgSamplesOverTime = Scalar(0);

  unsigned int idxX = 0;
  unsigned int idxY = 0;
  std::for_each(samplesOverTime.begin(), samplesOverTime.end(), [&](double sample)
  {
    idxY = (sample + 2.0)*100. / 4.;

    idxY = idxY >= 100 ? 99 : idxY;

    if (idxX < 200)
    {
      m_imgSamplesOverTime.at<char>(idxY, idxX) = 255;
    }

    idxX++;
  });

  flip(m_imgSamplesOverTime, m_imgSamplesOverTime, 0);  

  return *this;
}

bool FFTAnalyserResultHandlerBitmap::validate(std::vector< std::complex<double> >& samplesOverFrequency, double samplingRate, double nFFT)
{
  bool valid = true;
  unsigned int idxXFFT = 0;

  for (auto oItComplex = samplesOverFrequency.begin(); oItComplex != samplesOverFrequency.end(); ++oItComplex)
  {
    auto frequency = idxXFFT * samplingRate / nFFT;
    unsigned int magnitude = std::abs(*oItComplex);

    bool goodFrequency = ((1. <= frequency && frequency <= 50.) && (10. <= magnitude && magnitude <= 1000.));
    valid |= goodFrequency;

    idxXFFT += 1.0;
  }

  return valid;
}

void FFTAnalyserResultHandlerBitmap::generateReport()
{
  cv::imwrite(STMSTR("/home/pi/images/" << FI(8, '0') << getCounter() << ".report.jpg"), m_imgRaw);
}

unsigned int FFTAnalyserResultHandlerBitmap::getCounter()
{
  return m_counterProcessSamples % 3600;
}

