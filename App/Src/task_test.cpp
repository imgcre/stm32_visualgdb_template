#include "task_test.hpp"
#include "cmsis_os.h"
#include "led.hpp"
#include <stdio.h>

#include "async.hpp"

using namespace Bsp;
using namespace Os;

void StartTestTask(void *argument) {
	auto ledOk = Led<LedResources::Ok>::instance();

	for (;;) {
		ledOk.on();
		osDelay(100);
		ledOk.off();
		osDelay(100);
  }
}