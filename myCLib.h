#ifndef MYLIB_H_
#define MYLIB_H_
#pragma once

#include"setting.h"
#include<stdio.h>
#include<math.h>
#include<stdlib.h>
#include<string.h>
#include<memory.h>
#include<assert.h>
#include<time.h>
#include<ctype.h>

typedef unsigned char UInt8;
typedef unsigned char Uint8;
typedef unsigned int uint32;

typedef struct rect{
	int x, y, width, height;
}rect;

void drawRect(UInt8 *img, int imgR, int imgC, rect r);

#endif