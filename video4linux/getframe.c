// Get a frame from a video4linux device, and then exit

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>

#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>

#include <linux/videodev.h>

#include <jpeglib.h>
#include <tiffio.h>

void getframe_display_capability(struct video_capability *cap)
{
  printf("Information about this video device:\n");
  printf("  Name: %s\n", cap->name);
  printf("  Type: %d (0x%04x)\n", cap->type, cap->type);
  printf("  Number of channels: %d\n", cap->channels);
  printf("  Number of audio channels: %d\n", cap->audios);
  printf("  Maximum size: %d x %d\n", cap->maxwidth, cap->maxheight);
  printf("  Minimum size: %d x %d\n", cap->minwidth, cap->minheight);
  printf("\n");
}

void getframe_display_channel(struct video_channel *chan)
{
  printf("Information about the first channel on the device:\n");
  printf("  Channel number: %d\n", chan->channel);
  printf("  Name: %s\n", chan->name);
  printf("  Number of tuners: %d\n", chan->tuners);
  printf("  Flags: %d (0x%04x)\n", chan->flags, chan->flags);
  printf("\n");
}

void getframe_display_picture(struct video_picture *pict)
{
  printf("Picture information:\n");
  printf("  Brightness: %d\n", pict->brightness);
  printf("  Hue: %d\n", pict->hue);
  printf("  Colour: %d\n", pict->colour);
  printf("  Contrast: %d\n", pict->contrast);
  printf("  Whiteness: %d\n", pict->whiteness);
  printf("  Depth: %d\n", pict->depth);

  printf("  Palette: %d (", pict->palette);
  switch(pict->palette){
  case VIDEO_PALETTE_GREY: printf("grey"); break;
  case VIDEO_PALETTE_HI240: printf("hi40"); break;
  case VIDEO_PALETTE_RGB565: printf("rgb565"); break;
  case VIDEO_PALETTE_RGB555: printf("rgb555"); break;
  case VIDEO_PALETTE_RGB24: printf("rgb24"); break;
  case VIDEO_PALETTE_RGB32: printf("rgb32"); break;
  case VIDEO_PALETTE_YUV422: printf("yuv422"); break;
  case VIDEO_PALETTE_YUYV: printf("yuyv"); break;
  case VIDEO_PALETTE_UYVY: printf("uyvy"); break;
  case VIDEO_PALETTE_YUV420: printf("yuv420"); break;
  case VIDEO_PALETTE_YUV411: printf("yuv411"); break;
  case VIDEO_PALETTE_RAW: printf("raw"); break;
  case VIDEO_PALETTE_YUV422P: printf("yuv422p"); break;
  case VIDEO_PALETTE_YUV411P: printf("yuv411p"); break;
  default: printf("unknown"); break;
  }
  printf(")\n");
  printf("\n");
}

