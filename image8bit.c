/// image8bit - A simple image processing module.
///
/// This module is part of a programming project
/// for the course AED, DETI / UA.PT
///
/// You may freely use and modify this code, at your own risk,
/// as long as you give proper credit to the original and subsequent authors.
///
/// João Manuel Rodrigues <jmr@ua.pt>
/// 2013, 2023

// Student authors (fill in below):
// NMec:  Name:
// 112665 Tomás Brás
// 113480 Afonso Ferreira
// 
// Date: 11/11/2023
//

#include "image8bit.h"

#include <assert.h>
#include <ctype.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include "instrumentation.h"
// The data structure
//
// An image is stored in a structure containing 3 fields:
// Two integers store the image width and height.
// The other field is a pointer to an array that stores the 8-bit gray
// level of each pixel in the image.  The pixel array is one-dimensional
// and corresponds to a "raster scan" of the image from left to right,
// top to bottom.
// For example, in a 100-pixel wide image (img->width == 100),
//   pixel position (x,y) = (33,0) is stored in img->pixel[33];
//   pixel position (x,y) = (22,1) is stored in img->pixel[122].
// 
// Clients should use images only through variables of type Image,
// which are pointers to the image structure, and should not access the
// structure fields directly.

// Maximum value you can store in a pixel (maximum maxval accepted)
const uint8 PixMax = 255;

// Internal structure for storing 8-bit graymap images
struct image {
  int width;
  int height;
  int maxval;   // maximum gray value (pixels with maxval are pure WHITE)
  uint8* pixel; // pixel data (a raster scan)
};


// This module follows "design-by-contract" principles.
// Read `Design-by-Contract.md` for more details.

/// Error handling functions

// In this module, only functions dealing with memory allocation or file
// (I/O) operations use defensive techniques.
// 
// When one of these functions fails, it signals this by returning an error
// value such as NULL or 0 (see function documentation), and sets an internal
// variable (errCause) to a string indicating the failure cause.
// The errno global variable thoroughly used in the standard library is
// carefully preserved and propagated, and clients can use it together with
// the ImageErrMsg() function to produce informative error messages.
// The use of the GNU standard library error() function is recommended for
// this purpose.
//
// Additional information:  man 3 errno;  man 3 error;

// Variable to preserve errno temporarily
static int errsave = 0;

// Error cause
static char* errCause;

/// Error cause.
/// After some other module function fails (and returns an error code),
/// calling this function retrieves an appropriate message describing the
/// failure cause.  This may be used together with global variable errno
/// to produce informative error messages (using error(), for instance).
///
/// After a successful operation, the result is not garanteed (it might be
/// the previous error cause).  It is not meant to be used in that situation!
char* ImageErrMsg() { ///
  return errCause;
}


// Defensive programming aids
//
// Proper defensive programming in C, which lacks an exception mechanism,
// generally leads to possibly long chains of function calls, error checking,
// cleanup code, and return statements:
//   if ( funA(x) == errorA ) { return errorX; }
//   if ( funB(x) == errorB ) { cleanupForA(); return errorY; }
//   if ( funC(x) == errorC ) { cleanupForB(); cleanupForA(); return errorZ; }
//
// Understanding such chains is difficult, and writing them is boring, messy
// and error-prone.  Programmers tend to overlook the intricate details,
// and end up producing unsafe and sometimes incorrect programs.
//
// In this module, we try to deal with these chains using a somewhat
// unorthodox technique.  It resorts to a very simple internal function
// (check) that is used to wrap the function calls and error tests, and chain
// them into a long Boolean expression that reflects the success of the entire
// operation:
//   success = 
//   check( funA(x) != error , "MsgFailA" ) &&
//   check( funB(x) != error , "MsgFailB" ) &&
//   check( funC(x) != error , "MsgFailC" ) ;
//   if (!success) {
//     conditionalCleanupCode();
//   }
//   return success;
// 
// When a function fails, the chain is interrupted, thanks to the
// short-circuit && operator, and execution jumps to the cleanup code.
// Meanwhile, check() set errCause to an appropriate message.
// 
// This technique has some legibility issues and is not always applicable,
// but it is quite concise, and concentrates cleanup code in a single place.
// 
// See example utilization in ImageLoad and ImageSave.
//
// (You are not required to use this in your code!)


