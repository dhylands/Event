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
*   @file   TimerEvent.h
*
*   @brief  Contains definitions for the TimedEvent class, which triggers
*           after a period of time has elapsed.
* 
*   TimedEvents can repeat, or be one-shot.
*
****************************************************************************/

#if !defined(TIMEREVENT_H)
#define TIMEREVENT_H

#include "Event.h"
#include "TimeVal.h"

class TimerEvent : public Event
{
  friend class TimerQueue;

public:
  enum Mode {
    ONE_SHOT,
    REPEAT
  };

  TimerEvent(const char *name);

  void OneShot(TimeDuration when);
  void Repeat(TimeDuration interval);
  
  void Dump(const char *label);

private:
  Mode  _mode;
  TimeVal _expiry;
  TimeDuration  _interval;

  TimerEvent *_next;
  TimerEvent *_prev;

  int _behind;
};

class TimerQueue
{
  friend class Timer;

public:
  TimerQueue();
  void Add(TimerEvent *timer);
  void Remove(TimerEvent *timer);
  void Run();
  void Dump();

private:
  TimerEvent *_head;
  TimerEvent *_tail;
};

extern TimerQueue gTimerQueue;

#endif  // TIMEREVENT_H

