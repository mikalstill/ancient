#include "usblogdump.h"
#include "md5-global.h"
#include "md5.h"

extern int do_now, do_decoder, do_binarydumps, do_excessivedumps;

extern char *urb_buffer;
extern long urb_buffer_size;
extern long urb_buffer_inset;
extern int urb_reallocs;

extern char *code_buffer[section_max];
extern long code_buffer_size[section_max];
extern long code_buffer_inset[section_max];

// ----------------------------------------------------------------------
// Utility functions
// ----------------------------------------------------------------------
// This function is used to cache urb descriptions for duplicate checking
void
urb_printf (char *format, ...)
{
  va_list ap;
  char *temp;
  int templen;

  va_start (ap, format);
  if(do_now == 1)
    {
      vprintf(format, ap);
    }
  else
    {
      // Get the string which is just this output
      temp = xsnprintf (format, ap);
      templen = strlen (temp);
      
      // Append it to the buffer which is this URB
      if (urb_buffer_inset + templen + 1 > urb_buffer_size)
	{
	  urb_buffer =
	    (char *) realloc (urb_buffer, urb_buffer_size + templen + 500);
	  if (urb_buffer == NULL)
	    {
	      perror("Memory allocation error");
	      exit (42);
	    }
	  urb_buffer_size += templen + 500;
	  urb_reallocs++;
	}
      strcpy (urb_buffer + urb_buffer_inset, temp);
      urb_buffer_inset += templen;
      
      // Cleanup
      free (temp);
    }
  va_end (ap);
}

char *
xsnprintf (char *format, va_list ap)
{
  char *output = NULL;
  int size, result;

  /* We start with the size of the format string as a guess */
  size = strlen (format);
  while (1)
    {
      output = (char *) realloc (output, size);
      if (output == NULL)
	{
	  perror("Memory allocation error 2");
	  exit (42);
	}
      result = vsnprintf (output, size, format, ap);

      if (result == -1)
	{
	  /* Up to glibc 2.0.6 and Microsoft's implementation */
	  size += 100;
	}
      else
	{
	  /* Check if we are done */
	  if (result < size)
	    break;

	  /* Glibc from now on */
	  size = result + 1;
	}
    }

  return output;
}

void
urb_printf_flags(unsigned int number)
{
  char *tempbinary;
  
  urb_printf("%d [", number);

  tempbinary = to_binary((unsigned char) (number & 0xFF000000) >> 24);
  urb_printf ("%s ", tempbinary);
  free(tempbinary);

  tempbinary = to_binary((unsigned char) (number & 0x00FF0000) >> 16);
  urb_printf ("%s ", tempbinary);
  free(tempbinary);

  tempbinary = to_binary((unsigned char) (number & 0x0000FF00) >> 8);
  urb_printf ("%s ", tempbinary);
  free(tempbinary);

  tempbinary = to_binary((unsigned char) (number & 0x000000FF));
  urb_printf ("%s]\n", tempbinary);
  free(tempbinary);
}

void
urb_xfree (void *m)
{
  if (m)
    free (m);
}

char *
md5hash (char *input)
{
  MD5_CTX context;
  unsigned char *digest;

  if ((digest = (char *) malloc (16)) == NULL)
    {
      perror("Memory error 3");
      exit (42);
    }

  MD5Init (&context);
  MD5Update (&context, input, strlen (input));
  MD5Final (digest, &context);

  return digest;
}

int
hashcmp (usb_allurbs * head, int inset, int testinset, int length)
{
  int count, count2;

  for (count = 0; count < length; count++)
    {
      if (head[inset + count].deleted == 1)
	{
	  return -4;
	}
      if (head[testinset + count].deleted == 1)
	{
	  return -4;
	}

      for (count2 = 0; count2 < 16; count2++)
	{
	  if ((unsigned char) head[inset + count].md5[count2] !=
	      (unsigned char) head[testinset + count].md5[count2])
	    {
	      return -2;
	    }
	}
    }

  return 0;
}

