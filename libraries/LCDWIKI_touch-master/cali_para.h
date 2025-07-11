#ifndef _CALI_PARA_
#define _CALI_PARA_

//for resolution 240x320,the calibration parameter is 663,-13,894,-30
//for resolution 320x480,the calibration parameter is 852,-14,1284,-30

//#define XFAC      639
//#define XOFFSET   (-11)
//#define YFAC      897
//#define YOFFSET   (-31) 

#define XFAC      (-663)  // Inverted X-axis factor
#define XOFFSET   (240 + 13)  // Adjusted X-axis offset
#define YFAC      894     // Keep Y-axis as is if no issues
#define YOFFSET   (-30)   // Keep Y-axis offset as is

#endif