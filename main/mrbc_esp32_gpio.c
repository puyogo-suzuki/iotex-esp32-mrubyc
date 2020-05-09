/*! @file
  @brief
  mruby/c GPIO class for ESP32
  本クラスはインスタンスを生成せず利用する
*/

#include "mrbc_esp32_gpio.h"

#include "driver/gpio.h"


static struct RClass* mrbc_class_esp32_gpio;

static int unreferenced;


/*! メソッド nop(count) 本体 : nop (no operation)

  @param count nop の長さ、ダミー処理ループの回数
*/
static void
mrbc_nop(mrb_vm* vm, mrb_value* v, int argc)
{
  // NO OPERATION
  int max = GET_INT_ARG(1);
  for ( int i = 0 ; i < max ; ++i ) {
    unreferenced += 1;
  }
}


/*! メソッド config_output(pin) 本体 : wrapper for gpio_config

  @param pin GPIO ピン番号
*/
static void
mrbc_esp32_gpio_config_output(mrb_vm* vm, mrb_value* v, int argc)
{
  int pin = GET_INT_ARG(1);

  gpio_config_t config = {
    .intr_type = GPIO_PIN_INTR_DISABLE,
    .mode = GPIO_MODE_OUTPUT,
    .pin_bit_mask = (1ULL << pin),
    .pull_down_en = 0,
    .pull_up_en = 0,
  };

  gpio_config(&config);
}


/*! メソッド set_pullup(pin) 本体 : wrapper for gpio_set_pull_mode
  GPIO_PULLUP_ONLY 専用

  @param pin GPIO ピン番号
*/
static void
mrbc_esp32_gpio_set_pullup(mrb_vm* vm, mrb_value* v, int argc)
{
  int pin = GET_INT_ARG(1);
  gpio_set_pull_mode(pin, GPIO_PULLUP_ONLY);
}


/*! メソッド set_floating(pin) 本体 : wrapper for gpio_set_pull_mode
  GPIO_FLOATING 専用

  @param pin GPIO ピン番号
*/
static void
mrbc_esp32_gpio_set_floating(mrb_vm* vm, mrb_value* v, int argc)
{
  int pin = GET_INT_ARG(1);
  gpio_set_pull_mode(pin, GPIO_FLOATING);
}


/*! メソッド set_mode_input(pin) 本体 : wrapper for gpio_set_direction
  GPIO_MODE_INPUT 専用

  @param pin GPIO ピン番号
*/
static void
mrbc_esp32_gpio_set_mode_input(mrb_vm* vm, mrb_value* v, int argc)
{
  int pin = GET_INT_ARG(1);
  gpio_pad_select_gpio(pin);
  gpio_set_direction(pin, GPIO_MODE_INPUT);
  //  gpio_set_pull_mode(pin, GPIO_PULLUP_ONLY);
}


/*! メソッド set_mode_input(pin) 本体 : wrapper for gpio_set_direction
  GPIO_MODE_OUTPUT 専用

  @param pin GPIO ピン番号
*/
static void
mrbc_esp32_gpio_set_mode_output(mrb_vm* vm, mrb_value* v, int argc)
{
  int pin = GET_INT_ARG(1);
  gpio_pad_select_gpio(pin);
  gpio_set_direction(pin, GPIO_MODE_OUTPUT);
}


/*! メソッド set_level(pin, level) 本体 : wrapper for gpio_set_level

  @param pin   GPIO ピン番号
  @param level ピンレベル、0 : low / 1 : high
*/
static void
mrbc_esp32_gpio_set_level(mrb_vm* vm, mrb_value* v, int argc)
{
  int pin   = GET_INT_ARG(1);
  int level = GET_INT_ARG(2);
  gpio_set_level(pin, level);
}


/*! メソッド get_level(pin) 本体 : wrapper for gpio_get_level

  @param pin GPIO ピン番号
  @return    ピンレベル、0 : low / 1 : high
*/
static void
mrbc_esp32_gpio_get_level(mrb_vm* vm, mrb_value* v, int argc)
{
  int pin = GET_INT_ARG(1);
  // Fixnum インスタンスを本メソッドの返り値としてセット、値は gpio_get_level(pin) と同値
  SET_INT_RETURN(gpio_get_level(pin));
}


/*! クラス定義処理を記述した関数
  この関数を呼ぶことでクラス GPIO が定義される

  @param vm mruby/c VM
*/
void
mrbc_mruby_esp32_gpio_gem_init(struct VM* vm)
{
/*
GPIO.set_pullup(pin)
GPIO.set_floating(pin)
GPIO.set_mode_input(pin)
GPIO.set_mode_output(pin)
GPIO.set_level(pin, level)
GPIO.get_level(pin)
*/
  // クラス GPIO 定義
  mrbc_class_esp32_gpio = mrbc_define_class(vm, "GPIO", mrbc_class_object);

  // 各メソッド定義（mruby/c ではインスタンスメソッドをクラスメソッドとしても呼び出し可能）
  mrbc_define_method(vm, mrbc_class_esp32_gpio, "set_pullup",      mrbc_esp32_gpio_set_pullup);
  mrbc_define_method(vm, mrbc_class_esp32_gpio, "set_floating",    mrbc_esp32_gpio_set_floating);
  mrbc_define_method(vm, mrbc_class_esp32_gpio, "set_mode_input",  mrbc_esp32_gpio_set_mode_input);
  mrbc_define_method(vm, mrbc_class_esp32_gpio, "set_mode_output", mrbc_esp32_gpio_set_mode_output);
  mrbc_define_method(vm, mrbc_class_esp32_gpio, "set_level",       mrbc_esp32_gpio_set_level);
  mrbc_define_method(vm, mrbc_class_esp32_gpio, "get_level",       mrbc_esp32_gpio_get_level);
  mrbc_define_method(vm, mrbc_class_esp32_gpio, "nop",             mrbc_nop);
}