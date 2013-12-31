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
*   @brief  Contains definitions for the TimeStamp class, which is used for
*           maintaining a monotonic notion of time.
*
****************************************************************************/

#include "TimeVal.h"
#include "Logging.h"
#include "Arduino.h"

TimeVal::TimeVal(TimeVal::Seconds_t seconds, TimeVal::MicroSeconds_t microSeconds)
  : _seconds(seconds),
    _microSeconds(microSeconds)
{
  Normalize();
}

void TimeVal::Normalize()
{
  if (_microSeconds > 1000000) {
    _seconds += _microSeconds / 1000000; 
    _microSeconds %= 1000000;
  }
}

TimeVal &TimeVal::operator += (const TimeDuration &delay)
{
  _seconds += delay._seconds;
  _microSeconds += delay._microSeconds;
  if (_microSeconds >= 1000000) {
    _seconds++;
    _microSeconds -= 1000000;
  }
  return *this;
}

TimeVal &TimeVal::operator -= (const TimeDuration &delay)
{
  if (delay > *this) {
    _seconds = 0;
    _microSeconds = 0;
    return *this;
  }
  _seconds -= delay.Seconds();
  if (delay.MicroSeconds() > _microSeconds) {
    _seconds--;
    _microSeconds += 1000000;
  }
  _microSeconds -= delay.MicroSeconds();
  return *this;
}

TimeDuration TimeVal::operator - (const TimeVal &rhs)
{
  if (*this > rhs) {
    if (_microSeconds >= rhs._microSeconds) {
      return TimeDuration(_seconds - rhs._seconds, _microSeconds - rhs._microSeconds);
    }
    return TimeDuration(rhs._seconds - _seconds - 1, 1000000 + _microSeconds - rhs._microSeconds);
  }
  return TimeDuration(0, 0);
}

TimeVal TimeVal::operator - (const TimeDuration &rhs)
{
  TimeVal result(*this);
  result -= rhs;
  return result;
}

TimeVal TimeVal::operator + (const TimeDuration &rhs)
{
  TimeVal result(*this);
  result += rhs;
  return result;
}

static TimeVal sNow;

// static
TimeVal TimeVal::Now()
{
  static uint32_t prev_micros;

  uint32_t curr_micros = micros();
  
  sNow += ::MicroSeconds(curr_micros - prev_micros);
  
  prev_micros = curr_micros;
  return sNow;
}
