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

cGyroServer::cGyroServer(int p_iAdress)
: m_oAvgTemp(20)
, m_oAvgX(10)
, m_oAvgY(10)
, m_oAvgAccelX(10)
, m_oAvgAccelY(10)
, m_oAvgAccelZ(10)
, m_oAvgGyroScopeX(10)
, m_oAvgGyroScopeY(10)
, m_oAvgGyroScopeZ(10)
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
  double dTemperaturCelcius = double(getWord(0x41)) / 340.0 + 36.53;
  m_oAvgTemp.SetValue(dTemperaturCelcius);

  double dGyroskop_xout = getWord(0x43) / 131.;
  double dGyroskop_yout = getWord(0x45) / 131.;
  double dGyroskop_zout = getWord(0x47) / 131.;

  m_oAvgGyroScopeX.SetValue(dGyroskop_xout);
  m_oAvgGyroScopeY.SetValue(dGyroskop_yout);
  m_oAvgGyroScopeZ.SetValue(dGyroskop_zout);
  
  auto acceleration = getAcceleration();
  
  m_oAvgAccelX.SetValue(2. * m_g * acceleration.X);
  m_oAvgAccelY.SetValue(2. * m_g * acceleration.Y);
  m_oAvgAccelZ.SetValue(2. * m_g * acceleration.Z);

  m_oAvgX.SetValue(getXRotation(acceleration.X, acceleration.Y, acceleration.Z));
  m_oAvgY.SetValue(getYRotation(acceleration.X, acceleration.Y, acceleration.Z));

  std::cout << "\r TemperatureAvg:" << FF(3, 3, '0') << m_oAvgTemp.GetAvg() << " AVGX:" << m_oAvgX.GetAvg() << " AVGY:" << m_oAvgY.GetAvg() << " ACCELX:" << m_oAvgAccelX.GetAvg() << " ACCELY:" << m_oAvgAccelY.GetAvg() << " ACCELZ:" << m_oAvgAccelZ.GetAvg() << FF(5, 4, '0') << " GyroX:" << m_oAvgGyroScopeX.GetAvg() << " GyroY:" << m_oAvgGyroScopeY.GetAvg() << " GyroZ:" << m_oAvgGyroScopeZ.GetAvg() << std::flush;

  //cCommonTools::Sleep(50);
}

GyroServer::Acceleration cGyroServer::getAcceleration()
{
  double dBeschleunigung_xout = getWord(0x3b);
  double dBeschleunigung_yout = getWord(0x3d);
  double dBeschleunigung_zout = getWord(0x3f);

  double dBeschleunigung_xout_skaliert = dBeschleunigung_xout / 16384.0;
  double dBeschleunigung_yout_skaliert = dBeschleunigung_yout / 16384.0;
  double dBeschleunigung_zout_skaliert = dBeschleunigung_zout / 16384.0;
  
  return {dBeschleunigung_xout_skaliert, dBeschleunigung_yout_skaliert, dBeschleunigung_zout_skaliert};
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
  if (0 != wiringPiI2CWriteReg8(m_iFilehandleGyroDev, p_iAdress, p_ucWord))
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
