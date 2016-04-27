/* 
 * File:   cUnitTest.cpp
 * Author: juia
 * 
 * Created on 17. April 2016, 17:15
 */

#include "cUnitTest.h"
#include "cCamera.h"
#include "cCommonTools.h"
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace std;

void cUnitTest::DoAllTests()
{
  CheckFPSCamera();
  CheckDifference();
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
    std::string strImgName = STREAMSTRING("/home/pi/images/raspicam_cv_image_" << FI(4,'0') <<  dN <<  ".jpg");
    cv::imwrite(strImgName, oImageDiff);
    LOG_TRACE("Image saved to " << strImgName);
    
    //cv::minMaxLoc(oImageDiff, &dMin, &dMax);
    LOG_TRACE("Progress:" << FF(3, 3,'0') << dN/2000 << "% Min:" << FI(3,'0') << dMin << " Max:" << FI(3,'0') << dMax);

    
  }
}