// Check a condition and set errCause to failmsg in case of failure.
// This may be used to chain a sequence of operations and verify its success.
// Propagates the condition.
// Preserves global errno!
static int check(int condition, const char* failmsg) {
  errCause = (char*)(condition ? "" : failmsg);
  return condition;
}


/// Init Image library.  (Call once!)
/// Currently, simply calibrate instrumentation and set names of counters.
void ImageInit(void) { ///
  InstrCalibrate();
  InstrName[0] = "pixmem";
  InstrName[1] = "pixcmp";
  InstrName[2] = "iterations";
    // InstrCount[0] will count pixel array acesses
  // Name other counters here...
  
}

// Macros to simplify accessing instrumentation counters:
#define PIXMEM InstrCount[0]
#define PIXCMP InstrCount[1] //contar o número de comparações
#define ITERATIONS InstrCount[2] //contar o número de iterações
// Add more macros here...

// TIP: Search for PIXMEM or InstrCount to see where it is incremented!


/// Image management functions

/// Create a new black image.
///   width, height : the dimensions of the new image.
///   maxval: the maximum gray level (corresponding to white).
/// Requires: width and height must be non-negative, maxval > 0.
/// 
/// On success, a new image is returned.
/// (The caller is responsible for destroying the returned image!)
/// On failure, returns NULL and errno/errCause are set accordingly.
Image ImageCreate(int width, int height, uint8 maxval) { ///
  assert (width >= 0);
  assert (height >= 0);
  assert (0 < maxval && maxval <= PixMax);
  Image img = (Image)malloc(sizeof(struct image)); //alocamos memória para a imagem
  if (check(img != NULL, "Failed to create the new image.")){ //verificamos se a alocação foi bem sucedida
  img->width = width;
  img->height = height;
  img->maxval = maxval;
  img->pixel = (uint8*)malloc(width*height*sizeof(uint8));
  PIXMEM += (unsigned long)(width*height);  // atribui o número de acessos à memória 
  return img;
  }
  else {
    ImageDestroy(&img); //destroi a imagem
    return NULL;
  }
}

/// Destroy the image pointed to by (*imgp).
///   imgp : address of an Image variable.
/// If (*imgp)==NULL, no operation is performed.
/// Ensures: (*imgp)==NULL.
/// Should never fail, and should preserve global errno/errCause.
void ImageDestroy(Image* imgp) { ///
  assert (imgp != NULL);
  if (*imgp != NULL) {
    free((*imgp)->pixel);
    free(*imgp);
    *imgp = NULL;
  }
}


/// PGM file operations

// See also:
// PGM format specification: http://netpbm.sourceforge.net/doc/pgm.html

// Match and skip 0 or more comment lines in file f.
// Comments start with a # and continue until the end-of-line, inclusive.
// Returns the number of comments skipped.
static int skipComments(FILE* f) {
  char c;
  int i = 0;
  while (fscanf(f, "#%*[^\n]%c", &c) == 1 && c == '\n') {
    i++;
  }
  return i;
}

