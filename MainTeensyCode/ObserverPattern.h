// code is based off of https://www.codeproject.com/articles/3267/implementing-a-subject-observer-pattern-with-templ

#ifndef _OBSERVER_PATTERN_H_
#define _OBSERVER_PATTERN_H_

template <class T>
class Observer {
  public:
    Observer() {};
    virtual ~Observer() {}
    virtual void update(T *subject) = 0;
};

template <class T>
class Subject {
  private:
    Observer<T> *_observers[16] = {};
    byte _curObserver;
  public:
    Subject() {};
    virtual ~Subject() {};
    void attach(Observer<T> &observer) {
      _observers[_curObserver] = &observer;
    }
    void notify() {
      for (int i = 0; i < _curObserver + 1; i++) {
        _observers[i]->update((T*) this);
      }
    }
};

#endif
