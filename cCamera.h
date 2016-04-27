/* 
 * File:   cCamera.h
 * Author: juia
 *
 * Created on 17. April 2016, 16:55
 */

#ifndef CCAMERA_H
#define	CCAMERA_H

#include <raspicam/raspicam_cv.h>



class cCamera
{
public:
  cCamera();
  virtual ~cCamera();
  
  void getImage(cv::Mat& p_oImage);
  
private:

  raspicam::RaspiCam_Cv m_oCamera;

};

#endif	/* CCAMERA_H */

