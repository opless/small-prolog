#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "prtypes.h"



    
char *os_alloc(zone_size_t how_much);
void ini_term();
void exit_term();
void errmsg(char *s);
int tty_getc();
int tty_getche();
int tty_pr_string(char *s);
int pr_string(char *s);
int read_config( zone_size_t *hsp,zone_size_t *strsp,zone_size_t *dsp,zone_size_t *tsp,zone_size_t *sbsp,zone_size_t *tmpsp,int *rsp,int *psp);
int more_y_n();
int read_yes();
//long clock(); // is there in osx.
