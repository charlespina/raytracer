#ifndef RT_MULTITHREADING_H
#define RT_MULTITHREADING_H

#include "tbb/parallel_for.h"
#include <functional>

#define RT_MULTITHREADING_ENABLED 1

namespace raytracer {

void parallel_for(size_t initial_value, size_t count, std::function<void(size_t)> fn) {
  tbb::parallel_for(initial_value, count, fn);
}

void serial_for(size_t initial_value, size_t count, std::function<void(size_t)> fn) {
  for (size_t i=initial_value; i<count; i++) {
    fn(i);
  }
}

#if RT_MULTITHREADING_ENABLED
#define RT_FOR(...) parallel_for(__VA_ARGS__)
#else
#define RT_FOR(...) serial_for(__VA_ARGS__)
#endif

} // raytracer

#endif // header guard