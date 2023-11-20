// imageTest - A program that performs some image processing.
//
// This program is an example use of the image8bit module,
// a programming project for the course AED, DETI / UA.PT
//
// You may freely use and modify this code, NO WARRANTY, blah blah,
// as long as you give proper credit to the original and subsequent authors.
//
// João Manuel Rodrigues <jmr@ua.pt>
// 2023

#include <assert.h>
#include <errno.h>
#include "error.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "image8bit.h"
#include "instrumentation.h"
#define PIXMEM InstrCount[0]
#define PIXCOMP InstrCount[1]

int main(int argc, char* argv[]) {
  if (argc != 3) {
    error(1, 0, "Usage: imageTest input.pgm output.pgm");
  }

  ImageInit();
  
  printf("# LOAD image");
  InstrReset(); // to reset instrumentation
  Image img1 = ImageLoad(argv[1]);
  Image cp1 = ImageCrop(img1, 0, 0, 150, 150);
  int inicial = PIXMEM;
  int *x = malloc(sizeof(int));
  int *y  = malloc(sizeof(int));
  ImageLocateSubImage(img1, x, y, cp1);
  InstrPrint(); 
  
  InstrReset();
  printf("# NORMAL BLUR image");
  ImageBlur(cp1,)
  
  
  
  // to print instrumentation
  // Try changing the behaviour of the program by commenting/uncommenting
  // the appropriate lines.

  //img2 = ImageCrop(img1, ImageWidth(img1)/4, ImageHeight(img1)/4, ImageWidth(img1)/2, ImageHeight(img1)/2);
  //Image img2 = ImageRotate(img1);
  //if (img2 == NULL) {
  //  error(2, errno, "Rotating img2: %s", ImageErrMsg());
  //}

  //ImageNegative(img2);
  //ImageThreshold(img2, 100);
  //ImageBrighten(img2, 1.3);
  //if (ImageSave(img2, argv[2]) == 0) {
   // error(2, errno, "%s: %s", argv[2], ImageErrMsg());
  //}

  //ImageDestroy(&img1);
  //ImageDestroy(&img2);
  return 0;
}

