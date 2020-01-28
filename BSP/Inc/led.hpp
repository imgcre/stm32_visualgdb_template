#pragma once
#include "singleton.hpp"
#include "stm32f1xx_hal.h"

namespace Bsp {
using namespace Utilities;
enum class LedResources {
  Ok,
  Error,
};
    
template <LedResources Resource>
class Led : public Singleton<Led<Resource>> {
  friend class Singleton<Led<Resource>>;
public:
  void on() {
    HAL_GPIO_WritePin(gpioGroup, gpioPin, GPIO_PIN_RESET);
  }
  void off() {
    HAL_GPIO_WritePin(gpioGroup, gpioPin, GPIO_PIN_SET);
  }
private:
  Led(){};
  static GPIO_TypeDef* gpioGroup;
  static uint16_t gpioPin;
    
};
    
}
