
void calc_pixels(bool digits [][SEGMENTS], int digit_show[], bool *pix_show);

void calc_pixels(bool digits [][SEGMENTS], int digit_show[], bool *pix_show){
  Serial.begin(4800);
  for(int d = 0; d < DIGITS; d++){
    for(int seg = 0; seg < SEGMENTS; seg++){
      for(int p = 0; p < SEGMENTLEDS; p++){
        pix_show[SEGMENTLEDS*(d*SEGMENTS+seg)+p] = digits[digit_show[d]][seg];
        
        
        
        /* **DEBUG**
        //Printout entire pix_show Array with Comments
        Serial.print("Digit: "); 
        Serial.print(d); 
        Serial.print(" | Seg: "); 
        Serial.print(seg); 
        Serial.print(" | Pixel: "); 
        Serial.print(p);
        Serial.print(" | Val: ");
        Serial.println(digits[digit_show[d]][seg]);
        */
      }
    }
  }
}