/// Load a raw PGM file.
/// Only 8 bit PGM files are accepted.
/// On success, a new image is returned.
/// (The caller is responsible for destroying the returned image!)
/// On failure, returns NULL and errno/errCause are set accordingly.
Image ImageLoad(const char* filename) { ///
  int w, h;
  int maxval;
  char c;
  FILE* f = NULL;
  Image img = NULL;

  int success = 
  check( (f = fopen(filename, "rb")) != NULL, "Open failed" ) &&
  // Parse PGM header
  check( fscanf(f, "P%c ", &c) == 1 && c == '5' , "Invalid file format" ) &&
  skipComments(f) >= 0 &&
  check( fscanf(f, "%d ", &w) == 1 && w >= 0 , "Invalid width" ) &&
  skipComments(f) >= 0 &&
  check( fscanf(f, "%d ", &h) == 1 && h >= 0 , "Invalid height" ) &&
  skipComments(f) >= 0 &&
  check( fscanf(f, "%d", &maxval) == 1 && 0 < maxval && maxval <= (int)PixMax , "Invalid maxval" ) &&
  check( fscanf(f, "%c", &c) == 1 && isspace(c) , "Whitespace expected" ) &&
  // Allocate image
  (img = ImageCreate(w, h, (uint8)maxval)) != NULL &&
  // Read pixels
  check( fread(img->pixel, sizeof(uint8), w*h, f) == w*h , "Reading pixels" );
  PIXMEM += (unsigned long)(w*h);  // count pixel memory accesses

  // Cleanup
  if (!success) {
    errsave = errno;
    ImageDestroy(&img);
    errno = errsave;
  }
  if (f != NULL) fclose(f);
  return img;
}

/// Save image to PGM file.
/// On success, returns nonzero.
/// On failure, returns 0, errno/errCause are set appropriately, and
/// a partial and invalid file may be left in the system.
int ImageSave(Image img, const char* filename) { ///
  assert (img != NULL);
  int w = img->width;
  int h = img->height;
  uint8 maxval = img->maxval;
  FILE* f = NULL;

  int success =
  check( (f = fopen(filename, "wb")) != NULL, "Open failed" ) &&
  check( fprintf(f, "P5\n%d %d\n%u\n", w, h, maxval) > 0, "Writing header failed" ) &&
  check( fwrite(img->pixel, sizeof(uint8), w*h, f) == w*h, "Writing pixels failed" ); 
  PIXMEM += (unsigned long)(w*h);  // count pixel memory accesses

  // Cleanup
  if (f != NULL) fclose(f);
  return success;
}


/// Information queries

/// These functions do not modify the image and never fail.

/// Get image width
int ImageWidth(Image img) { ///
  assert (img != NULL);
  return img->width;
}

/// Get image height
int ImageHeight(Image img) { ///
  assert (img != NULL);
  return img->height;
}

/// Get image maximum gray level
int ImageMaxval(Image img) { ///
  assert (img != NULL);
  return img->maxval;
}

/// Pixel stats
/// Find the minimum and maximum gray levels in image.
/// On return,
/// *min is set to the minimum gray level in the image,
/// *max is set to the maximum.
void ImageStats(Image img, uint8* min, uint8* max) { ///
  assert (img != NULL);
  *min = PixMax; 
  *max = 0; // Define o valor do min como sendo o valor máximo do pixel e o valor do max como sendo o valor mínimo do pixel
  for (int i = 0; i < img->width * img->height; i++){
  if (img->pixel[i] < *min) *min = img->pixel[i]; 
  if (img->pixel[i] > *max) *max = img->pixel[i]; // Define o valor do min como sendo o valor mínimo do pixel e o valor do max como sendo o valor máximo do pixel
}

}

/// Check if pixel position (x,y) is inside img.
int ImageValidPos(Image img, int x, int y) { ///
  assert (img != NULL);
  return (0 <= x && x < img->width) && (0 <= y && y < img->height);
}

/// Check if rectangular area (x,y,w,h) is completely inside img.
int ImageValidRect(Image img, int x, int y, int w, int h) { ///
  assert (img != NULL);
  return (0 <= x && x+w <= img->width) && (0 <= y && y+h <= img->height);
}
/// Pixel get & set operations

/// These are the primitive operations to access and modify a single pixel
/// in the image.
/// These are very simple, but fundamental operations, which may be used to 
/// implement more complex operations.

