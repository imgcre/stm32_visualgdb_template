#pragma once
#include <functional>
#include "cmsis_os.h"
#include "future.hpp"

#include "FreeRTOS.h"
#include <task.h>
#include <event_groups.h>

namespace Os {
using namespace std;

enum class Launch
{
	Async,
	Deferred,
};
template <class Function, class Result_t, class... Args>
class AsyncBase : public Future<Result_t> {
public:
  AsyncBase(Launch policy, Function &&f, Args &&... args) :
	func(bind(f, forward<Args>(args)...)), 
	policy(policy),
	eventId(osEventFlagsNew(nullptr)),
	exceptionThrown() {
	  osThreadNew([](void *arg) {
		  auto self = (AsyncBase<Function, Result_t, Args...> *)arg;
		  if (self->policy == Launch::Deferred) {
			  osThreadFlagsWait(true, NULL, osWaitForever);
		  }
		  try {
			  self->onFuncInvoking();
		  }
		  catch (const exception &e) {
			  self->exceptionThrown = e;
		  }

		  osEventFlagsSet(self->eventId, true);
		  self->terminated = true;
		  osThreadExit();
	  }, this, nullptr);
	}

	~AsyncBase() {
		//不要直接判断线程是否被终止, 因为线程id可能被另一个线程使用
		if (terminated) {
			osThreadTerminate(threadId);
		}

		osEventFlagsDelete(eventId);
	}

	virtual Result_t get() override = 0;
	
	virtual FutureStatus wait() override {
		return waitFor(osWaitForever);
	}

	virtual FutureStatus waitFor(int timeout) override {
		if (policy == Launch::Deferred) {
			osThreadFlagsSet(threadId, true);
		}

		osEventFlagsWait(eventId, true, NULL, timeout);
	}

  protected:
	virtual void onFuncInvoking() = 0;
	osThreadId_t threadId;
	osEventFlagsId_t eventId;
	function<Result_t()> func;
	Launch policy;
	bool terminated = false;
	optional<exception> exceptionThrown;
};
}