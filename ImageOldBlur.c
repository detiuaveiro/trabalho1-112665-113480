///filtering
/// Blur an image by a applying a (2dx+1)x(2dy+1) mean filter.
/// Each pixel is substituted by the mean of the pixels in the rectangle
/// [x-dx, x+dx]x[y-dy, y+dy].
/// The image is changed in-place.
// void OldImageBlur(Image img, int dx, int dy) {
  // Toda a função foi transformada em comentário
  /*
  // assert (img != NULL);
  // assert (dx >= 0 && dy >= 0);

  // Create a temporary image to store the blurred result
  // Image blurredImage = ImageCreate(img->width+1, img->height+1, img->maxval);
  // ImagePaste(blurredImage,0, 0, img);
  for (int i = 0; i <= img->width; i++) {
    for (int j = 0; j <= img->height; j++) {
      int sum = 0;
      int count = 0;

      for (int k = i - dx; k <= i + dx; k++) {
        for (int l = j - dy; l <= j + dy; l++) {
          // ITERATIONS++;
          if (ImageValidPos(img, k, l)) {
            sum += ImageGetPixel(img, k, l);
            count++;
          }
        }
      }
      ImageSetPixel(img, i, j, (sum + count / 2) / count);
    }
  }
  // for (int i = 0; i < img->width; i++) {
  //   for (int j = 0; j < img->height; j++) {
  //     // ImageSetPixel(img, i, j, ImageGetPixel(blurredImage, i, j));
  //     // ITERATIONS++;
  //   }
  // }
  // ImageDestroy(&blurredImage); //destrói a imagem
  */
//}