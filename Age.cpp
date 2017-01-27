#include "Age.h"

helper::Age::Age( helper::TimeServerUnix::us p_creationTime /*= helper::TimeServer::GetCurrentTimeStamp()*/ ) :m_creationTimeStamp(p_creationTime)
{

}

helper::Age::~Age()
{

}

helper::TimeServerUnix::secAsDouble helper::Age::GetAgeS()
{
	return boost::chrono::duration_cast<helper::TimeServerUnix::secAsDouble>(GetAgeMS());
}

helper::TimeServerUnix::us helper::Age::GetAgeUS()
{
	return helper::TimeServerUnix::getTimestampCurrent() - m_creationTimeStamp;
}

helper::TimeServerUnix::ms helper::Age::GetAgeMS()
{
  return boost::chrono::duration_cast<helper::TimeServerUnix::ms>(GetAgeUS());
}