// Transform (x, y) coords into linear pixel index.
// This internal function is used in ImageGetPixel / ImageSetPixel. 
// The returned index must satisfy (0 <= index < img->width*img->height)
static inline int G(Image img, int x, int y) {
  int index = y * img->width + x;
  assert (0 <= index && index < img->width*img->height); // Verifica se o index está dentro dos limites
  return index;
}

/// Get the pixel (level) at position (x,y).
uint8 ImageGetPixel(Image img, int x, int y) { ///
  assert (img != NULL);
  assert (ImageValidPos(img, x, y)); // Verifica se a posição (x, y) é válida na imagem img
  PIXMEM++; // Incrementa o contador de acessos à memória
  return img->pixel[G(img, x, y)]; // Retorna o valor do pixel na posição (x, y)
} 

/// Set the pixel at position (x,y) to new level.
void ImageSetPixel(Image img, int x, int y, uint8 level) { ///
  assert (img != NULL);
  assert (ImageValidPos(img, x, y));
  PIXMEM++;
  img->pixel[G(img, x, y)] = level; // Define o valor do pixel na posição (x, y) como sendo level
} 


/// Pixel transformations

/// These functions modify the pixel levels in an image, but do not change
/// pixel positions or image geometry in any way.
/// All of these functions modify the image in-place: no allocation involved.
/// They never fail.


/// Transform image to negative image.
/// This transforms dark pixels to light pixels and vice-versa,
/// resulting in a "photographic negative" effect.
void ImageNegative(Image img) { ///
  assert (img != NULL);
  for (int i = 0; i < img->width * img->height; i++){
    img->pixel[i] = PixMax - img->pixel[i]; // Define o valor do pixel na posição i como sendo o valor máximo menos o valor do pixel na posição i
  }
}

/// Apply threshold to image.
/// Transform all pixels with level<thr to black (0) and
/// all pixels with level>=thr to white (maxval).
void ImageThreshold(Image img, uint8 thr) { ///
  assert (img != NULL);
  for (int i = 0; i < img->width * img->height; i++){
    if (img->pixel[i] < thr) img->pixel[i] = 0;
    else img->pixel[i] = img->maxval; // Define o valor do pixel na posição i como sendo o valor máximo se o valor do pixel na posição i for maior ou igual a thr, caso contrário, define o valor do pixel na posição i como sendo 0
  }
}

/// Brighten image by a factor.
/// Multiply each pixel level by a factor, but saturate at maxval.
/// This will brighten the image if factor>1.0 and
/// darken the image if factor<1.0.
void ImageBrighten(Image img, double factor) { ///
  assert (img != NULL);
  assert( factor > 0.0);
  for (int x = 0; x < img->width; x++) {
    for (int y = 0; y < img->height; y++) {
      int index = G(img, x, y); // Define o valor do index como sendo o valor do pixel na posição (x, y)
      img->pixel[index] = (uint8)(img->pixel[index] * factor + 0.5); // Multiplica o valor do pixel por factor e arredonda o valor para o inteiro mais próximo
      if (img->pixel[index] > img->maxval) img->pixel[index] = img->maxval; // Verifica se o valor do pixel está dentro dos limites
    }
  }
}

/// Geometric transformations

/// These functions apply geometric transformations to an image,
/// returning a new image as a result.
/// 
/// Success and failure are treated as in ImageCreate:
/// On success, a new image is returned.
/// (The caller is responsible for destroying the returned image!)
/// On failure, returns NULL and errno/errCause are set accordingly.

// Implementation hint: 
// Call ImageCreate whenever you need a new image!

/// Rotate an image.
/// Returns a rotated version of the image.
/// The rotation is 90 degrees anti-clockwise.
/// Ensures: The original img is not modified.
/// On success, a new image is returned.
/// (The caller is responsible for destroying the returned image!)
/// On failure, returns NULL and errno/errCause are set accordingly.
Image ImageRotate(Image img) {
  assert(img != NULL);
  uint8 maxval = img->maxval;
  Image newImg = ImageCreate(img->height, img->width, maxval);
  for (int i = 0; i < img->height; i++) {
    for (int j = 0; j < img->width; j++) {
      uint8 newPixel = img->pixel[i*img->width + j]; // Define o valor do newPixel como sendo o pixel da imagem img na posição (i, j)
      newImg->pixel[(img->width - j - 1) * img->height + i] = newPixel; //copia os pixeis da imagem img para a imagem newImg, fazedo a rotação de 90º
    }
  }
  if (check(newImg != NULL, "Failed to create the new image.")) return newImg; //verificamos se a alocação foi bem sucedida
  else {
    ImageDestroy(&newImg); //destroi a imagem
    return NULL;
  }
}

