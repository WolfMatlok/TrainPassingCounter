/* 
 * File:   cGyroServer.cpp
 * Author: wmk
 * 
 * Created on 6. Januar 2017, 16:19
 */

#include "cGyroServer.h"
#include <cmath>
#include <exception>
#include <wiringPiI2C.h>
#include <eigen3/unsupported/Eigen/FFT>

cGyroServer::cGyroServer(int p_iAdress)
{
  m_iFilehandleGyroDev = wiringPiI2CSetup(p_iAdress);
  if (-1 == m_iFilehandleGyroDev)
  {
    throw std::runtime_error("cGyroServer::cGyroServer() Can't setup i2c!");
  }
  
//  setWord(0x6B, 0x80, 100); //reset al registers to default values
//  setWord(0x68, 0x07, 100); //reset signal paths to their power up configurations
//  setWord(0x6C, 0xc0, 100); //define the frequency of internal register updating
  setWord(0x6B, 0x00, 100); //set the clock source (this starts the device)
}

cGyroServer::~cGyroServer()
{
}

void cGyroServer::processData()
{
  static AVGDEVCOUNTER3<> oAvgTemp(20);
  static AVGDEVCOUNTER3<> oAvgX(10);
  static AVGDEVCOUNTER3<> oAvgY(10);
  static AVGDEVCOUNTER3<> oAvgAccelX(10);
  static AVGDEVCOUNTER3<> oAvgAccelY(10);
  static AVGDEVCOUNTER3<> oAvgAccelZ(10);
  static AVGDEVCOUNTER3<> oAvgGyroScopeX(10);
  static AVGDEVCOUNTER3<> oAvgGyroScopeY(10);
  static AVGDEVCOUNTER3<> oAvgGyroScopeZ(10);

  double dTemperaturCelcius = double(getWord(0x41)) / 340.0 + 36.53;
  oAvgTemp.SetValue(dTemperaturCelcius);

  double dGyroskop_xout = getWord(0x43)/131.;
  double dGyroskop_yout = getWord(0x45)/131.;
  double dGyroskop_zout = getWord(0x47)/131.;
  
  oAvgGyroScopeX.SetValue(dGyroskop_xout);
  oAvgGyroScopeY.SetValue(dGyroskop_yout);
  oAvgGyroScopeZ.SetValue(dGyroskop_zout);
  
  double dBeschleunigung_xout = getWord(0x3b);
  double dBeschleunigung_yout = getWord(0x3d);
  double dBeschleunigung_zout = getWord(0x3f);

  double dBeschleunigung_xout_skaliert = dBeschleunigung_xout / 16384.0;
  double dBeschleunigung_yout_skaliert = dBeschleunigung_yout / 16384.0;
  double dBeschleunigung_zout_skaliert = dBeschleunigung_zout / 16384.0;

  oAvgAccelX.SetValue(dBeschleunigung_xout_skaliert);
  oAvgAccelY.SetValue(dBeschleunigung_yout_skaliert);
  oAvgAccelZ.SetValue(dBeschleunigung_zout_skaliert);

  oAvgX.SetValue(getXRotation(dBeschleunigung_xout_skaliert, dBeschleunigung_yout_skaliert, dBeschleunigung_zout_skaliert));
  oAvgY.SetValue(getYRotation(dBeschleunigung_xout_skaliert, dBeschleunigung_yout_skaliert, dBeschleunigung_zout_skaliert));


  std::cout << "\r TemperatureAvg: " << FF(3, 3, '0') << oAvgTemp.GetAvg() << " AVGX:" << oAvgX.GetAvg() << " AVGY:" << oAvgY.GetAvg() << " ACCELX:" << oAvgAccelX.GetAvg() << " ACCELY:" << oAvgAccelY.GetAvg() << " ACCELZ:" << oAvgAccelZ.GetAvg() << FF(5,4,'0') << " GyroX:" << oAvgGyroScopeX.GetAvg() << " GyroY:" << oAvgGyroScopeY.GetAvg() << " GyroZ:" << oAvgGyroScopeZ.GetAvg() << std::flush;
  
  
  if(oAvgTemp.Full())
  {
    static Eigen::FFT<double> oFFT;
    std::vector<double> vecOverTime(oAvgTemp.m_oBufferCircular.size());
    std::copy(oAvgTemp.m_oBufferCircular.begin(), oAvgTemp.m_oBufferCircular.end(), vecOverTime.begin());
    std::vector<std::complex<double> > vecFrequency;
    oFFT.fwd(vecFrequency, vecOverTime);
    
    std::cout << "***" << std::endl;
    for(auto oItComplex = vecFrequency.begin(); oItComplex!=vecFrequency.end(); ++oItComplex)
    {
      std::cout << " FFT:" << *oItComplex << std::endl;
    }
    
  }

  cCommonTools::Sleep(50);
}

int cGyroServer::getWord(int p_iAdress)
{
  int iValue = wiringPiI2CReadReg8(m_iFilehandleGyroDev, p_iAdress);
  iValue = (iValue << 8) + wiringPiI2CReadReg8(m_iFilehandleGyroDev, p_iAdress + 1);

  if (iValue >= 0x8000)
  {
    iValue = -((65535 - iValue) + 1);
  }

  return iValue;
}

void cGyroServer::setWord(int p_iAdress, uint8_t p_ucWord, nTimeTypes::MilliSecs p_oWaitTimeAfterSetting)
{
  if(0 != wiringPiI2CWriteReg8(m_iFilehandleGyroDev, p_iAdress, p_ucWord))
  {
    LOG_TRACE("Unsuccessfull!");
  }
  cCommonTools::Sleep(p_oWaitTimeAfterSetting);
}

double cGyroServer::getXRotation(double p_dX, double p_dY, double p_dZ)
{
  double dRadians = atan2(p_dY, getDist(p_dX, p_dZ));
  return (dRadians * 180. / 3.14159265359);
}

double cGyroServer::getYRotation(double p_dX, double p_dY, double p_dZ)
{
  double dRadians = atan2(p_dX, getDist(p_dY, p_dZ));
  return -(dRadians * 180. / 3.14159265359);
}

double cGyroServer::getDist(double p_dA, double p_dB)
{
  return sqrt((p_dA * p_dA)+(p_dB * p_dB));
}