int main(int argc, char *argv[])
{
  int fd, out, i, doraw = 0, dojpeg = 0, dotiff = 0;
  ssize_t size;
  struct video_capability cap;
  struct video_channel chan;
  struct video_picture pict;
  char img[640 * 480 * 3], *device = NULL, optchar;
  TIFF *output;

  while ((optchar = getopt (argc, argv, "d:rjt")) != -1)
    {
      switch (optchar)
        {
        case 'd':
          device = (char *) strdup(optarg);
          break;

	case 'r':
	  doraw = 1;
	  break;

	case 'j':
	  dojpeg = 1;
	  break;

	case 't':
	  dotiff = 1;
	  break;

        default:
        case '?':
          printf ("Unknown command line option...\n");
          printf ("Try: %s [-d devifile] [-r] [-j]\n", argv[0]);
          exit (0);
          break;
        }
    }

  if(!device)
    device = (char *) strdup("/dev/video0");

  /* Open the video device */
  fd = open(device, O_RDWR);
  if(fd < 0)
    {
      perror("Error opening video device");
      exit(1);
    }

  if(ioctl(fd, VIDIOCGCAP, &cap) < 0)
    {
      perror("Error reading capabilities");
      exit(1);
    }
  getframe_display_capability(&cap);

  chan.channel = 0;
  if(ioctl(fd, VIDIOCGCHAN, &chan) < 0)
    {
      perror("Error reading channel capabilities");
      exit(1);
    }
  getframe_display_channel(&chan);

  if(ioctl(fd, VIDIOCSCHAN, &chan.channel) < 0)
    {
      perror("Could not set channel to zero");
      exit(1);
    }

  if(ioctl(fd, VIDIOCGPICT, &pict) < 0)
    {
      perror("Could not get picture information");
      exit(1);
    }
  getframe_display_picture(&pict);

  pict.palette = VIDEO_PALETTE_RGB24;
  pict.depth = 24;
  if(ioctl(fd, VIDIOCSPICT, &pict) < 0)
    {
      perror("Could not set picture information");
      exit(1);
    }

  if(ioctl(fd, VIDIOCGPICT, &pict) < 0)
    {
      perror("Could not get picture information");
      exit(1);
    }
  getframe_display_picture(&pict);

  /* Now we can read a single frame from the first device */
  size = read(fd, &img, 640 * 480 * 3);
  printf("Read %d bytes (expected %d)\n", size, 640 * 480 * 3);

  if(dojpeg)
    {
      int tempfd;
      char filename[] = "/tmp/jpeg-XXXXXX";

      struct jpeg_decompress_struct cinfo;
      struct jpeg_error_mgr jerr;
      FILE * infile;
      JSAMPARRAY buffer;
      char *upto;
      
      if((tempfd = mkstemp(filename)) < 0){
	perror("mkstemp failed");
	exit(1);
      }
      
      write(tempfd, img + 2, size - 2);
      close(tempfd);

      /* Open a JPEG file and grab the contents */
      if ((infile = fopen(filename, "rb")) == NULL) {
	fprintf(stderr, "Cannot open input image\n");
	exit(1);
      }
      
      cinfo.err = jpeg_std_error(&jerr);
      jpeg_create_decompress(&cinfo);
      jpeg_stdio_src(&cinfo, infile);
      jpeg_read_header(&cinfo, TRUE);
      jpeg_start_decompress(&cinfo);
      
      buffer = (*cinfo.mem->alloc_sarray)
	((j_common_ptr) &cinfo, JPOOL_IMAGE, 
	 cinfo.output_width * cinfo.output_components, 1);
      
      upto = img;
      while (cinfo.output_scanline < cinfo.output_height)
	{
	  jpeg_read_scanlines(&cinfo, buffer, 1);
	  memcpy(upto, *buffer, cinfo.output_width * cinfo.output_components);
	  upto += cinfo.output_width * cinfo.output_components;
	} 
    }
  
  /* The R and B bytes seem to be swapped from RGB */
  for(i = 0; i < 640 * 480; i++)
    {
      int t = img[i * 3];
      img[i * 3] = img[i * 3 + 2];
      img[i * 3 + 2] = t;
    }

  if(dotiff)
    {
      printf("Writing TIFF image\n");

     /* Write those bytes out to a TIFF file */
      if((output = TIFFOpen("frame.tif", "w")) == NULL){
	fprintf(stderr, "Could not open outgoing image\n");
	exit(1);
      }
      
      /* Write the tiff tags to the file */
      TIFFSetField(output, TIFFTAG_IMAGEWIDTH, 640);
      TIFFSetField(output, TIFFTAG_IMAGELENGTH, 480);
      TIFFSetField(output, TIFFTAG_COMPRESSION, COMPRESSION_DEFLATE);
      TIFFSetField(output, TIFFTAG_PLANARCONFIG, PLANARCONFIG_CONTIG);
      TIFFSetField(output, TIFFTAG_PHOTOMETRIC, PHOTOMETRIC_RGB);
      TIFFSetField(output, TIFFTAG_BITSPERSAMPLE, 8);
      TIFFSetField(output, TIFFTAG_SAMPLESPERPIXEL, 3);
      
      /* Actually write the image */
      if(TIFFWriteEncodedStrip(output, 0, img, 640 * 480 * 3) == 0){
	fprintf(stderr, "Could not write image\n");
	exit(42);
      }
      
      TIFFClose(output);
    }
  
  if(doraw)
    {
      printf("Write raw bytes\n");
      if((out = creat("frame.raw", S_IRWXU)) < 0)
	{
	  perror("Opening raw image file");
	  exit(1);
	}

      write(out, img, size);
      close(out);
    }
  return 0;
}
