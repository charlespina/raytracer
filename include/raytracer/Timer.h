#ifndef RT_TIMER_H
#define RT_TIMER_H

#include <chrono>

namespace raytracer {

class Timer {
public:
  using Clock = std::chrono::high_resolution_clock;

  Timer(const std::string &name) : _name(name), _start(Clock::now()) {}
  ~Timer() {
    auto end = Clock::now();
    std::cout << _name << ": " << std::chrono::duration_cast<std::chrono::milliseconds>(end - _start).count() << " ms" << std::endl;
  }

public:
  std::string _name;
  Clock::time_point _start;
};

} // raytracer

#endif