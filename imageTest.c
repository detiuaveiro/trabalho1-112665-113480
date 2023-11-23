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
  ImageDestroy(&cp1);

  ///// New set of images
  Image img2 = ImageLoad(argv[1]);
  printf("\nHeight: %d, Width: %d\n", ImageHeight(img2), ImageWidth(img2));

  printf("\n# CROP image - New image has coordinates (150,150) and 149x149 size.");
  Image cp2 = ImageCrop(img2, 200, 200, 100, 100);
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
  Image cp3 = ImageCrop(img3, 15, 15, 15, 15);
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

  Image img4 = ImageLoad(argv[2]);
  printf("\nHeight: %d, Width: %d\n", ImageHeight(img4), ImageWidth(img4));

  printf("\n# CROP image - New image has coordinates (384,384) and 128x128 size which will be 1/4 of the original image.");
  Image cp4 = ImageCrop(img4, 384, 384, 128, 128);
  printf("\nCropped Image - Height: %d, Width: %d\n", ImageHeight(cp4), ImageWidth(cp4));

  printf("\n# LOCATE subimage");
  int x4, y4;
  InstrReset(); // Reset instrumentation
  if (ImageLocateSubImage(img4, &x4, &y4, cp4)) {
    printf("\nSubimage found at coordinates - x: %d, y: %d\n", x4, y4);
  } else {
    printf("\nSubimage not found.\n");
  }
  InstrPrint(); // Print instrumentation
  // Free memory
  ImageDestroy(&img4);
  ImageDestroy(&cp4);
    Image img5 = ImageLoad(argv[2]);
  printf("\nHeight: %d, Width: %d\n", ImageHeight(img5), ImageWidth(img5));

  printf("\n# CROP image - New image has coordinates (448, 448) and 64X64 size which will be 1/8 of the original image.");
  Image cp5 = ImageCrop(img5, 448, 448, 64, 64);
  printf("\nCropped Image - Height: %d, Width: %d\n", ImageHeight(cp5), ImageWidth(cp5));

  printf("\n# LOCATE subimage");
  int x5, y5;
  InstrReset(); // Reset instrumentation
  if (ImageLocateSubImage(img5, &x5, &y5, cp5)) {
    printf("\nSubimage found at coordinates - x: %d, y: %d\n", x5, y5);
  } else {
    printf("\nSubimage not found.\n");
  }
  InstrPrint(); // Print instrumentation
  // Free memory
  ImageDestroy(&cp5);
  Image img6 = ImageLoad(argv[2]);
  printf("\nHeight: %d, Width: %d\n", ImageHeight(img6), ImageWidth(img6));

  printf("\n# CROP image - New image has coordinates (480, 480) and 32X32 size which will be 1/16 of the original image.");
  Image cp6 = ImageCrop(img6, 480, 480, 32, 32);
  printf("\nCropped Image - Height: %d, Width: %d\n", ImageHeight(cp6), ImageWidth(cp6));

  printf("\n# LOCATE subimage");
  int x6, y6;
  InstrReset(); // Reset instrumentation
  if (ImageLocateSubImage(img6, &x6, &y6, cp6)) {
    printf("\nSubimage found at coordinates - x: %d, y: %d\n", x6, y6);
  } else {
    printf("\nSubimage not found.\n");
  }
  InstrPrint(); // Print instrumentation
  // Free memory
  ImageDestroy(&img6);
  ImageDestroy(&cp6);
  Image img7 = ImageLoad(argv[2]);
  printf("\nHeight: %d, Width: %d\n", ImageHeight(img7), ImageWidth(img7));

  printf("\n# CROP image - New image has coordinates (496, 496) and 16X16 size which will be 1/32 of the original image.");
  Image cp7 = ImageCrop(img7, 496, 496, 16, 16);
  printf("\nCropped Image - Height: %d, Width: %d\n", ImageHeight(cp7), ImageWidth(cp7));

  printf("\n# LOCATE subimage");
  int x7, y7;
  InstrReset(); // Reset instrumentation
  if (ImageLocateSubImage(img7, &x7, &y7, cp7)) {
    printf("\nSubimage found at coordinates - x: %d, y: %d\n", x7, y7);
  } else {
    printf("\nSubimage not found.\n");
  }
  InstrPrint(); // Print instrumentation
  // Free memory
  ImageDestroy(&img7);
  ImageDestroy(&cp7);
      Image img8 = ImageLoad(argv[2]);
  printf("\nHeight: %d, Width: %d\n", ImageHeight(img8), ImageWidth(img8));

  printf("\n# CROP image - New image has coordinates (504, 504) and 8x8 size which will be 1/64 of the original image.");
  Image cp8 = ImageCrop(img8, 504, 504, 8, 8);
  printf("\nCropped Image - Height: %d, Width: %d\n", ImageHeight(cp8), ImageWidth(cp8));

  printf("\n# LOCATE subimage");
  int x8, y8;
  InstrReset(); // Reset instrumentation
  if (ImageLocateSubImage(img8, &x8, &y8, cp8)) {
    printf("\nSubimage found at coordinates - x: %d, y: %d\n", x8, y8);
  } else {
    printf("\nSubimage not found.\n");
  }
  InstrPrint(); // Print instrumentation
  // Free memory
  ImageDestroy(&img8);
  ImageDestroy(&cp8);
        Image img9 = ImageLoad(argv[2]);
  printf("\nHeight: %d, Width: %d\n", ImageHeight(img9), ImageWidth(img9));

  printf("\n# CROP image - New image has coordinates (508, 508) and 4x4 size which will be 1/128 of the original image.");
  Image cp9 = ImageCrop(img9, 508, 508, 4, 4);
  printf("\nCropped Image - Height: %d, Width: %d\n", ImageHeight(cp9), ImageWidth(cp9));

  printf("\n# LOCATE subimage");
  int x9, y9;
  InstrReset(); // Reset instrumentation
  if (ImageLocateSubImage(img9, &x9, &y9, cp9)) {
    printf("\nSubimage found at coordinates - x: %d, y: %d\n", x9, y9);
  } else {
    printf("\nSubimage not found.\n");
  }
  InstrPrint(); // Print instrumentation
  // Free memory
  ImageDestroy(&img9);
  ImageDestroy(&cp9);
  Image img10 = ImageLoad(argv[2]);
  printf("\nHeight: %d, Width: %d\n", ImageHeight(img10), ImageWidth(img10));

  printf("\n# CROP image - New image has coordinates (510, 510) and 2x2 size which will be 1/128 of the original image.");
  Image cp10 = ImageCrop(img10, 510, 510, 2, 2);
  printf("\nCropped Image - Height: %d, Width: %d\n", ImageHeight(cp10), ImageWidth(cp10));

  printf("\n# LOCATE subimage");
  int x10, y10;
  InstrReset(); // Reset instrumentation
  if (ImageLocateSubImage(img10, &x10, &y10, cp10)) {
    printf("\nSubimage found at coordinates - x: %d, y: %d\n", x10, y10);
  } else {
    printf("\nSubimage not found.\n");
  }
  InstrPrint(); // Print instrumentation
  // Free memory
  ImageDestroy(&img10);
  ImageDestroy(&cp10);
  Image img11 = ImageLoad(argv[2]);
  printf("\nHeight: %d, Width: %d\n", ImageHeight(img11), ImageWidth(img11));

  printf("\n# CROP image - New image has coordinates (509, 509) and 3x3 size which will be _____ of the original image.");
  Image cp11 = ImageCrop(img11, 509, 509, 3, 3);
  printf("\nCropped Image - Height: %d, Width: %d\n", ImageHeight(cp11), ImageWidth(cp11));

  printf("\n# LOCATE subimage");
  int x11, y11;
  InstrReset(); // Reset instrumentation
  if (ImageLocateSubImage(img11, &x11, &y11, cp11)) {
    printf("\nSubimage found at coordinates - x: %d, y: %d\n", x11, y11);
  } else {
    printf("\nSubimage not found.\n");
  }
  InstrPrint(); // Print instrumentation
  // Free memory
  ImageDestroy(&img11);
  ImageDestroy(&cp11);
  Image img12 = ImageCreate(2000,2000,255);
  ImageSetPixel(img12,1999,1999,255);
  printf("\nHeight: %d, Width: %d\n", ImageHeight(img12), ImageWidth(img12));

  printf("\n# CROP image - New image has coordinates (511, 511) and 1x1 size which will be _____ of the original image.");
  Image cp12 = ImageCrop(img12, 1999, 1999, 1, 1);
  printf("\nCropped Image - Height: %d, Width: %d\n", ImageHeight(cp12), ImageWidth(cp12));

  printf("\n# LOCATE subimage");
  int x12, y12;
  InstrReset(); // Reset instrumentation
  if (ImageLocateSubImage(img12, &x12, &y12, cp12)) {
    printf("\nSubimage found at coordinates - x: %d, y: %d\n", x12, y12);
  } else {
    printf("\nSubimage not found.\n");
  }
  InstrPrint(); // Print instrumentation
  // Free memory
  ImageDestroy(&img12);
  ImageDestroy(&cp12);
  Image img13 = ImageLoad(argv[3]);
  printf("\nHeight: %d, Width: %d\n", ImageHeight(img13), ImageWidth(img13));

  printf("\n# CROP image - New image has coordinates (1500, 1500) and 100x100 size which will be  of the original image.");
  Image cp13 = ImageCrop(img13, 1575, 1175, 25, 25);
  printf("\nCropped Image - Height: %d, Width: %d\n", ImageHeight(cp13), ImageWidth(cp13));

  printf("\n# LOCATE subimage");
  int x13, y13;
  InstrReset(); // Reset instrumentation
  if (ImageLocateSubImage(img13, &x13, &y13, cp13)) {
    printf("\nSubimage found at coordinates - x: %d, y: %d\n", x13, y13);
  } else {
    printf("\nSubimage not found.\n");
  }
  InstrPrint(); // Print instrumentation
  // Free memory
  ImageDestroy(&img13);
  ImageDestroy(&cp13);
  int px, py;
      printf("# Teste da função ImageLocateSubImage\n");

  //criar uma imagem branca com o pixel ultimo pixel preto
  Image branca = ImageCrop(img5, 0, 0, ImageWidth(img1), ImageHeight(img1));
  ImageThreshold(branca, 0);

    //ciclo para criar varias janelas e testar a função
  for (int width = 1; width < ImageWidth(branca); width*=2) {
    //criar uma janela para o melhor cenário
    Image subBest = ImageCrop(branca, 0, 0, width, width);  
    //criar uma janela para o pior cenário
    Image subWorst = ImageCrop(branca, 0, 0, width, width);
    ImageSetPixel(subWorst, ImageWidth(subWorst)-1, ImageHeight(subWorst)-1, 0);  
    InstrReset(); // to reset instrumentation
    printf("\n# IMAGELOCATESUBIMAGE BEST CASE (size: %d)\n", width);
    ImageLocateSubImage(branca, &px, &py, subBest);
    InstrPrint();  
    InstrReset(); // to reset instrumentation
    printf("\n# IMAGELOCATESUBIMAGE WORST CASE (size: %d)\n", width);
    ImageLocateSubImage(branca, &px, &py, subWorst);
    InstrPrint();  
    printf("\n");
  }
  InstrReset();
  Image img14 = ImageLoad(argv[1]);
  printf("\nHeight: %d, Width: %d\n", ImageHeight(img14), ImageWidth(img14));
  ImageBlur(img14,5,5);
  printf("\nBlurred Image - Height: %d, Width: %d\n", ImageHeight(img14), ImageWidth(img14));
  InstrPrint();

  InstrReset();
  Image img18 = ImageLoad(argv[1]);
  printf("\nHeight: %d, Width: %d\n", ImageHeight(img18), ImageWidth(img18));
  OldImageBlur(img18,5,5);
  printf("\nOldBlurred Image - Height: %d, Width: %d\n", ImageHeight(img18), ImageWidth(img18));
  InstrPrint();


  InstrReset();
  Image img15 = ImageLoad(argv[1]);
  printf("\nHeight: %d, Width: %d\n", ImageHeight(img15), ImageWidth(img15));
  ImageBlur(img15,20,20);
  printf("\nBlurred Image - Height: %d, Width: %d\n", ImageHeight(img15), ImageWidth(img15));
  InstrPrint();

  InstrReset();
  Image img19 = ImageLoad(argv[1]);
  printf("\nHeight: %d, Width: %d\n", ImageHeight(img19), ImageWidth(img19));
  OldImageBlur(img19,20,20);
  printf("\nOldBlurred Image - Height: %d, Width: %d\n", ImageHeight(img19), ImageWidth(img19));
  InstrPrint();

  InstrReset();
  Image img16 = ImageLoad(argv[1]);
  printf("\nHeight: %d, Width: %d\n", ImageHeight(img16), ImageWidth(img16));
  ImageBlur(img16,40,40);
  printf("\nBlurred Image - Height: %d, Width: %d\n", ImageHeight(img16), ImageWidth(img16));
  InstrPrint();

  InstrReset();
  Image img20 = ImageLoad(argv[1]);
  printf("\nHeight: %d, Width: %d\n", ImageHeight(img20), ImageWidth(img20));
  OldImageBlur(img20,40,40);
  printf("\nOldBlurred Image - Height: %d, Width: %d\n", ImageHeight(img20), ImageWidth(img20));
  InstrPrint();

  InstrReset();
  Image img17 = ImageLoad(argv[2]);
  printf("\nHeight: %d, Width: %d\n", ImageHeight(img17), ImageWidth(img17));
  ImageBlur(img17,500,400);
  printf("\nBlurred Image - Height: %d, Width: %d\n", ImageHeight(img17), ImageWidth(img17));
  InstrPrint();

  InstrReset();
  Image img21 = ImageLoad(argv[2]);
  printf("\nHeight: %d, Width: %d\n", ImageHeight(img21), ImageWidth(img21));
  OldImageBlur(img21,50,20);
  printf("\nOldBlurred Image - Height: %d, Width: %d\n", ImageHeight(img21), ImageWidth(img21));
  InstrPrint();
  //funciona se forem valores de dx e dy baixo tipo 50,20 se for 500 200 fica bue tempo e nao corre
  return 0;

}