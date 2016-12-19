#include <iostream>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <time.h>
#include <unistd.h>

#include "GPIOClass.h"

using namespace std;

#define CLOCKID CLOCK_REALTIME
#define SIG SIGRTMIN

#define err_exit(msg) do { perror(msg); exit(EXIT_FAILURE); } while (0)

static GPIOClass gpio("4", "out");



static void sig_handler(int sig, siginfo_t* si, void* uc) {
  // TODO Change Write function to not use printf or stream operator.
  gpio.Write("1");
  gpio.Write("0");
}

int main(int argc, char *argv[]) {
  // Setup signal handler.
  struct sigaction sa;
  sa.sa_flags = SA_SIGINFO;
  sa.sa_sigaction = sig_handler;
  sigemptyset(&sa.sa_mask);
  if (sigaction(SIG, &sa, NULL) == -1)
    err_exit("sigaction");

  // Block timer signal temporarily.
/*  sigset_t mask;
  sigemptyset(&mask);
  sigaddset(&mask, SIG);
  if (sigprocmask(SIG_SETMASK, &mask, NULL) == -1)
    err_exit("sigprocmask");
*/
  // Create timer.
  timer_t timerid;
  struct sigevent sev;
  sev.sigev_notify = SIGEV_SIGNAL;
  sev.sigev_signo = SIG;
  sev.sigev_value.sival_ptr = &timerid;
  if (timer_create(CLOCKID, &sev, &timerid) == -1)
    err_exit("timer_create");

  // Start absolute timer.
  struct itimerspec its;
  long long freq_nanosecs = 10000000; // TODO from argv...
  if (clock_gettime(CLOCKID, &its.it_value) == -1)
    err_exit("clock_gettime");
  its.it_interval.tv_sec = 0;
  its.it_interval.tv_nsec = freq_nanosecs;
  if (timer_settime(timerid, TIMER_ABSTIME, &its, NULL) == -1)
    err_exit("timer_settime");

  while (1);

  return 0;
}
