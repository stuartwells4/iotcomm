#include "bledev.h"
#include "blesetup.h"

int main(int argc, char *argv[])
{
  bledev dev = bledev::get_instance(); 
  blesetup setup = blesetup::get_instance();

  setup.start();
  dev.start();
  setup.power_on();
  setup.process();

  //  printf("Do we have an instance %p\n", ble);

  return 0;
}
