#include "chess.h"

FILE *logfile;

/* I'm assuming that this will only get called if we're still in an opening */
void InitLogFile()
{
  char filename[30] = LOG_NAME;

  logfile = fopen(filename,"w");

  if (logfile == (FILE *) 0) {
    printf("COULDN'T OPEN LOGFILE!\n");
    exit(1);
  }
}

void ForceLogFlush()
{
  fflush(logfile);
}

