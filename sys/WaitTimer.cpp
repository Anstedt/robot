/*******************************************************************************
PACKAGE: Robot
FILE:    WaitTimer.cpp

PURPOSE: Driver controlled timer
*******************************************************************************/

/* NOTES ***********************************************************************
*******************************************************************************/

/* INCLUDE ********************************************************************/
#include <unistd.h>
#include <fcntl.h>

#include "WaitTimer.h"

#include "Slog.h"

#define STEP_WAIT_FILE "/sys/devices/platform/soc/fe20c000.pwm/wait_timer"

/* METHODS ********************************************************************/
/*------------------------------------------------------------------------------
FUNCTION:  WaitTimer::WaitTimer()
------------------------------------------------------------------------------*/
WaitTimer::WaitTimer()
{
  SLOG << "WaitTimer::WaitTimer()" << std::endl;
  m_waittimer_fd = open(STEP_WAIT_FILE, O_WRONLY | O_SYNC);  /* might need root access */
	if (m_waittimer_fd < 0)
  {
		SLOG << "ERROR: could not open wait timer fd from " << STEP_WAIT_FILE << std::endl;
	}
  else
  {
    SLOG << "WaitTimer::WaitTimer() success file=" << STEP_WAIT_FILE << " handle=" << m_waittimer_fd << std::endl;
    lseek(m_waittimer_fd, 0, SEEK_SET); // Make sure we start at the Beginning
  }  

  return;
}

/*------------------------------------------------------------------------------
FUNCTION:  WaitTimer::~WaitTimer()
------------------------------------------------------------------------------*/
WaitTimer::~WaitTimer()
{
  if (m_waittimer_fd >= 0)
    close(m_waittimer_fd);
}

/*------------------------------------------------------------------------------
FUNCTION:  WaitTimer::Sleep()

ARGUMENTS: None
RETURNS:   None
------------------------------------------------------------------------------*/
bool WaitTimer::Sleep(s32 sleep_in_us)
{
  bool status = true;
  
  lseek(m_waittimer_fd, 0, SEEK_SET);

  // Convert us to ns
  m_wait.wait_time = sleep_in_us * 1000;

  if (write(m_waittimer_fd, &m_wait, sizeof(m_wait)) != sizeof(m_wait))
  {
    SLOG << "ERROR: write to wait timer handle=" << m_waittimer_fd << " failed" << std::endl;
    status = false;
  }

  return(status);
}
