#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>

#include"keypress.h"

struct termios oldSettings, newSettings;

char local_getkey()
{
  char c = 0;

  fd_set set;
  struct timeval tv;

  tv.tv_sec = 10;
  tv.tv_usec = 0;

  FD_ZERO( &set );
  FD_SET( fileno( stdin ), &set );

  int res = select( fileno( stdin )+1, &set, NULL, NULL, &tv );

  if( res > 0 )
  {
    // printf( "Input available\n" );
    read( fileno( stdin ), &c, 1 );
    // printf( "Char %c\n", c);
  }
  else if( res < 0 )
  {
    perror( "select error" );
    return(0);
  }
  else
  {
    printf( "Select timeout\n" );
  }

  return(c);
}

void local_open_keypress()
{
  tcgetattr( fileno( stdin ), &oldSettings );
  newSettings = oldSettings;
  newSettings.c_lflag &= (~ICANON & ~ECHO);
  tcsetattr( fileno( stdin ), TCSANOW, &newSettings );
}

void local_close_keypress()
{
  tcsetattr( fileno( stdin ), TCSANOW, &oldSettings );
}

char getkey()
{
  char c = 0;

  // Grab terminal IO
  tcgetattr( fileno( stdin ), &oldSettings );
  newSettings = oldSettings;
  newSettings.c_lflag &= (~ICANON & ~ECHO);
  tcsetattr( fileno( stdin ), TCSANOW, &newSettings );

  fd_set set;
  struct timeval tv;

  tv.tv_sec = 10;
  tv.tv_usec = 0;

  FD_ZERO( &set );
  FD_SET( fileno( stdin ), &set );

  int res = select( fileno( stdin )+1, &set, NULL, NULL, &tv );

  if( res > 0 )
  {
    // printf( "Input available\n" );
    read( fileno( stdin ), &c, 1 );
    // printf( "Char %c\n", c);
  }
  else if( res < 0 )
  {
    perror( "select error" );
    return(0);
  }
  else
  {
    printf( "Select timeout\n" );
  }

  // Set terminal back to normal
  tcsetattr( fileno( stdin ), TCSANOW, &oldSettings );

  return(c);
}

// int test_main()
// {
//   open_keypress();

//   while ( 1 )
//   {
//     printf("Key press: %c\n", getkey());
//   }

//   close_keypress();

//   return 0;
// }
