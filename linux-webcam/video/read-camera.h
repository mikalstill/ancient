/*
 * Class To work deal with Video4Linux. Alot of this code was borrowed from
 * http://www.tazenda.demon.co.uk/phil/vgrabber.c
 *
 * The largest change was converting to to c++ class format.
 *
 * Matt Poepping
 * Poepping@ksni.net
 *
 *
*/

#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <sys/ioctl.h>
#include <stdlib.h>
#include <iostream.h>
#include <linux/types.h>
#include <linux/videodev.h>

// This class is used to return RGB color sets;
class rgbColor {
	public:
		int r;
		int g;
		int b;
};

class videoObject {
	public:
	videoObject(const char* inputFile="/dev/video0");
	virtual ~videoObject();
	int takePicture();		//return 1 if ok
	int returnColorDepth();
	int returnPictureSize();	
	int returnPictureWidth();
	int returnPictureHeight();	
	int returnPicturePalette();	
	unsigned char* dumpPicture();
	rgbColor returnNextRGB();
	
	protected:
	unsigned char *videoBuffer;
	int bpp, r, g, b,fd;
	unsigned int i, src_depth;
	rgbColor pixelColor;	
	struct video_capability cap;
	struct video_window	win;
	struct video_picture	vpic;
};
