#include <iostream>

#include "Controller.h"
#include "PCA9685.h"
#include "CmdLine.h"
#include "keypress.h"

#include <unistd.h>
#include <signal.h>

#include "Slog.h"

static sigset_t sigs_to_catch; // Signals

// Globals for testing
int g_heartbeat_driver;

int main(int argc, char *argv[])
{
  int sig_caught; // signal caught

  SLOG << "" << std::endl;
  SLOG << "++++++++++++++++++++++++++++++++++++++++ Hello Robbie ++++++++++++++++++++++++++++++++++++++++" << std::endl;

  CmdLine::Instance()->Parse(argc, argv);

  // Signals turn off control-c, etc
	sigemptyset(&sigs_to_catch);
	sigaddset(&sigs_to_catch, SIGINT);
	sigaddset(&sigs_to_catch, SIGQUIT);
  sigaddset(&sigs_to_catch, SIGKILL);
  sigaddset(&sigs_to_catch, SIGTERM);
  sigaddset(&sigs_to_catch, SIGSEGV);

    // Block all above signals from all spawned threads
  if (pthread_sigmask(SIG_BLOCK, &sigs_to_catch, NULL) != 0)
  {
    SLOG << "main::pthread_sigmask() failed" << std::endl;
  }
  // Signals are now off so we can create threads

  // Do this here so we initialize PCA9685
  PCA9685::Instance();

  Controller* p_controller = new Controller();

  // Now we can wait for a signal to stop us
  cout << "Waiting for signal" << std::endl;
  // But the Balancer thread, main and in our case, waits on those signals to
  // catch them. Wait for a signal to stop, normally control-c
  sigwait(&sigs_to_catch, &sig_caught);

  cout << "Caught Signal=" << sig_caught << std::endl;
  
  // Got a signal so shut things down
  delete p_controller;

  SLOG << "---------------------------------------- Bye Robbie ----------------------------------------" << std::endl;
    
  return(0);
}