/// Mirror an image = flip left-right.
/// Returns a mirrored version of the image.
/// Ensures: The original img is not modified.
/// 
/// On success, a new image is returned.
/// (The caller is responsible for destroying the returned image!)
/// On failure, returns NULL and errno/errCause are set accordingly.
Image ImageMirror(Image img) {
  assert (img != NULL);
  Image NewImg = ImageCreate(img->width, img->height, img->maxval);
  for (int i = 0; i < img->width; i++){
    for (int j = 0; j < img->height; j++){
      NewImg->pixel[G(img, i, j)] = img->pixel[G(img, img->width - i - 1, j)]; //copia os pixeis da imagem img para a imagem NewImg
    }
  }
  if (check(NewImg != NULL, "Failed to create the new image.")) return NewImg; //verificamos se a alocação foi bem sucedida
  else {
    ImageDestroy(&NewImg); //destroi a imagem
    return NULL;
  }
}

/// Crop a rectangular subimage from img.
/// The rectangle is specified by the top left corner coords (x, y) and
/// width w and height h.
/// Requires:
///   The rectangle must be inside the original image.
/// Ensures:
///   The original img is not modified.
///   The returned image has width w and height h.
/// 
/// On success, a new image is returned.
/// (The caller is responsible for destroying the returned image!)
/// On failure, returns NULL and errno/errCause are set accordingly.
Image ImageCrop(Image img, int x, int y, int w, int h) {
    assert(img != NULL);
    assert(ImageValidRect(img, x, y, w, h));

    Image newImg = ImageCreate(w, h, img->maxval);
    if (newImg == NULL) {
        return NULL;
    }

    for (int i = 0; i < w; i++) {
        for (int j = 0; j < h; j++) {
            uint8 pixelValue = ImageGetPixel(img, x + i, y + j);  // Define o valor do pixelValue como sendo o pixel da imagem img na posição (x + i, y + j)
            ImageSetPixel(newImg, i, j, pixelValue); //copia os pixeis da imagem img para a imagem newImg
        }
    }

    return newImg;
}


/// Operations on two images

