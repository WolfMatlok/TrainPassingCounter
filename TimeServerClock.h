#pragma once

#include <boost/chrono.hpp>
#include <boost/chrono/clock_string.hpp>

namespace helper
{
  // 		template<typename T_TIMESERVER>
  // 		struct _TimeServerClockString
  // 		{
  // 			typedef T_TIMESERVER time_server;
  // 			static std::string name() { return STMSTR(" ClockName_" << time_server::EPOCHE_START); };
  // 			static std::string since() { return STMSTR(" since " << time_server::EPOCHE_START); };
  // 		};

  template<typename T_TIMESERVER>
  class TimeServerClock
  {
  public:
    typedef T_TIMESERVER time_server;
    typedef typename T_TIMESERVER::t_clock_precision duration;
    typedef typename duration::rep rep;
    typedef typename duration::period period;
    typedef boost::chrono::time_point<TimeServerClock<T_TIMESERVER>> time_point;
    //typedef _TimeServerClockString<T_TIMESERVER>                      clock_string_timeserver;
    static const bool is_steady = false;

    static time_point now()
    {
      using namespace boost::chrono;
      typedef boost::chrono::duration<uint64_t, boost::ratio_multiply<boost::chrono::hours::period, boost::ratio < 24 >> > days;
      static auto daySince1970_Count = time_server::EPOCHE_START_1970_1_1.day_count().as_number();
      static auto daySinceOwnEpoche_Count = time_server::EPOCHE_START.day_count().as_number();
      static auto daySince1970 = boost::chrono::duration_cast<duration>(days(daySince1970_Count));
      static auto daySinceOwnEpoche = boost::chrono::duration_cast<duration>(days(daySinceOwnEpoche_Count));
      static auto dayEpocheDifference = daySince1970 - daySinceOwnEpoche;

      auto nowTPSince1970_1_1 = duration_cast<duration>(system_clock::now().time_since_epoch());

      auto nowTPFinal = nowTPSince1970_1_1 + dayEpocheDifference;

      auto nowTP2Return = time_point(nowTPFinal);

      return nowTP2Return;
    }
  };
}