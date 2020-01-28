#include "task_default.hpp"
#include "cmsis_os.h"

void StartDefaultTask(void *argument) {
    for (;;) {
        osDelay(1);
    }
}