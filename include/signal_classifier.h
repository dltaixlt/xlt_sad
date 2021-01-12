/*******************************************************************************
This software module was originally developed by

HUAWEI TECHNOLOGIES CO.,LTD

Initial author: Lijing Xu
and edited by: Lijing Xu
-

Function: AAC vs LP mode decision for MPEG USAC Reference Encoder

HUAWEI TECHNOLOGIES CO.,LTD. retain full right to
modify and use the code for its (their) own purpose, assign or donate the code
to a third party and to inhibit third parties from using the code for products
that do not conform to MPEG-related ITU Recommendations and/or ISO/IEC
International Standards. This copyright notice must be included in all copies or
derivative works.

Copyright (c) ISO/IEC 2009.
$Id: signal_classifier.h,v 1.1 2011-01-20 21:15:06 mul Exp $
*******************************************************************************/

#ifndef _CLASSIFICATION_H_
#define _CLASSIFICATION_H_

#define FRAMELEN 1024
#define MIN_POW -200
#define N_TONAL_BUFF  40    
#define N_TILT_BUFF 80
#define N_SMOOTH 90

#define FD_MODE 1
#define TD_MODE 0

#ifndef PI
#define PI 3.1415926535897932f
#endif

#ifndef min
#define min(a, b) ((a) < (b) ? (a) : (b))
#endif
#ifndef max
#define max(a, b) ((a) > (b) ? (a) : (b))
#endif

typedef struct 
{
	float  input_samples[FRAMELEN];

    int    tonal_num_fr_buf[4][N_TONAL_BUFF];	//number of tonal components in four frequency regions 
												//of the latest N_TONAL_BUFF frames
	float  spec_tilt_buf[N_TILT_BUFF];			//spectral tilt of the latest N_TILT_BUFF frames
	
    int    coding_mode;		//coding mode of the current frame
	int    pre_mode;		//coding mode of the previous frame		
} CLASSIFICATION;

void init_classification(CLASSIFICATION *classfy);
void classification (CLASSIFICATION *classfy);

#endif _CLASSIFICATION_H_
