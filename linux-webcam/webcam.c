#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <sys/ioctl.h>
#include <stdlib.h>
#include <linux/types.h>
#include <linux/videodev.h>

int
main (int argc, char *argv[])
{
  int bpp, fd;
  struct video_capability cap;
  struct video_window win;
  struct video_picture vpic;
  char *videoBuffer;

  // Open the video4link device for reading.
  if ((fd = open ("/dev/video0", O_RDONLY)) < 0)
    {
      fprintf (stderr, "Could not open the device file\n");
      exit (1);
    }

  // Make sure this is a video4linux device
  if (ioctl (fd, VIDIOCGCAP, &cap) < 0)
    {
      fprintf (stderr, "VIDIOCGCAP says this is not a video4linux device\n");
      close (fd);
      exit (1);
    }

  // Get the video overlay window
  if (ioctl (fd, VIDIOCGWIN, &win) < 0)
    {
      fprintf (stderr, "Could not obtain the video overlay window\n");
      close (fd);
      exit (1);
    }

  // Get the picture properties
  if (ioctl (fd, VIDIOCGPICT, &vpic) < 0)
    {
      fprintf (stderr, "Could not obtain picture properties\n");
      close (fd);
      exit (1);
    }

  // Now set the color resolution on the camera -- this is ugly
  if (cap.type & VID_TYPE_MONOCHROME)
    {
      vpic.depth = 8;
      vpic.palette = VIDEO_PALETTE_GREY;
      if (ioctl (fd, VIDIOCSPICT, &vpic) < 0)
	{
	  vpic.depth = 6;
	  if (ioctl (fd, VIDIOCSPICT, &vpic) < 0)
	    {
	      vpic.depth = 4;
	      if (ioctl (fd, VIDIOCSPICT, &vpic) < 0)
		{
		  fprintf (stderr,
			   "Unable to find a supported Capture format\n");
		  close (fd);
		  exit (1);
		}
	    }
	}
    }
  else
    {
      vpic.depth = 24;
      vpic.palette = VIDEO_PALETTE_RGB24;

      if (ioctl (fd, VIDIOCSPICT, &vpic) < 0)
	{
	  vpic.palette = VIDEO_PALETTE_RGB565;
	  vpic.depth = 16;

	  if (ioctl (fd, VIDIOCSPICT, &vpic) == -1)
	    {
	      vpic.palette = VIDEO_PALETTE_RGB555;
	      vpic.depth = 15;

	      if (ioctl (fd, VIDIOCSPICT, &vpic) == -1)
		{
		  fprintf (stderr,
			   "Unable to find a supported capture format.\n");
		  close (fd);
		  exit (1);
		}
	    }
	}
    }

  bpp = vpic.depth;

  // Grab a single frame
  if ((videoBuffer =
       (unsigned char *) malloc (win.width * win.height * bpp)) == NULL)
    {
      fprintf (stderr, "Out of memory.\n");
      exit (1);
    }

  read (fd, videoBuffer, win.width * win.height * bpp);


}
