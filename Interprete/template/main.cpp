#include <systemc.h>
#include "Top.h"
___MAIN_INCLUDES___


int main(int argc, char* argv[])
{

  ___MAIN_INIT_CODE___

  Top* top = new Top("top");

  ___MAIN_CODE___

  sc_start();
  delete top;
  return 0;

}
