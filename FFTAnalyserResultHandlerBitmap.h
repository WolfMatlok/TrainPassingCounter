/* 
 * File:   FFTAnalyserResultHandlerBitmap.h
 * Author: wmk
 *
 * Created on 21. Dezember 2017, 18:57
 */

#include "FFTAnalyserResultHandler.h"
#include <cv.h>
#include <highgui.h>
#include "opencv2/imgproc/imgproc.hpp"

#pragma once

class FFTAnalyserResultHandlerBitmap : public FFTAnalyserResultHandler
{
public:
  FFTAnalyserResultHandlerBitmap();

  virtual ~FFTAnalyserResultHandlerBitmap();

  virtual FFTAnalyserResultHandler& handleSamplesOverFrequecy(std::vector< std::complex<double> >& samplesOverFrequency, double samplingRate, double nFFT) override;

  /**
   * expects samples within range [-2;+2]
   */
  virtual FFTAnalyserResultHandler& handleSamplesOverTime(std::vector<double>& samplesOverTime) override;
  
  virtual bool validate(std::vector< std::complex<double> >& samplesOverFrequency, double samplingRate, double nFFT) override;
  

  virtual void generateReport() override;

   
private:
  
  unsigned int getCounter();
    
  cv::Mat m_imgRaw;
  cv::Mat m_imgFrequency;
  cv::Mat m_imgSamplesOverTime;
  
};

typedef std::tr1::shared_ptr<FFTAnalyserResultHandlerBitmap> FFTAnalyserResultHandlerBitmapPtr;
