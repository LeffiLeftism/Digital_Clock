//Function overview
void calc_pixels(bool symbols [][SEGMENTS], byte symbol_show[], bool *pix_show);
//Add multiple functions to change color for each pixel


//Functions
void calc_pixels(bool symbols [][SEGMENTS], byte symbol_show[], bool *pix_show){
  for(int d = 0; d < DIGITS; d++){
    for(int seg = 0; seg < SEGMENTS; seg++){
      for(int p = 0; p < LEDSPERSEGMENT; p++){
        pix_show[LEDSPERSEGMENT*(d*SEGMENTS+seg)+p] = symbols[symbol_show[d]][seg];
        
        
        
        /* **DEBUG**
        //Printout entire pix_show Array with Comments (digit|segment|pixel|val 1/0)
        Serial.begin(4800);
        Serial.print("Digit: "); 
        Serial.print(d); 
        Serial.print(" | Seg: "); 
        Serial.print(seg); 
        Serial.print(" | Pixel: "); 
        Serial.print(p);
        Serial.print(" | Val: ");
        Serial.println(symbols[symbol_show[d]][seg]);
        */
      }
    }
  }
}
