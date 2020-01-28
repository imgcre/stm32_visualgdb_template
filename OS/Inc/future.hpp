#pragma once

namespace Os {
enum class FutureStatus
{
	Deferred,
	Ready,
	Timeout,
};

template <class T>
class Future {
public:
  virtual T get() = 0;
  virtual FutureStatus wait() = 0;
  virtual FutureStatus waitFor(int ticks) = 0;
};
}