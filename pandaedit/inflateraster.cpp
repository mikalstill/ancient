#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include <string>
#include "verbosity.h"
#include "utility.h"

// Inflate a raster to a given pixel sample size
char *inflateraster(char *input, unsigned long width, unsigned long height, 
		    int bitdepth, int targetbitdepth, 
		    int channels, int targetchannels){
  float scalefactor;
  char *output;
  int bytedepth, targetbytedepth;
  unsigned int inset, outset;

  debug(dlTrace, string("Inflating raster: ") + toString((long) width) + 
	string(" x ") + toString((long) height) + string(" bitdepth from ") +
	toString(bitdepth) + string(" to ") + toString(targetbitdepth) +
	string(" channels from ") + toString(channels) + string(" to ") +
	toString(targetchannels));

  // Why are we here?
  if((channels == targetchannels) && (bitdepth == targetbitdepth)){
    debug(dlTrace, "Inflate raster resulted in NOOP");
    // todo_mikal: this might cause a seg fault because of the double free
    return input;
  }

  // Calculate the byte depth
  bytedepth = bitdepth / 8;
  if(bitdepth % 8 != 0)
    bytedepth++;
  
  // Calculate the target byte depth
  targetbytedepth = targetbitdepth / 8;
  if(targetbitdepth % 8 != 0)
    targetbytedepth++;

  // Build the output raster
  if((output = (char *) malloc(width * height * targetchannels * targetbytedepth)) 
     == NULL){
    debug(dlError, "Failed to allocate enough memory for output raster");
    return (char *) -1;
  }

  // Are we changing the bitdepth?
  if(bitdepth != targetbitdepth){
    // Determine how much each sample has to be scaled by to get to the new bitdepth
    scalefactor = (float) pow(2.0, (double) targetbitdepth)  / 
      (float) pow(2.0, (double) bitdepth);
    debug(dlTrace, string("Scaling factor is ") + toString(scalefactor));
    debug(dlTrace, string("Byte depth is ") + toString(bytedepth));
    
    // Work through the input pixels, and turn them into output pixels
    outset = 0;
    for(inset = 0; inset < width * height * channels; inset += bytedepth){
      // todo_mikal: This will only work for images with a bytedepth of one
      output[outset] = input[inset] * scalefactor;
      outset += targetbytedepth;
    }
  }
  
  // Are we the number of channels?
  // todo_mikal: we can't do both of these changes in one pass at the moment...
  else if(channels != targetchannels){
    debug(dlTrace, string("Expanding from ") + toString(channels) + 
	  string(" channels to ") + toString(targetchannels) + string(" channels"));
    debug(dlTrace, string("Target byte depth is: ") + toString(targetbytedepth));

    // Work through the input pixels, and turn them into output pixels
    outset = 0;
    for(inset = 0; inset < width * height * channels; inset += bytedepth){
      int i;

      // this wont work for just adding an alpha channel
      for(i = 0; i < targetchannels - channels + 1; i++){
	output[outset] = input[inset];
	outset += targetbytedepth;
      }
    }
  }

  return output;
}
