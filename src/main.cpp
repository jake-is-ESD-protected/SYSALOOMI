#include <Arduino.h>
#include "bridge.h"

void setup() {
  err_t stat;
  stat = core_init();
  if(stat != e_no_err)
  {
    err_handler(e_core_fail, (void*)&stat);
  }

}

void loop() {

}