/// Paste an image into a larger image.
/// Paste img2 into position (x, y) of img1.
/// This modifies img1 in-place: no allocation involved.
/// Requires: img2 must fit inside img1 at position (x, y).
void ImagePaste(Image img1, int x, int y, Image img2) { ///
  assert (img1 != NULL);
  assert (img2 != NULL);
  assert (ImageValidRect(img1, x, y, img2->width, img2->height)); // Verifica se a imagem img2 cabe na imagem img1 na posição (x, y)
  // Insert your code here!
  for (int i = 0; i < img2->width; i++) {
    for (int j = 0; j < img2->height; j++) {
      img1->pixel[G(img1, x + i, y + j)] = img2->pixel[G(img2, i, j)]; //copia os pixeis da imagem img2 para a imagem img1
    }
  }
}
/// Blend an image into a larger image.
/// Blend img2 into position (x, y) of img1.
/// This modifies img1 in-place: no allocation involved.
/// Requires: img2 must fit inside img1 at position (x, y).
/// alpha usually is in [0.0, 1.0], but values outside that interval
/// may provide interesting effects.  Over/underflows should saturate.
void ImageBlend(Image img1, int x, int y, Image img2, double alpha) {
    assert(img1 != NULL); // Verifica se a imagem img1 existe
    assert(img2 != NULL); // Verifica se a imagem img2 existe
    assert(ImageValidRect(img1, x, y, img2->width, img2->height)); // Verifica se a imagem img2 cabe na imagem img1 na posição (x, y)

    for (int i = 0; i < img2->height; i++) { // Percorre os pixeis da imagem img2
        for (int j = 0; j < img2->width; j++) { 
            uint8_t pixel1 = img1->pixel[G(img1, x + j, y + i)]; // Define o valor do pixel1 como sendo o pixel da imagem img1 na posição (x + j, y + i)
            uint8_t pixel2 = img2->pixel[G(img2, j, i)]; // Define o valor do pixel2 como sendo o pixel da imagem img2 na posição (j, i)

            double blendedPixel = (1.0 - alpha) * pixel1 + alpha * pixel2;  // Calcula o valor do pixel resultante da mistura dos pixeis das imagens img1 e img2 tendo em conta o valor de alpha

            uint8_t finalPixel = (blendedPixel > img1->maxval) ? img1->maxval : (blendedPixel < 0) ? 0 : (uint8_t)(blendedPixel + 0.5); // Verifica se o valor do pixel está dentro dos limites e arredonda o valor para o inteiro mais próximo.

            img1->pixel[G(img1,x+j,y+i)] = finalPixel; // Define o valor do pixel na posição (x + j, y + i) da imagem img1 como sendo o finalPixel
        }
    }
}

/// Compare an image to a subimage of a larger image.
/// Returns 1 (true) if img2 matches subimage of img1 at pos (x, y).
/// Returns 0, otherwise.
int ImageMatchSubImage(Image img1, int x, int y, Image img2){
  assert (img1 != NULL); // Verifica se a imagem img1 existe
  assert (img2 != NULL); // Verifica se a imagem img2 existe
  assert (ImageValidPos(img1, x, y)); // Verifica se a posição (x, y) é válida na imagem img1
  for (int i = 0; i < img2-> width; i++){ // Percorre os pixeis da imagem img2
    for (int j = 0; j < img2->height; j++){ 
      PIXCMP += 1; // Incrementa o contador de comparações
      PIXMEM += 2; // Incrementa o contador de acessos à memória 2 vezes (1 para cada variável)
      ITERATIONS++; // Incrementa o contador de iterações
      if (img1->pixel[G(img1, i+x, j+y)] != img2->pixel[G(img2, i, j)]){
        return 0; // Se os pixeis não forem iguais, retorna 0
      }
    }
  }
  return 1; // Se os pixeis forem todos iguais, retorna 1
}
/// Locate a subimage inside another image.
/// Searches for img2 inside img1.
/// If a match is found, returns 1 and matching position is set in vars (*px, *py).
/// If no match is found, returns 0 and (*px, *py) are left untouched.
int ImageLocateSubImage(Image img1, int* px, int* py, Image img2) {
    assert(img1 != NULL); // Verifica se a imagem img1 existe
    assert(img2 != NULL); // Verifica se a imagem img2 existe

    for (int i = 0; i <= img1->width - img2->width; i++) { // percorre os pixeis da imagem img1 até aos pixeis em que a imagem img2 cabe na imagem img1
        for (int j = 0; j <= img1->height - img2->height; j++) {  
            if (ImageMatchSubImage(img1, i, j, img2)) { // Verifica se a imagem img2 é igual a uma subimagem de img1
                *px = i; // Se for igual, atualiza as variáveis px e py com as coordenadas da subimagem
                *py = j; 
                return 1;
            }
        }
    }
    return 0;
}

