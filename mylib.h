#ifndef MYLIB_H
#define MYLIB_H

void clrbuf (int);
long decin (void);
void decout (long number);
void hexout (unsigned long);
void newline (void);
long writeline (const char *, FILE *);

#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif

#endif

