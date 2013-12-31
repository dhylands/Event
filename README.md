Event
=====

Event based processing (for Arduino)

Add timers to your setup() routine.

For example, to create a Hearbeat, you would add the following to your main 
sketch:
```
#include "Event.h"
#include "TimerEvent.h"

class HeartBeat : public TimerEvent
{
public:
  HeartBeat() : TimerEvent("HeartBeat"), _count(0) {}
  virtual void onEvent(const TimeVal &when)
  {
    if ((_count & 0xC) == 0) {
      digitalWrite(led, !(_count & 1));
    }
    _count = (_count + 1) % 10; 
  }

private:
  int _count;
} gHeartBeat;
```

Then add to setup():

```
gHeartBeat.Repeat(MilliSeconds(100));
```

and in loop():
```
gTimerQueue.Run();
gEventQueue.Run();
```

gHeartBeat's onEvent will be called every 100 milliseconds, and it logic in there
will do a double blink, once per second.

So on for 100 msec, off for 100 msec, on for 100 msec, off for 700 msec.
