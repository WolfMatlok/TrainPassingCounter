#include "Age.h"

helper::Age::Age( helper::TimeServerUnix::us p_creationTime /*= helper::TimeServer::GetCurrentTimeStamp()*/ ) :m_creationTimeStamp(p_creationTime)
{

}

helper::Age::~Age()
{

}

helper::TimeServerUnix::secAsDouble helper::Age::GetAgeS(bool reset /*= false*/)
{
	return boost::chrono::duration_cast<helper::TimeServerUnix::secAsDouble>(GetAgeMS(reset));
}

helper::TimeServerUnix::us helper::Age::GetAgeUS(bool reset /*= false*/)
{
  if(reset)
  {
    auto currentAge = helper::TimeServerUnix::getTimestampCurrent() - m_creationTimeStamp;
    this->reset();
    return currentAge;
  }
  else
  {
    return helper::TimeServerUnix::getTimestampCurrent() - m_creationTimeStamp;
  }
}

helper::TimeServerUnix::ms helper::Age::GetAgeMS(bool reset /*= false*/)
{
  return boost::chrono::duration_cast<helper::TimeServerUnix::ms>(GetAgeUS(reset));
}