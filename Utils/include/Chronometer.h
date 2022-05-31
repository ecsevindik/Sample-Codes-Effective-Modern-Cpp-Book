#pragma once //For compatibility

#ifndef UTILS_INCLUDE_CHRONOMETER_H_
#define UTILS_INCLUDE_CHRONOMETER_H_

#include <chrono>
#include <cassert>
#include <string>
#include <iostream>

namespace utils {

class Chronometer {
 public:
  Chronometer() : state_{kFresh} {}
  void Start() {
    assert(kFresh == state_);
    state_ = kRunning;
    start_time_ = std::chrono::steady_clock::now();
  }
  void Stop()  {
    assert(kRunning == state_);
    state_ = kStopped;
    end_time_   = std::chrono::steady_clock::now();
  }
  void Report(std::string pre= "") {
    assert(kStopped == state_);
    auto nsecs = std::chrono::duration_cast<std::chrono::nanoseconds>(end_time_ - start_time_);
    std::string msg = pre + "- Processing Elapsed Time:" + std::to_string(nsecs.count()) + " ns";
    std::cout << msg << std::endl;
    state_ = kFresh;
  }
 private:
  enum {kFresh, kRunning, kStopped} state_;
  std::chrono::time_point<std::chrono::steady_clock> start_time_;
  std::chrono::time_point<std::chrono::steady_clock> end_time_;
};

} // namespace utils

#endif //UTILS_INCLUDE_CHRONOMETER_H_
