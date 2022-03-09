
#ifndef _LOOPER_H_
#define _LOOPER_H_

const int LOOPER_MAX = 10;
const int UPDATES_PER_SECOND = 60;
const int DRAWS_PER_SECOND = 20;

class Looper {
  public:
    virtual void tick() = 0;
    virtual void update() = 0;
    virtual void draw() = 0;
};

Looper *__allLoopers[LOOPER_MAX];
int __curLooper = 0;

void addLooper(Looper &looper) {
  __allLoopers[__curLooper] = &looper;
  __curLooper++;
}


void runLoopers() {
  const float timeU = 1000.0 / UPDATES_PER_SECOND;
  const float timeD = 1000.0 / DRAWS_PER_SECOND;
  static int initTime = millis();
  static int updates = 0;
  static int draws = 0;
  static int ticks = 0;
  static int timer = initTime;
  static float deltaU = 0;
  static float deltaD = 0;

  int currentTime = millis();
  deltaU += (currentTime - initTime) / timeU;
  deltaD += (currentTime - initTime) / timeD;
  initTime = currentTime;

  for (int i = 0; i < __curLooper; i++) {
    __allLoopers[i]->tick();
  }
  ticks++;

  if (deltaU >= 1) {
    for (int i = 0; i < __curLooper; i++) {
      __allLoopers[i]->update();
    }
    updates++;
    deltaU--;
  }

  if (deltaD >= 1) {
    for (int i = 0; i < __curLooper; i++) {
      __allLoopers[i]->draw();
    }
    draws++;
    deltaD--;
  }

  if (currentTime - timer > 1000) {
    Serial.printf("UPS: %d   DPS: %d   TPS: %d\n", updates, draws, ticks);
    updates = 0;
    draws = 0;
    ticks = 0;
    timer += 1000;
  }
}

#endif
