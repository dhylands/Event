/****************************************************************************
*
*   Copyright (c) 2013 Dave Hylands     <dhylands@gmail.com>
*
*   This program is free software; you can redistribute it and/or modify
*   it under the terms of the MIT License.
*
*   See LICENSE for more details.
*
****************************************************************************/
/**
*
*   @file   TimeVal.h
*
*   @brief  Contains definitions for the TimeVal class, which is used for
*           maintaining a monotonic notion of time, and the TimeDuration
*           class which is used for measuring the difference between two
*           TimeVals.
*
****************************************************************************/

#if !defined(TIMESTAMP_H)
#define TIMESTAMP_H

#include <stdint.h>

class TimeDuration;

class TimeVal
{
public:
  typedef uint32_t  Seconds_t;
  typedef uint32_t  MilliSeconds_t;
  typedef uint32_t  MicroSeconds_t;

  TimeVal() : _seconds(0), _microSeconds(0) {}
  TimeVal(Seconds_t seconds, MicroSeconds_t microSeconds);

  Seconds_t Seconds() const { return _seconds; }
  MicroSeconds_t MicroSeconds() const {return _microSeconds; }

  TimeVal &operator += (const TimeDuration &delay);
  TimeVal &operator -= (const TimeDuration &delay);

  TimeDuration operator - (const TimeVal &rhs);
  TimeVal operator + (const TimeDuration &rhs);
  TimeVal operator - (const TimeDuration &rhs);

  static TimeVal Now();

private:
  void Normalize();

  Seconds_t       _seconds;
  MicroSeconds_t  _microSeconds;
};

inline bool operator < (const TimeVal &lhs, const TimeVal &rhs)
{
  return lhs.Seconds() < rhs.Seconds() 
      || (lhs.Seconds() == rhs.Seconds() && lhs.MicroSeconds() < rhs.MicroSeconds());
}

inline bool operator > (const TimeVal &lhs, const TimeVal &rhs)
{
  return operator <(rhs, lhs);
}

inline bool operator <= (const TimeVal &lhs, const TimeVal &rhs)
{
  return !operator >(lhs, rhs);
}

inline bool operator >= (const TimeVal &lhs, const TimeVal &rhs)
{
  return !operator <(lhs, rhs);
}

inline bool operator == (const TimeVal &lhs, const TimeVal &rhs)
{
  return lhs.Seconds() == rhs.Seconds() && lhs.MicroSeconds() == rhs.MicroSeconds();
}

inline bool operator != (const TimeVal &lhs, const TimeVal &rhs)
{
  return !operator ==(lhs, rhs);
}


class TimeDuration : public TimeVal
{
public:
  TimeDuration() : TimeVal(0, 0) {}
  TimeDuration(Seconds_t seconds, MicroSeconds_t microSeconds)
    : TimeVal(seconds, microSeconds) {}
};

inline TimeDuration MicroSeconds(TimeVal::MicroSeconds_t microSeconds)
{
  return TimeDuration(0, microSeconds);
}

inline TimeDuration MilliSeconds(TimeVal::MilliSeconds_t milliSeconds)
{
  return TimeDuration(0, milliSeconds * 1000);
}

#endif  // TIMESTAMP_H
