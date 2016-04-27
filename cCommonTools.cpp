/* 
 * File:   cCommonTools.cpp
 * Author: juia
 * 
 * Created on 17. April 2016, 17:12
 */

#include "cCommonTools.h"
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/thread/thread.hpp> 

void cCommonTools::Sleep(nTimeTypes::MilliSecs p_oTimeMS)
{
  boost::this_thread::sleep(boost::posix_time::milliseconds(p_oTimeMS));
}
