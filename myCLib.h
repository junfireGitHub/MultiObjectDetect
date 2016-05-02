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

typedef unsigned char UInt8;
typedef unsigned int uint;

typedef struct rect{

	int x, y, width, height;
}rect;

typedef struct ChnRect{
	
	int x, y, width, height, chnId;
}ChnRect;


void imResizeFloat(
	const float *img,
	uint imgR, uint imgC,
	uint afterZoomR, uint afterZoomC,
	float *afterZoomImg);

void imResizeUInt8(
	const UInt8 *img,
	uint imgR, uint imgC,
	uint afterZoomR, uint afterZoomC,
	float *afterZoomImg);

void outputImg(const UInt8 *img, uint imgR, uint imgC);

void printRect(rect rec);

void drawRect(UInt8 *img, int imgR, int imgC, rect r);

#endif