#include <iostream>

#include "Controller.h"
#include "PCA9685.h"
#include "keypress.h"

#include <unistd.h>
#include <signal.h>

#include "Config.h"
#include "Slog.h"

static sigset_t sigs_to_catch; // Signals

// Globals for testing
int g_heartbeat_driver;

int main(int argc, char *argv[])
{
  int sig_caught; // signal caught
  double knee_angle = KNEE_ANGLE;
  int leg_offset = LEG_OFFSET;

  SLOG << "" << std::endl;
  SLOG << "++++++++++++++++++++++++++++++++++++++++ Hello Robbie ++++++++++++++++++++++++++++++++++++++++" << std::endl;

  // Handle passed Controller parameters for legs
  if (argc == 3)
  {
    // std::cout << "argv[0]=" << argv[0] << " argv[1]=" << argv[1]<< " argv[2]=" << argv[2] << std::endl;
    knee_angle = atof(argv[1]);
    leg_offset = atoi(argv[2]);
    SLOG << "knee_angle=" << knee_angle << " leg_offset=" << leg_offset << std::endl;
  }
  else if (argc != 1 && argc != 3)
  {
    SLOG << "ERROR: parameters leg angles and offset required, argc must be 1 or 3 argc=" << argc << std::endl;
  }

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

  Controller* p_controller = new Controller(knee_angle, leg_offset);

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
