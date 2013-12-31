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
*   @file   Event.h
*
*   @brief  Contains definitions for the Event class, which is an abstract
*           base class for events.
*
****************************************************************************/

#if !defined(EVENT_H)
#define EVENT_H

#include "CBUF.h"
#include "TimeVal.h"

// MAX_EVENTS needs to be a power of 2 (to be compatible with CBUF)
#if !defined(MAX_EVENTS)
#define MAX_EVENTS  64
#endif

class Event
{
public:
  Event(const char *name) : _name(name) {}
  virtual ~Event() {}

  virtual void onEvent(const TimeVal &when) = 0;

  const char *Name() {return _name; }

private:
  const char *_name;
};

class EventEntry
{
public:
  EventEntry() : _event(nullptr) {}
  EventEntry(Event *event) : _when(TimeVal::Now()), _event(event) {}

  void Run()
  {
    _event->onEvent(_when);
  }
  
private:
  TimeVal _when;
  Event *_event;
};

class EventQueue
{
public:
  void Add(Event *event);
  void Run();

private:
  CBUF<uint32_t, MAX_EVENTS, EventEntry> _queue;
};

extern EventQueue gEventQueue;

#endif // EVENT_H
