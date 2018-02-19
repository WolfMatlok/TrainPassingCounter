#include <iostream>
#include "cUnitTest.h"
#include "View.h"

 
int main ( int argc,char **argv )
{
#if 0
  cUnitTest::DoAllTests();
  return 0;
#endif
  
  
  return View(argc, argv).run();
}