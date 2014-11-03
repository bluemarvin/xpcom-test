#include "XPCOMRTInit.h"
#include <stdio.h>

#include "nsISupports.h"
#include "nsRefPtr.h"
#include "nsCOMPtr.h"
#include "nsAutoPtr.h"
#include "mozilla/Mutex.h"
#include "iTest.h"

class Test : public iTest
{
public:
  NS_DECL_ISUPPORTS
  NS_DECL_ITEST

  Test();

private:
  ~Test();

protected:
};

/* Implementation file */
NS_IMPL_ISUPPORTS(Test, iTest)

Test::Test()
{
}

Test::~Test()
{
}

/* void init (); */
NS_IMETHODIMP Test::Init()
{
  fprintf(stderr, "Test::Init!\n");
  return NS_OK;
}

class test : nsISupports {

};

#include <stdio.h>

int
main(int argc, char* argv[])
{
//  NS_InitXPCOMRT();

  mozilla::Mutex mux("");
  nsRefPtr<nsISupports> ptr = new Test;
  nsCOMPtr<iTest> iptr = do_QueryObject(ptr);
  iptr->Init();

//  NS_ShutdownXPCOMRT();
  return 0;
}
