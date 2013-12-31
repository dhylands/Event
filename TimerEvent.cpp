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
*   @file   TimerEvent.cpp
*
*   @brief  Implements the TimerEvent and Timer classes.
*
****************************************************************************/

#include "TimerEvent.h"
#include "Logging.h"
#include <Arduino.h>

TimerQueue gTimerQueue;

TimerEvent::TimerEvent(const char *name)
  : Event(name),
    _mode(ONE_SHOT),
    _next(nullptr),
    _prev(nullptr),
    _behind(0)
{
}

void TimerEvent::OneShot(TimeDuration when)
{
  _mode = ONE_SHOT;
  _interval = when;
  _expiry = TimeVal::Now() + when;

  gTimerQueue.Add(this);
}

void TimerEvent::Dump(const char *label)
{
  Log.Info("%s: %s expiry: %d.%06d mode: %s interval: %d.%06d",
           label, Name(), _expiry.Seconds(), _expiry.MicroSeconds(),
           _mode == ONE_SHOT ? "OneShot" : "Repeat",
           _interval.Seconds(), _interval.MicroSeconds());
}

void TimerEvent::Repeat(TimeDuration interval)
{
  _mode = REPEAT;
  _interval = interval;
  _expiry = TimeVal::Now() + interval;

  gTimerQueue.Add(this);
}

TimerQueue::TimerQueue()
  : _head(nullptr),
    _tail(nullptr)
{
}

void TimerQueue::Add(TimerEvent *timer)
{
  Remove(timer);
  
  uint8_t sreg = SREG;
  cli();
  if (_head == nullptr) {
    // Adding the first node to the list.
    _head = _tail = timer;
    return;
  }
  TimerEvent *scan = _head; 
  while (scan && timer->_expiry < scan->_expiry) {
    scan = scan->_next;
  }

  timer->_next = scan;
  if (scan->_prev) {
    timer->_prev = scan->_prev;
    scan->_prev->_next = timer;
  }
  else {
    _head = timer;
  }
  if (scan) {
    scan->_prev = timer;
  }
  else {
    _tail = timer;
  }
  SREG = sreg;
}

void TimerQueue::Remove(TimerEvent *timer)
{
  // Remove timer from the list, allowing for the fact that timer may
  // not already be on this list (i.e. next == prev === nullptr

  uint8_t sreg = SREG;
  cli();
  if (_head == timer) {
    _head = timer->_next;
  }
  if (_tail == timer) {
    _tail = timer->_prev;
  }
  if (timer->_prev) {
    timer->_prev->_next = timer->_next;
  }
  if (timer->_next) {
    timer->_next->_prev = timer->_prev;
  }
  SREG = sreg;
  timer->_next = timer->_prev = nullptr;
}

void TimerQueue::Run()
{
  TimeVal now = TimeVal::Now();
  
  static TimeVal::Seconds_t prev_seconds = 9999;
  if (now.Seconds() != prev_seconds) {
    prev_seconds = now.Seconds();
  }

  uint8_t sreg = SREG;
  cli();

  TimerEvent *timer = _head;
  if (timer && timer->_expiry <= now) {
    Remove(timer);
  }
  else {
    timer = nullptr;
  }
  SREG = sreg;

  if (!timer) {
    return;
  }

  if (timer->_mode == TimerEvent::REPEAT) {
    timer->_expiry += timer->_interval;
    if (timer->_expiry <= now) {
      // This means that the timer is falling behind. We let this happen
      // a few times, but if it persists then we print a warning and stop
      // re-adding the timer.

      if (++timer->_behind > 5) {
        Log.Warning("Timer finding behind");
        return;
      }
    }
    else {
      timer->_behind = 0;
    }
    Add(timer);
  }
  gEventQueue.Add(timer);
}

void TimerQueue::Dump()
{
  TimerEvent *scan = _head;
  Log.Info("TimerQueue");
  while (scan) {
    scan->Dump("  Q");
    scan = scan->_next;
  }
}