// Do a nicely formatted dump of some data
void dump_data(char *file, long long *filep, int psize, char *type)
{
  long long count = *filep;
  int i;
  unsigned char temp;

  urb_printf("\t");
  if (do_decoder == 1)
    {
      write(decodein, type, strlen(type));
      write(decodein, " ", 1);
    }
 
  for (i = 0; i < psize; i++)
    {
      if (do_decoder == 1)
	{
	  write(decodein, file + count, 1);
	}
      else if (do_binarydumps == 1)
	{
	  if(do_excessivedumps == 1)
	    {
	      urb_printf("%3d: ", i);
	    }

	  temp = (unsigned char) file[count];
	  char *conv = to_binary(temp);
	  urb_printf("%s ", conv);
	  free(conv);

	  if(do_excessivedumps == 1)
	    {
	      urb_printf("decimal(%3d) hex(%02x)", temp, temp);
	      if(isgraph(temp))
		urb_printf(" ascii(%c)", temp);
	      urb_printf("\n\t");
	    }
	}
      else if (isgraph (file[count]))
	{
	  urb_printf (" %c ", (unsigned char) file[count]);
	}
      else if (file[count] == 0x20)
	{
	  urb_printf ("<> ");
	}
      else
	{
	  urb_printf ("%02x ", (unsigned char) file[count]);
	}
      count++;

      if(do_excessivedumps == 0)
	{
	  if (!((i + 1) % 16))
	    {
	      urb_printf (" ");
	    }
	  if (!((i + 1) % 32))
	    {
	      urb_printf ("\n\t");
	    }
	}
    }

  if(do_decoder == 1)
    {
      char decoded[2];
      int lastwasnewline = 0, len;

      write(decodein, "\n.\n", 3);
      while(1)
	{
	  if((len = read(decodeout, decoded, 1)) == 0)
	    {
	      break;
	    }
	  if(len == -1)
	    {
	      perror("Error reading from decoder");
	      exit(42);
	    }

	  decoded[len] = 0;
	  if(decoded[0] == '\n')
	    {
	      lastwasnewline = 1;
	    }
	  else if((decoded[0] == '.') && (lastwasnewline == 1))
	    {
	      break;
	    }
	  else
	    {
	      lastwasnewline = 0;
	    }

	  urb_printf("%c", decoded[0]);
	}

      read(decodeout, decoded, 1);
    }

  *filep = count;
}

char *to_binary(unsigned char number)
{
  char *retval;

  if((retval = malloc(sizeof(char) * 9)) == NULL)
    {
      perror("Could not allocate memory");
      exit(42);
    }

  memset(retval, '0', 8);
  retval[8] = 0;

  if(number & 0x80) retval[0] = '1';
  if(number & 0x40) retval[1] = '1';
  if(number & 0x20) retval[2] = '1';
  if(number & 0x10) retval[3] = '1';
  if(number & 0x08) retval[4] = '1';
  if(number & 0x04) retval[5] = '1';
  if(number & 0x02) retval[6] = '1';
  if(number & 0x01) retval[7] = '1';
 
  return retval;
}

char *to_bcd(int twobcdbytes)
{
  char *retval;

  if((retval = malloc(sizeof(char) * 5)) == NULL)
    {
      perror("Could not allocate memory");
      exit(42);
    }

  memset(retval, '0', 4);
  retval[4] = 0;

  sprintf(retval, "%02x%02x", 
	  (twobcdbytes << 24) >> 24,
	  (twobcdbytes << 16) >> 24);
  return retval;
}

// This function is like urb_printf but for code. I could probably
// one day roll the two implementations together
void
code_printf (section sect, char *format, ...)
{
  va_list ap;
  char *temp;
  int templen;

  va_start (ap, format);
  if(do_now == 1)
    {
      vprintf(format, ap);
    }
  else
    {
      // Get the string which is just this output
      temp = xsnprintf (format, ap);
      templen = strlen (temp);
      
      if (code_buffer_inset[sect] + templen + 1 > code_buffer_size[sect])
	{
	  code_buffer[sect] =
	    (char *) realloc (code_buffer[sect], code_buffer_size[sect] + templen + 500);
	  if (code_buffer[sect] == NULL)
	    {
	      perror("Memory allocation error");
	      exit (42);
	    }
	  code_buffer_size[sect] += templen + 500;
	}
      strcpy (code_buffer[sect] + code_buffer_inset[sect], temp);
      code_buffer_inset[sect] += templen;
      
      // Cleanup
      free (temp);
    }
  va_end (ap);
}
