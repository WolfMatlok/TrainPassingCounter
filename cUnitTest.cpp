/* 
 * File:   cUnitTest.cpp
 * Author: juia
 * 
 * Created on 17. April 2016, 17:15
 */

#include "cUnitTest.h"
#include "cGyroServer.h"
#include "cCamera.h"
#include "cCommonTools.h"
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include "TimeServer.h"
#include "TimeServerClock.h"
#include "FFTAnalyser.h"
#include "Age.h"
#include "MonitorSeismograph.h"

using namespace std;

void cUnitTest::DoAllTests()
{
  //CheckFFT();
  //CheckTimeServer();
  //CheckGyroSensor();
  //CheckFPSCamera();
  //CheckDifference();
  CheckGyroHandler();
}

void cUnitTest::CheckFPSCamera()
{
  time_t oTimeBegin, oTimeEnd;

  cCamera oCamera;
  cv::Mat oImage;
  int iCount = 100;

  //Start capture
  LOG_TRACE("Capturing " << iCount << " frames ....");
  time(&oTimeBegin);
  for (int i = 0; i < iCount; i++)
  {
    oCamera.getImage(oImage);
    if (i % 5 == 0) cout << "\r captured " << i << " images" << std::flush;
  }

  cout << std::endl;

  //show time statistics
  time(&oTimeEnd); /* get current time; same as: timer = time(NULL)  */
  double secondsElapsed = difftime(oTimeEnd, oTimeBegin);
  LOG_TRACE(secondsElapsed << " seconds for " << iCount << "  frames : FPS = " << (float) ((float) (iCount) / secondsElapsed));

}

void cUnitTest::CheckDifference()
{
  cv::Mat oImageA;
  cv::Mat oImageB;
  cv::Mat oImageDiff;
  double dMin, dMax;

  cCamera oCam;

  oCam.getImage(oImageA);


  for (double dN = 0; dN <= 1000.; dN++)
  {
    oCam.getImage(oImageB);

    cv::absdiff(oImageA, oImageB, oImageDiff);


    cv::threshold(oImageDiff, oImageDiff, 80, 255, cv::THRESH_BINARY);
    cv::erode(oImageDiff, oImageDiff, cv::getStructuringElement(cv::MORPH_RECT, cv::Size(3, 3)));

    //save image 
    std::string strImgName = STREAMSTRING("/home/pi/images/raspicam_cv_image_" << FI(4, '0') << dN << ".jpg");
    cv::imwrite(strImgName, oImageDiff);
    //LOG_TRACE("Image saved to " << strImgName);

    cv::minMaxLoc(oImageDiff, &dMin, &dMax);
    std::cout << "\r Progress:" << FF(3, 3, '0') << dN / 10. << "% Min:" << FI(3, '0') << dMin << " Max:" << FI(3, '0') << dMax << std::flush;


  }

  std::cout << std::endl;
}

void cUnitTest::CheckGyroSensor()
{
  cGyroServer oGyroDev(0x68);

  //for(int iN = 1; iN <=10000; ++iN)
  while (true)
  {
    oGyroDev.processData();
  }

}

boost::gregorian::date EPOCHE_1_1_2017 = boost::gregorian::date(2017, 1, 1);
boost::gregorian::date EPOCHE_1_1_2018 = boost::gregorian::date(2018, 1, 1);

void cUnitTest::CheckTimeServer()
{
  using namespace helper;
  auto nowTSUnix = TimeServerUnix::getTimestampCurrent();
  auto strTSUNIX = TimeServerUnix::getFormattedTime(nowTSUnix);
  auto nowTPUnix = TimeServerUnix::getTimepointCurrent();
  auto strTPUNIX = TimeServerUnix::getFormattedTime(nowTPUnix);

  auto nowTSNT = TimeServerNT::getTimestampCurrent();
  auto strTSNT = TimeServerNT::getFormattedTime(nowTSNT);
  auto nowTPNT = TimeServerNT::getTimepointCurrent();
  auto strTPNT = TimeServerNT::getFormattedTime(nowTPNT);


  typedef helper::TimeServer<&EPOCHE_1_1_2017> TimeServerSince20170101;
  auto nowTS20170101 = TimeServerSince20170101::getTimestampCurrent();
  auto strTS20170101 = TimeServerSince20170101::getFormattedTime(nowTS20170101);
  auto nowTP20170101 = TimeServerSince20170101::getTimepointCurrent();
  auto strTP20170101 = TimeServerSince20170101::getFormattedTime(nowTP20170101);


  typedef helper::TimeServer<&EPOCHE_1_1_2018> TimeServerSince20180101;
  auto nowTS20180101 = TimeServerSince20180101::getTimestampCurrent();
  auto strTS20180101 = TimeServerSince20180101::getFormattedTime(nowTS20180101);
  auto nowTP20180101 = TimeServerSince20180101::getTimepointCurrent();
  auto strTP20180101 = TimeServerSince20180101::getFormattedTime(nowTP20180101);
}

void cUnitTest::CheckFFT()
{
  double N = 1000., NCounter = 0.;
  double samplingRateHZ = 100.;
  double samplingPeriodMS = 1000./samplingRateHZ;
  std::vector<double> vecOverTime(N);
  
  double dPeriodPercent = 0.0;
  std::generate(vecOverTime.begin(), vecOverTime.end(), [&](){
    
    double ret =    std::cos(  3. * dPeriodPercent * (2.*cCommonTools::PI) )
                  + std::cos(  5. * dPeriodPercent * (2.*cCommonTools::PI) )
                  + std::cos( 10. * dPeriodPercent * (2.*cCommonTools::PI) )
                  + std::cos( 20. * dPeriodPercent * (2.*cCommonTools::PI) )
                  + std::cos( 30. * dPeriodPercent * (2.*cCommonTools::PI) )
                  + std::cos( 40. * dPeriodPercent * (2.*cCommonTools::PI) )
                  + std::cos( 50. * dPeriodPercent * (2.*cCommonTools::PI) );
    
    dPeriodPercent+=(samplingPeriodMS/1000.);   
    
    if(dPeriodPercent >= 100.)
      dPeriodPercent = 0.;    
    
    return ret;
  });

  FFTAnalyser analyserFFT;
  AVGDEVCOUNTER3<> avgOfCycle(vecOverTime.size());
  std::for_each(vecOverTime.begin(), vecOverTime.end(), [&](double sample)
  {
    helper::Age ageOfCycle;
    analyserFFT.add(sample);
    cCommonTools::Sleep(samplingPeriodMS);
    std::cout << "Generating sample data for unittest " << FF(6,2,'0') << 100.*(NCounter++/N)  << "% " <<  cCommonTools::ROTATECURSOR() << "\r";
    avgOfCycle.SetValue( ageOfCycle.GetAgeMS(true).count() );
  });
  
  std::cout << std::endl << "avgSamplingRate:" << avgOfCycle.GetAvg() << " stddevSamplingrate:" << avgOfCycle.GetStdDev() << std::endl;
  
  analyserFFT.processSamples();
  
  return;
}

void cUnitTest::CheckGyroHandler()
{
  MonitorSeismograph monSeismograph;
  
  helper::Age ageTest;
  helper::Age ageCylcle;
  while(ageTest.GetAgeS().count() <= 10 /*43200*/) //12 Stunden
  {
    monSeismograph.processData();
    cCommonTools::Sleep( 10 - ageCylcle.GetAgeMS(true).count());
  }
}