#ifndef MAIN_H
#define MAIN_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

char * symbols [7];

#define SKleene             symbols[0]
#define SQuestion           symbols[1]
#define SPlus               symbols[2]
#define SOr                 symbols[3]
#define SConcat             symbols[4]
#define SLeft               symbols[5]
#define SRight              symbols[6]
#define SFinal              symbols[7]

#endif
