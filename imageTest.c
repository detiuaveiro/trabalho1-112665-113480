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
  

  ImageInit();

  printf("# LOAD image");
  InstrReset(); // Reset instrumentation
  Image img1 = ImageLoad(argv[1]);
  printf("\nHeight: %d, Width: %d\n", ImageHeight(img1), ImageWidth(img1));

  printf("\n# CROP image - New image has has coordinates (0,0) and 150x150 size.");
  Image cp1 = ImageCrop(img1, 0, 0, 150, 150);
  printf("\nCropped Image - Height: %d, Width: %d\n", ImageHeight(cp1), ImageWidth(cp1));

  printf("\n# LOCATE subimage");
  int x, y;
  InstrReset(); // Reset instrumentation
  if (ImageLocateSubImage(img1, &x, &y, cp1)) {
    printf("\nSubimage found at coordinates - x: %d, y: %d\n", x, y);
  } else {
    printf("\nSubimage not found.\n");
  }
  InstrPrint(); // Print instrumentation
  // Free memory
  ImageDestroy(&img1);
  ImageDestroy(&cp1);

  ///// New set of images
  Image img2 = ImageLoad(argv[1]);
  printf("\nHeight: %d, Width: %d\n", ImageHeight(img2), ImageWidth(img2));

  printf("\n# CROP image - New image has coordinates (150,150) and 149x149 size.");
  Image cp2 = ImageCrop(img2, 150, 150, 149, 149);
  printf("\nCropped Image - Height: %d, Width: %d\n", ImageHeight(cp2), ImageWidth(cp2));

  printf("\n# LOCATE subimage");
  int x2, y2;
  InstrReset(); // Reset instrumentation
  if (ImageLocateSubImage(img2, &x2, &y2, cp2)) {
    printf("\nSubimage found at coordinates - x: %d, y: %d\n", x2, y2);
  } else {
    printf("\nSubimage not found.\n");
  }
  InstrPrint(); // Print instrumentation
  // Free memory
  ImageDestroy(&img2);
  ImageDestroy(&cp2);

  Image img3 = ImageLoad(argv[1]);
  printf("\nHeight: %d, Width: %d\n", ImageHeight(img3), ImageWidth(img3));

  printf("\n# CROP image - New image has coordinates (0,0) and 299x299 size.");
  Image cp3 = ImageCrop(img3, 0, 0, 299, 299);
  printf("\nCropped Image - Height: %d, Width: %d\n", ImageHeight(cp3), ImageWidth(cp3));

  printf("\n# LOCATE subimage");
  int x3, y3;
  InstrReset(); // Reset instrumentation
  if (ImageLocateSubImage(img3, &x3, &y3, cp3)) {
    printf("\nSubimage found at coordinates - x: %d, y: %d\n", x3, y3);
  } else {
    printf("\nSubimage not found.\n");
  }
  InstrPrint(); // Print instrumentation
  // Free memory
  ImageDestroy(&img3);
  ImageDestroy(&cp3);

  return 0;
}