#include "image8bit.h"

#include <assert.h>
#include <ctype.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include "instrumentation.h"
struct image {
  int width;
  int height;
  int maxval;   // maximum gray value (pixels with maxval are pure WHITE)
  uint8* pixel; // pixel data (a raster scan)
};

void KMPSearch(Image img1, Image img2, int* x, int*y){
    int tamanho1 = ImageWidth(img1)*ImageHeight(img1);
    int tamanho2 = ImageWidth(img2)*ImageHeight(img2);
    int prefixo[tamanho1];
    computeLPSArray(img2, tamanho2, prefixo);
    int i = 0;
    int j = 0;
    while(i < tamanho1){
        if(img1->pixel[i] == img2->pixel[j]){
            i++;
            j++;
        }
        if(j == tamanho2){
            *x = i - j;
            *y = i - j;
            j = prefixo[j - 1];
        }
        else if(i < tamanho1 && img1->pixel[i] != img2->pixel[j]){
            if(j != 0){
                j = prefixo[j - 1];
            }
            else{
                i++;
            }
        }
    }
}
computeLPSArray(Image img2, int tamanho2, int* prefixo){
    int len = 0;
    int i = 1;
    prefixo[0] = 0;
    while(i < tamanho2){
        if(img2->pixel[i] == img2->pixel[len]){
            len++;
            prefixo[i] = len;
            i++;
        }
        else{
            if(len != 0){
                len = prefixo[len - 1];
            }
            else{
                prefixo[i] = 0;
                i++;
            }
        }
    }
}