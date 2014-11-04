#include "XPCOMRTInit.h"
#include <stdio.h>

#include "nsISupports.h"
#include "nsRefPtr.h"
#include "nsCOMPtr.h"
#include "nsAutoPtr.h"
#include "mozilla/Mutex.h"
#include "iTest.h"
#include "nsIRunnable.h"
#include "nsThreadUtils.h"

#include <stdio.h>

class nsRunner : public nsIRunnable {
public:
    NS_DECL_THREADSAFE_ISUPPORTS

    NS_IMETHOD Run() {
        nsCOMPtr<nsIThread> thread;
        nsresult rv = NS_GetCurrentThread(getter_AddRefs(thread));
        if (NS_FAILED(rv)) {
            printf("failed to get current thread\n");
            return rv;
        }
        printf("running %d on thread %p\n", mNum, (void *)thread.get());

        // if we don't do something slow, we'll never see the other
        // worker threads run
        PR_Sleep(PR_MillisecondsToInterval(100));

        return rv;
    }

    nsRunner(int num) : mNum(num) {
    }

protected:
    virtual ~nsRunner() {}
    int mNum;
};

NS_IMPL_ISUPPORTS(nsRunner, nsIRunnable)

class Test : public iTest
{
public:
  NS_DECL_ISUPPORTS
  NS_DECL_ITEST

  Test();

private:
  virtual ~Test();

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

int
main(int argc, char* argv[])
{
  NS_InitXPCOMRT();

  nsRefPtr<nsISupports> ptr = new Test;
  nsCOMPtr<iTest> iptr = do_QueryObject(ptr);
  iptr->Init();

  nsresult rv;

  nsCOMPtr<nsIRunnable> event = new nsRunner(0);
  if (!event)
    return 0;

  nsCOMPtr<nsIThread> runner;
  rv = NS_NewThread(getter_AddRefs(runner), event);

  NS_ShutdownXPCOMRT();
  return 0;
}
