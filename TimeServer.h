#pragma once

/* 
 * File:   TimeServer.h
 * Author: wmk
 *
 * Created on 21. Dezember 2016, 07:56
 */

#include <boost/chrono.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/date_time.hpp>
#include <boost/date_time/local_time/local_time.hpp>
#include <boost/filesystem.hpp>
#include <boost/locale.hpp>

#include "TimeServerClock.h"


namespace helper
{

  struct TimeServerEpoche
  {
    static const boost::gregorian::date EPOCHE_START_1970_1_1;
    static const boost::gregorian::date EPOCHE_START_1601_1_1;
  };

  template<const boost::gregorian::date* T_EPOCHESTART, typename T_CLOCK_PRECISION = boost::chrono::microseconds>
  class TimeServer : public TimeServerEpoche
  {
  public:
    typedef boost::chrono::microseconds us;
    typedef boost::chrono::milliseconds ms;
    typedef boost::chrono::seconds sec;
    typedef boost::chrono::duration<double> secAsDouble;
    typedef boost::chrono::duration<uint64_t, boost::ratio_multiply<boost::chrono::hours::period, boost::ratio < 24 >> > days;
    typedef T_CLOCK_PRECISION t_clock_precision;
    typedef TimeServerClock<TimeServer<T_EPOCHESTART, T_CLOCK_PRECISION>> clock_type;
    typedef typename TimeServerClock<TimeServer<T_EPOCHESTART, T_CLOCK_PRECISION>>::time_point time_point;

    TimeServer()
    {
    }

    virtual ~TimeServer()
    {
    }

    static T_CLOCK_PRECISION getTimestampCurrent()
    {
      return clock_type::now().time_since_epoch();
    }

    static time_point getTimepointCurrent()
    {
      return clock_type::now();
    }

    static std::string getFormattedTimeCurrent()
    {
      return getFormattedTime(getTimestampCurrent());
    }

    static std::string getFormattedTime(const T_CLOCK_PRECISION& timestamp, const boost::gregorian::date& epochestart = *T_EPOCHESTART)
    {
      using namespace boost::local_time;

      local_date_time t(epochestart, boost::posix_time::microseconds(boost::chrono::duration_cast<us>(timestamp).count()), time_zone_ptr(), false);

      local_time_facet* poLocFormat = new local_time_facet("%Y-%m-%dT%H:%M:%S%F%Q");

      std::stringstream oStrStr;
      oStrStr.imbue(std::locale(oStrStr.getloc(), poLocFormat));
      oStrStr << t;
      return oStrStr.str();
    }

    static std::string getFormattedTime(const time_point& timepoint)
    {
      return getFormattedTime(timepoint.time_since_epoch(), time_point::clock::time_server::EPOCHE_START);
    }

    static const boost::gregorian::date& EPOCHE_START;

  };

  template <const boost::gregorian::date* T_EPOCHESTART, typename T_CLOCK_PRECISION>
  const boost::gregorian::date& TimeServer<T_EPOCHESTART, T_CLOCK_PRECISION>::EPOCHE_START = *T_EPOCHESTART;

  typedef TimeServer<&TimeServerEpoche::EPOCHE_START_1970_1_1> TimeServerUnix;
  typedef TimeServer<&TimeServerEpoche::EPOCHE_START_1601_1_1> TimeServerNT;

}