#pragma once
char * parserr(char *s);
void fatal( char *s);
void fatal2( char *s,char *s2);
void internal_error(char *filename, int linenumber, char *s);
void argerr(int narg, char *msg);
int nargerr(int narg);
int typerr(int narg, objtype_t type);
