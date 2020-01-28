#include "led.hpp"
#include "main.h"

namespace Bsp {
  template <> GPIO_TypeDef* Led<LedResources::Ok>::gpioGroup{LED_OK_GPIO_Port};
  template <> uint16_t Led<LedResources::Ok>::gpioPin{LED_OK_Pin};
	
  template <> GPIO_TypeDef* Led<LedResources::Error>::gpioGroup{LED_ERR_GPIO_Port};
  template <> uint16_t Led<LedResources::Error>::gpioPin{LED_ERR_Pin};
}