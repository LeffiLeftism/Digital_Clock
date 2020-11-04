
bool calc_pixels(bool digits [][SEGMENTS], int digit_show[], int countPixels);

bool calc_pixels(bool digits [][SEGMENTS], int digit_show[], int countPixels){
  bool new_pix_show[countPixels];
  for(int d = 0; d < DIGITS; d++){
    for(int seg = 0; seg < SEGMENTS; seg++){
      for(int p = 0; p < SEGMENTLEDS; p++){
        new_pix_show[SEGMENTLEDS*(d*SEGMENTS+seg)+p] = digits[digit_show[d]][seg];
      }
    }
  }
  return new_pix_show;
}