///filtering
/// Blur an image by a applying a (2dx+1)x(2dy+1) mean filter.
/// Each pixel is substituted by the mean of the pixels in the rectangle
/// [x-dx, x+dx]x[y-dy, y+dy].
/// The image is changed in-place.
void ImageBlur(Image img, int dx, int dy){
  int* sumPixels; //array onde iremos armazenar a soma de pixeis
  int blurValue, xstart, xend, ystart, yend, xSize, ySize, count; 
  sumPixels = (uint8*) malloc(sizeof(uint8*) * img->height * img->width); //alocamos memória para o array
  if(check(sumPixels != NULL, "Failed memory allocation")){ //verificamos se a alocação foi bem sucedida
    for (int x = 0; x < img->width; x++){
      for (int y = 0; y < img->height; y++){
        PIXMEM+=4; //incrementamos o contador de acessos à memória 4 vezes (1 para cada variável)
        int currentPixel = ImageGetPixel(img, x, y);
        int leftPixelSum = (x > 0) ? sumPixels[G(img, x - 1, y)] : 0;
        int topPixelSum = (y > 0) ? sumPixels[G(img, x, y - 1)] : 0;
        int diagonalPixelSum = (x > 0 && y > 0) ? sumPixels[G(img, x - 1, y - 1)] : 0;

        PIXMEM+=1; //incrementamos o contador de acessos à memória 1 vez 
        sumPixels[G(img, x, y)] = currentPixel + leftPixelSum + topPixelSum - diagonalPixelSum; //somamos o pixel da posição atual com o pixel da posição à esquerda, o pixel da posição acima e subtraímos o pixel da posição diagonal para obter a soma de todos os pixeis até à posição atual (temos que remover o pixel da posição diagonal porque ele foi somado duas vezes)
        ITERATIONS++; //incrementamos o contador de iterações
      }
    }
    for (int x = 0; x < img->width; x++){
      for (int y = 0; y < img->height; y++){ //percorremos a imagem
        xstart = (x - dx) > 0 ? (x - dx) : 0; //calculamos o xstart e o ystart que serão os limites inicias da área que iremos percorrer, ou seja, a área que iremos percorrer será a área que está dentro do retângulo [x-dx, x+dx]x[y-dy, y+dy]
        ystart = (y - dy) > 0 ? (y - dy) : 0; 
        xend = (x + dx) < img->width ? (x + dx) : (img->width - 1); //calculamos o xend e o yend que serão os limites finais da área que iremos percorrer, ou seja, a área que iremos percorrer será a área que está dentro do retângulo [x-dx, x+dx]x[y-dy, y+dy]
        yend = (y + dy) < img->height ? (y + dy) : (img->height - 1); 
        xSize = xend - xstart + 1; //calculamos o tamanho da área que iremos percorrer
        ySize = yend - ystart + 1; 
        count = ySize * xSize; //calculamos o número de pixeis que iremos percorrer
        PIXMEM+=4; //incrementamos o contador de acessos à memória 4 vezes (1 para cada variável)
        blurValue = sumPixels[G(img, xend, yend)];
        if (ystart > 0) { //se o ystart for maior que 0, significa que o ystart não está na primeira linha da imagem, logo temos que subtrair o valor do pixel que está acima do ystart 
            blurValue -= sumPixels[G(img, xend, ystart - 1)];}
        if (xstart > 0) { //se o xstart for maior que 0, significa que o xstart não está na primeira coluna da imagem, logo temos que subtrair o valor do pixel que está à esquerda do xstart
            blurValue -= sumPixels[G(img, xstart - 1, yend)];}
        if (xstart > 0 && ystart > 0) { //se o xstart e o ystart forem maiores que 0, significa que o xstart e o ystart não estão na primeira coluna e na primeira linha da imagem, logo temos que somar o valor do pixel que está na posição diagonal ao xstart e ao ystart visto que ele foi subtraído duas vezes
            blurValue += sumPixels[G(img, xstart - 1, ystart - 1)];} 
        blurValue = (blurValue + count / 2)/count; //calculamos a média dos pixeis que percorremos arredondando o valor para o inteiro mais próximo
        ImageSetPixel(img, x, y, blurValue); //definimos o valor do pixel na posição atual como sendo o blurValue
        ITERATIONS++;  //incrementamos o contador de iterações
      }
    }
  }
  free(sumPixels); //destroi a imagem
}
