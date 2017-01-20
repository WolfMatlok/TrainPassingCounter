/* 
 * File:   cGyroServer.h
 * Author: wmk
 *
 * Created on 6. Januar 2017, 16:19
 */

#pragma once

#include <tuple>

#include "cCommonTools.h"

class cGyroServer
{
public:
  cGyroServer(int p_iAdress = 0x68);
  virtual ~cGyroServer();
  
  void processData();
  
private:

  int m_iFilehandleGyroDev;
  
  int getWord(int p_iAdress);
  void setWord(int p_iAdress, uint8_t p_ucWord, nTimeTypes::MilliSecs p_oWaitTimeAfterSetting = 0);
  
  double getXRotation(double p_dX, double p_dY, double p_dZ);
  double getYRotation(double p_dX, double p_dY, double p_dZ);
  
  double getDist(double p_dA,double p_dB);  
  
  double getGyroFactoryTrimValue();
  
};