#include <stdio.h>
#include <stdint.h>
#include "lift_io.h"
#include "lift_control.h"

__attribute__((noinline))
void controller() {
  ctrl_get_vals();
  ctrl_loop();
  ctrl_set_vals();
}

#define MAX_ITERATIONS 1000

int main()
{
  int i;
  ctrl_init();
  while(1) {
    /* zero input stimulus */
    simio_in = 0;
    simio_adc1 = 0;
    simio_adc2 = 0;
    simio_adc3 = 0;
    /* run controller */
    controller();
    if(MAX_ITERATIONS > 0 && i++ >= MAX_ITERATIONS)
      break;
  }
}
