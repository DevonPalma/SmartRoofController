#include "ObserverPattern.h"

class TestSubject : public Subject<TestSubject> {
  private:
    int _value;
  public:
    TestSubject() {};
    ~TestSubject() {};
    void setValue(int value) {
      _value = value;
      notify();
    }
    int getValue() {
      return _value;
    }
} A;


class TestObserver : public Observer<TestSubject> {
  public:
    TestObserver() {}
    ~TestObserver() {}

    void update(TestSubject *subject) {
      int val = subject->getValue();
      Serial.printf("Subjects value = %d", val);
    }
} B;

void setup() {
  Serial.begin(9600);
  while (!Serial);
  A.attach(B);
  A.setValue(3);
}

void loop() {
  
}
