// Routines to handle the sound for Vic, the Bug

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/soundcard.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>

int slots[100];

unsigned int toInteger(char *in){
  unsigned int ret;
  int i;
  char *p = ((char *) &ret);

  for(i = 0; i < 4; i++){
    *p++ = in[i];
  }

  return ret;
} 

char *getBytes(int input, size_t number, char *expect){
  char *data;
  int i;
  ssize_t numRead;

  if((data = malloc(number + 1)) == NULL){
    fprintf(stderr, "Malloc error\n");
    exit(0);
  }

  if((numRead = read(input, data, number)) < 1){
    fprintf(stderr, "Malformed wav file (EOF)\n");
    exit(0);
  }
  data[numRead] = 0;

  if(expect != NULL){
    for(i = 0; i < numRead; i++)
      data[i] = tolower(data[i]);

    if(strcmp(expect, data) != 0){
      fprintf(stderr, "Read \"");
      for(i = 0; i < numRead; i++)
	fprintf(stderr, "%c", (isprint(data[i])) ? data[i] : '%');
      fprintf(stderr, "\", expected \"%s\"\n", expect);
    }
  }

  return data;
}

void playSound(int slotNumber, char *filename){
  slots[slotNumber] = 0;

  switch(fork()){
  case -1:
    // Parent with failure
    fprintf(stderr, "Sound play fork error\n");
    return;
    break;

  case 0:
    // Child
    {
      int soundHandle = -1, fileHandle = -1;
      int channels = 1; // stereo
      int format;
      unsigned int rate = 22050;
      unsigned int fmtLen;
      int bytes = 0;
      long i;
      char *data, *data2;
      char *tempPtr;
      ssize_t numRead;

      printf("Play sound %s\n", filename);

      if((fileHandle = open(filename, O_RDONLY)) == -1){
	fprintf(stderr, "Could not open the specified wav file: %s\n",
		filename);
	exit(0);
      }

      // Check the format of the wav file
      free(getBytes(fileHandle, 4, "riff"));
      free(getBytes(fileHandle, 4, NULL)); // Length of package
      free(getBytes(fileHandle, 4, "wave"));
      free(getBytes(fileHandle, 4, "fmt "));

      tempPtr = getBytes(fileHandle, 4, NULL);
      fmtLen = toInteger(tempPtr);
      free(tempPtr);

      free(getBytes(fileHandle, 2, NULL)); // Reserved

      tempPtr = getBytes(fileHandle, 2, NULL);
      channels = tempPtr[0] - 1;
      free(tempPtr);

      tempPtr = getBytes(fileHandle, 4, NULL);
      rate = toInteger(tempPtr);
      free(tempPtr);

      free(getBytes(fileHandle, 4, NULL)); // Bytes per second

      tempPtr = getBytes(fileHandle, 2, NULL);
      bytes = tempPtr[0];
      free(tempPtr);

      free(getBytes(fileHandle, 2, NULL)); // Bits per sample
      free(getBytes(fileHandle, 4, "data"));
      free(getBytes(fileHandle, 3, NULL)); // Length of data

      // Open the sound interface
      if((soundHandle = open("/dev/dsp", O_WRONLY)) == -1){
	fprintf(stderr, "Couldn't open the sound device\n");
	exit(0);
      }

      if(ioctl(soundHandle, SNDCTL_DSP_STEREO, &channels) == -1){
	fprintf(stderr, "Could not set stereo\n");
	exit(0);
      }
      else{
	printf("\nStereo set to %d\n", channels);
      }

      switch(bytes){
      case 1:
	format = AFMT_U8;
	break;

      case 2:
	if(channels == 0)
	  format = AFMT_S16_LE;
	else
	  format = AFMT_U8;
	break;

      case 4:
	fprintf(stdout, "Stereo wav file\n");
	format = AFMT_S16_LE;
	break;

      default:
	fprintf(stderr, "Unknown byte rate for sound\n");
	exit(0);
      }

      if(ioctl(soundHandle, SNDCTL_DSP_SETFMT, &format) == -1){
	fprintf(stderr, "Could not set format\n");
	exit(0);
      }
      else{
	printf("Format set to %d\n", format);
      }

      if(ioctl(soundHandle, SNDCTL_DSP_SPEED, &rate) == -1){
	fprintf(stderr, "Could not set rate\n");
	exit(0);
      }
      else{
	printf("Speed set to %d\n", rate);
      }

      fprintf(stdout,
	      "Playing file %s: channels = %d, rate = %d, bytes = %d\n",
	      filename, channels, rate, bytes);

      if((data2 = malloc(1024)) == NULL){
	fprintf(stderr, "Memory allocation error\n");
	exit(0);
      }

      while(1){
	if((numRead = read(fileHandle, data, 1024)) == 0)
	  break;

	for(i = 0; i < numRead; i+=2){
	  data2[i] = data[i+1];
	  data2[i+1] = data[i];
	}

	if(write(soundHandle, data2, numRead) == -1){
	  fprintf(stderr, "Error writing to the sound device\n");
	  exit(0);
	}
      }

      // Get here when done?
      close(fileHandle);
      close(soundHandle);
      free(data);
      exit(0);
    }
    break;
   
  default:
    return;
    break;
  }
}
