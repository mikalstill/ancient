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
#include "read-camera.h"

int videoObject::returnPicturePalette() {
	return vpic.palette;
}

int videoObject::returnPictureWidth() {
	return win.width;
}

int videoObject::returnPictureHeight() {
	return win.height;
}

int videoObject::returnPictureSize() {
	return win.width * win.height;
}

rgbColor videoObject::returnNextRGB() {

	switch (vpic.palette) {
	
		case VIDEO_PALETTE_GREY:
			switch(bpp)
			{
			   case4:
			   case6:
			   case8:
				(pixelColor.r) = (pixelColor.g) = \
					(pixelColor.b) = (*videoBuffer++<<8);
				break;
			   case 16:
				(pixelColor.r) = (pixelColor.g) = \
					(pixelColor.b) = *((unsigned short *) \
					 videoBuffer);
				videoBuffer +=2;
				break;
			}
		break;

		case VIDEO_PALETTE_RGB565:
			{
				unsigned short tmp = *(unsigned short *)videoBuffer;
				(pixelColor.r) = tmp&0xF800;
				(pixelColor.g) = (tmp<<5)&0xFC00;
				(pixelColor.b) = (tmp<<11)&0xF800;
				videoBuffer +=2;
			}
		break;
	
		case VIDEO_PALETTE_RGB555:
			(pixelColor.r) = (videoBuffer[0]&0xF8)<<8;
			(pixelColor.g) = ((videoBuffer[0] << 5 | videoBuffer[1] \
					   >> 3)&0xF8)<<8;
			(pixelColor.b) = ((videoBuffer[1] <<2) & 0xF8)<<8;
			videoBuffer+=2;
			break;


		case VIDEO_PALETTE_RGB24:
			(pixelColor.r) = videoBuffer[0] ;
			(pixelColor.g) = videoBuffer[1] ;
			(pixelColor.b) = videoBuffer[2] ;
			videoBuffer += 3;
			break;
		
		default:	
			fprintf(stderr,"Not supported format");
	}

return pixelColor;
}

unsigned char* videoObject::dumpPicture() {
return videoBuffer;
}

int videoObject::returnColorDepth() {
return bpp;
}

int videoObject::takePicture() {
videoBuffer = (unsigned char*) malloc(win.width * win.height * bpp);

if (!videoBuffer) {
		fprintf(stderr, "Out of memory.\n");
		exit(1);
}

read(fd,videoBuffer, win.width * win.height * bpp);

return(1);
}


videoObject::videoObject(const char* FILE="/dev/video0") {
	// Open the video4link device for reading.
	fd = open(FILE,O_RDONLY);
	
	if (fd < 0) {
		perror("Can Not open /dev/vide0");
		exit(1);
	}	


	//Make sure this is a video4linux device
	if (ioctl(fd,VIDIOCGCAP, &cap) < 0) {
		perror("VIDEOGCAP");
		cerr << FILE << " not a video4linux device\n";	
		close(fd);
		exit(1);
	}
	
	//Get the video overlay window
	if (ioctl(fd,VIDIOCGWIN, &win) < 0) {
		perror("VIDIOCGWIN");
		close(fd);
		exit(1);
	}

	//Get the picture properties
	if (ioctl (fd,VIDIOCGPICT, &vpic) < 0) {
		perror("VIDIOCGPICT");
		close(fd);
		exit(1);
	}
	
	//Now set the color resolution on the camera
	if (cap.type & VID_TYPE_MONOCHROME) {
		vpic.depth=8;
		vpic.palette=VIDEO_PALETTE_GREY;
		if(ioctl(fd,VIDIOCSPICT,&vpic) < 0) {
			vpic.depth=6;
		  if(ioctl(fd,VIDIOCSPICT, &vpic) < 0) {
			vpic.depth=4;
		    if (ioctl(fd,VIDIOCSPICT, &vpic) <0) {
		    	fprintf(stderr, "Unable to find a supported Capture format\n");
	    		close(fd);
			exit(1);
		    }
		  }
		}
	} else {
	vpic.depth = 24;
	vpic.palette = VIDEO_PALETTE_RGB24;

	if (ioctl(fd,VIDIOCSPICT,&vpic) <0) {
		vpic.palette=VIDEO_PALETTE_RGB565;
		vpic.depth=16;

	  if(ioctl(fd,VIDIOCSPICT,&vpic) == -1) {
	  	vpic.palette = VIDEO_PALETTE_RGB555;
      		vpic.depth=15;

		if(ioctl(fd, VIDIOCSPICT, &vpic) == -1) {
		fprintf(stderr, "Unable to find a supported capture format.\n");
		close(fd);
		exit(1);
		}
	  }
	}
	}	
	
	bpp = vpic.depth;	

}

videoObject::~videoObject() {
	close(fd);	
	delete videoBuffer;	
}
