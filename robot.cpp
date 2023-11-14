#include <iostream>

#include "Controller.h"
#include "PCA9685.h"
#include "keypress.h"

#include <unistd.h>
#include <signal.h>

#include "Config.h"
#include "Slog.h"

static sigset_t sigs_to_catch; // Signals

int main(int argc, char *argv[])
{
  int sig_caught; // signal caught
  double knee_angle = KNEE_ANGLE;
  int leg_offset = LEG_OFFSET;

  double pid_kp = PID_Kp;
  double pid_ki = PID_Ki;
  double pid_kd = PID_Kd;

  SLOG << "" << std::endl;
  SLOG << "++++++++++++++++++++++++++++++++++++++++ Hello Robbie ++++++++++++++++++++++++++++++++++++++++" << std::endl;

  for(;;)
  {
    switch(getopt(argc, argv, "a:o:p:i:d:h")) // note the colon (:) to indicate that 'b' has a parameter and is not a switch
    {
      case 'a': // Knee angle
        printf("switch 'a' specified with the value %s\n", optarg);
        knee_angle = atof(optarg);
        continue;

      case 'o': // Leg offset
        printf("switch 'o' specified with the value %s\n", optarg);
        leg_offset = atoi(optarg);
        continue;

      case 'p':
        printf("switch 'p' specified with the value %s\n", optarg);
        pid_kp = atof(optarg);
        continue;

      case 'i':
        printf("parameter 'i' specified with the value %s\n", optarg);
        pid_ki = atof(optarg);
        continue;

      case 'd':
        printf("parameter 'd' specified with the value %s\n", optarg);
        pid_kd = atof(optarg);
        continue;

      case '?':
      case 'h':
      default :
        printf("Usage\n");
        printf("robot -a 90.0 -o -30 -p 833 -i 5 -d 30 -h -H\n");
        printf("-a leg knee angle -o leg wheel offset from center of robot\n");
        printf("-p proportional -i integral -d derivative\n");
        printf("-h help\n");
        break;

      case -1:
        break;
    }

    break;
  }

  std::cout << "knee_angle=" << knee_angle << " leg_offset=" << leg_offset << std::endl;
  std::cout << "pid_kp=" << pid_kp << std::endl;;
  std::cout << "pid_ki=" << pid_ki << std::endl;;
  std::cout << "pid_kd=" << pid_kd << std::endl;;

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

  Controller* p_controller = new Controller(knee_angle, leg_offset, pid_kp, pid_ki, pid_kd);

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
