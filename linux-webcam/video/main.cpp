#include "read-camera.h"

void main() {
unsigned char* mybuff;
unsigned char* src;
int w,h;
rgbColor rgb; 

videoObject* mine  = new videoObject;
mine->takePicture();
mybuff = mine->dumpPicture();
w = mine->returnPictureWidth();
h = mine->returnPictureHeight();

 fprintf(stdout, "P6\n%d %d 255\n", w, h);

   src = mybuff;

     for (int i = 0; i < w * h; i++) {
	         rgb = mine->returnNextRGB();
		     
		 
		 fputc(rgb.r, stdout);
		         fputc(rgb.g, stdout);
			     fputc(rgb.b, stdout);
		  
		 
			  }


/*
nfor (int i=0; i < 900; i++) {
rgb = mine->returnNextRGB();
cout << rgb.r << ":" << rgb.g << ":" << rgb.b << "\n";
}
*/

}
