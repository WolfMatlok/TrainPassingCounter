/* 
 * File:   cCamera.cpp
 * Author: juia
 * 
 * Created on 17. April 2016, 16:55
 */

#include <stdexcept>
#include <opencv2/imgproc.hpp>
#include "cCamera.h"
#include "cCommonTools.h"

cCamera::cCamera()
{
  //set camera params
  m_oCamera.set(CV_CAP_PROP_FORMAT, CV_8UC1);

  //Open camera
  LOG_TRACE("Opening Camera...");
  if (!m_oCamera.open())
  {
    LOG_FATAL("Error opening the camera");
    throw std::runtime_error(EXCEPTSTR("Error opening the camera"));
  }
}

cCamera::~cCamera()
{
  LOG_TRACE("Stop camera...");
  m_oCamera.release();
}

void cCamera::getImage(cv::Mat& p_oImage)
{
  m_oCamera.grab();
  m_oCamera.retrieve(p_oImage);
}