#pragma once

#include "TimeServer.h"

namespace helper{

class Age
{
public:
  explicit Age(helper::TimeServerUnix::us p_creationTime = helper::TimeServerUnix::getTimestampCurrent());
  virtual ~Age();

	TimeServerUnix::us GetAgeUS();
  TimeServerUnix::ms GetAgeMS();
	TimeServerUnix::secAsDouble GetAgeS();

  /** milliseconds
  * @author wmk
  * @date 2016/06/07
  */
  TimeServerUnix::us m_creationTimeStamp;
};

}