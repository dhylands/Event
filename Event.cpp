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
*   @file   Event.cpp
*
*   @brief  Implements the EventQueue class.
*
****************************************************************************/

#include "Event.h"

EventQueue gEventQueue;

void EventQueue::Add(Event *event)
{
  _queue.Push(EventEntry(event));
}

void EventQueue::Run()
{
  if (_queue.IsEmpty()) {
    return;
  }

  EventEntry entry = _queue.Pop();
  entry.Run();
}

