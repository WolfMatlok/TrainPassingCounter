#pragma once

#include "TimeServer.h"

namespace helper{

class Age
{
public:
  explicit Age(helper::TimeServerUnix::us p_creationTime = helper::TimeServerUnix::getTimestampCurrent());
  virtual ~Age();
  
  void reset(){m_creationTimeStamp = helper::TimeServerUnix::getTimestampCurrent();};

	TimeServerUnix::us GetAgeUS(bool reset = false);
  TimeServerUnix::ms GetAgeMS(bool reset = false);
	TimeServerUnix::secAsDouble GetAgeS(bool reset = false);

  /** milliseconds
  * @author wmk
  * @date 2016/06/07
  */
  TimeServerUnix::us m_creationTimeStamp;
};

}