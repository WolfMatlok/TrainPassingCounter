/* 
 * File:   cUnitTest.h
 * Author: juia
 *
 * Created on 17. April 2016, 17:15
 */

#ifndef CUNITTEST_H
#define	CUNITTEST_H

#include <ctime>


class cUnitTest
{
public:
  static void DoAllTests();
  
  static void CheckFPSCamera();
  
  static void CheckDifference();
  
  static void CheckGyroSensor();
  
  static void CheckTimeServer();
private:

};

#endif	/* CUNITTEST_H */

