#include "chess.h"

void Logger::Open()
{
  strcpy(filename,"/home/jrotter/git/chessica/logs/chessica.log");
  logfile = fopen(filename,"w");
}

void Logger::Write(const char *logtext)
{
  fprintf(logfile,logtext);
  fflush(logfile);
}

void Logger::Write(char *logtext)
{
  fprintf(logfile,logtext);
  fflush(logfile);
}

void Logger::Close()
{
  fclose(logfile);
}

