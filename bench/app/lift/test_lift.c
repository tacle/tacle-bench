#include <stdio.h>
#include <stdint.h>

#include "lift_io.h"
#include "lift_control.h"

/* Include Tst Data */
#include "test_lift.data.h"

__attribute__((noinline))
void controller() {
  ctrl_get_vals();
  ctrl_loop();
  ctrl_set_vals();
}

int main()
{
  int i;
  ctrl_init();
  for(i = 0; i < TEST_COUNT; i++)
    {
      (inits[i])();
      controller();
      //      main_();
    }
  return 0;
}
