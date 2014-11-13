#include "XPCOMRTInit.h"
#include <stdio.h>

#include "nsISupports.h"
#include "nsRefPtr.h"
#include "nsCOMPtr.h"
#include "nsComponentManagerUtils.h"
#include "nsAutoPtr.h"
#include "nsNetCID.h"
#include "mozilla/Mutex.h"
#include "iTest.h"
#include "nsIRunnable.h"
#include "nsIThread.h"
#include "nsITimer.h"
#include "nsThreadUtils.h"
#include "nsISocketTransportService.h"
#include "nsServiceManagerUtils.h"

#include <stdio.h>

class Runner : public nsIRunnable {
public:
  NS_DECL_THREADSAFE_ISUPPORTS

  NS_IMETHOD Run() {
      nsCOMPtr<nsIThread> thread;
      nsresult rv = NS_GetCurrentThread(getter_AddRefs(thread));
      if (NS_FAILED(rv)) {
          fprintf(stderr, "failed to get current thread\n");
          return rv;
      }
      fprintf(stderr, "running %d on thread %p\n", mNum, (void *)thread.get());
      return rv;
  }

  Runner(int num) : mNum(num) {}

protected:
  virtual ~Runner() {}
  int mNum;
};

NS_IMPL_ISUPPORTS(Runner, nsIRunnable)

class TimerCallback MOZ_FINAL : public nsITimerCallback
{
public:
  NS_DECL_THREADSAFE_ISUPPORTS

  TimerCallback(nsCOMPtr<nsIThread>& aTarget, bool& aQuit) : mTarget(aTarget), mQuit(aQuit), mCount(0) {}

  NS_IMETHOD Notify(nsITimer* aTimer) {
    fprintf(stderr, "Got timer callback\n");
    nsRefPtr<Runner> runner = new Runner(mCount);
    mCount++;
    mTarget->Dispatch(runner, NS_DISPATCH_NORMAL);

    if (mCount >= 5) {
      mQuit = true;
    }

    return NS_OK;
  }
private:
  ~TimerCallback() {}
  nsCOMPtr<nsIThread> mTarget;
  bool& mQuit;
  int mCount;
};

NS_IMPL_ISUPPORTS(TimerCallback, nsITimerCallback)

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

  nsCOMPtr<nsISocketTransportService> stservice = do_GetService(NS_SOCKETTRANSPORTSERVICE_CONTRACTID, &rv);

  nsCOMPtr<nsIThread> thread;
  rv = NS_NewThread(getter_AddRefs(thread));

  bool quit = false;

  nsCOMPtr<nsITimer> timer = do_CreateInstance(NS_TIMER_CONTRACTID, &rv);

  nsCOMPtr<nsITimerCallback> callback = new TimerCallback(thread, quit);

  rv = timer->InitWithCallback(callback, PR_MillisecondsToInterval(1000),
                               nsITimer::TYPE_REPEATING_PRECISE);

  while(!quit) {
    NS_ProcessNextEvent();
  }

  NS_ShutdownXPCOMRT();
  return 0;
}
