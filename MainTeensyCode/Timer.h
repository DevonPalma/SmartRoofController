#ifndef _TIMER_H_
#define _TIMER_H_


class Timer {
    unsigned int startTime;
    unsigned int durationTime;


  public:

    Timer() {
      startTime = 0;
      durationTime = 0;
    }

    Timer(int durationT) {
      setDuration(durationT);
      start();
    }

    void setDuration(int durTime) {
      durationTime = durTime;
    }

    void start() {
      startTime = millis();
    }
    void stop() {
      startTime = 0;
    }

    int timeLeft() {
      int endTime = startTime + durationTime;
      int curTime = millis();
      return isDone() ? 0 : endTime - curTime;
    }

    bool isDone() {
      return startTime + durationTime < millis();
    }
    bool isRunning() {
      return startTime != 0;
    }
};

#endif
