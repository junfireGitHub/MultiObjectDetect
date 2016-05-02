#ifndef SETTING_H_
#define SETTING_H_
#pragma once

//#define NDEBUG  // if define, disable assert

#define SHRINK (4u)
#define BIN_SIZE (4u)
#define NUM_ORIENT (6u)

#define MAX_IN_IMG_R (1200u)
#define MAX_IN_IMG_C (1200u)
#define MAX_IN_IMG_SIZE (MAX_IN_IMG_R*MAX_IN_IMG_C)   //the max size of input image

#define MAX_CHN_FTR_DIM (9216u)

//#define TRAIN_IMG_R (192u)
//#define TRAIN_IMG_C  (96u)

//#define MAX_CHN_FTR_DIM ((TRAIN_IMG_R / SHRINK)*(TRAIN_IMG_C / SHRINK) + \
//	                 (TRAIN_IMG_R / SHRINK)*(TRAIN_IMG_C / SHRINK) + \
//	                 (TRAIN_IMG_R / BIN_SIZE)* (TRAIN_IMG_C / BIN_SIZE)*NUM_ORIENT)

//#define CHN_NUM (8u)
//#define GRAD_MAG_CHN_ID (6u)
//#define COLOR_CHN_ID (7u)

#endif 