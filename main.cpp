#include "XPCOMRTInit.h"

#include <stdio.h>

int
main(int argc, char* argv[])
{
  NS_InitXPCOMRT();

  NS_ShutdownXPCOMRT();
  return 0;
}
