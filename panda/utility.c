/******************************************************************************
  utility.c

  Change Control:                                                      DDMMYYYY
    Michael Still    File created                                      17062000
                     Obscure bug in byte offsets fixed.                22062000
                       Windows printf will add a '\r' even if
                       you don't ask it to, and even if you
                       have added your own. I need to take
                       this into account in the byte offsets.
		     Added textstreamprintf(...) call.                 17072000
		     Added pdfputc call                                22072000

  Purpose:
    Utility functions for the panda library.
******************************************************************************/

#include "constants.h"
#include "functions.h"

// Print a possibly complex string into the PDF file and make sure the offset
// into the PDF file is stored correctly (dispite Windows)
void pdfprintf(pdf *file, char *format, ...){
  // Print some information into the pdf file, but also record how many
  // bytes we have gone into the file. POSSIBLE BUG: Cannot print more than
  // 1024 bytes at a time. I am not sure how to decide how much memory to
  // allocate...
  char     *buffer, *formatString;
  int      newlineCount = 0, counter = 0, indent = 0, actualLen;
  va_list  argPtr;

  // This is a little strange... On a windows machine, printf inserts \r's 
  // for us, even if we have them already. Therefore we don't put them into 
  // the format string, and then add them here if we were not compiled on 
  // windows
#if defined WINDOWS
  if((formatString = malloc(strlen(format) * sizeof(char))) == NULL)
    error("Could not make temporary printing space.");

  strcpy(formatString, format);
#else
  // We need to go through the format string and replace \n with \r\n
  // because lines in a PDF end with \r\n (one of the options)

  // Count the number of \n's.
  for(counter = 0; counter < strlen(format); counter++)
    if(format[counter] == '\n') newlineCount++;

  if((formatString = 
    malloc((strlen(format) + newlineCount) * sizeof(char))) == NULL)
    error("Could not make temporary printing space.");

  for(counter = indent = 0; counter < strlen(format); counter++){
    if(format[counter] == '\n'){
      formatString[indent++] = '\r';
      formatString[indent++] = format[counter];
      }
     else

    formatString[indent++] = format[counter];
    }

  formatString[indent] = 0;
#endif

  // Now we need to make a best guess at how long buffer needs to be -- it is 
  // hardly ever longer than 1k...
  if((buffer = malloc(1024 * sizeof(char))) == NULL)
    error("Could not grab space for a pdfprintf call (1k).");

  // Build the information
  va_start(argPtr, format);
  if((actualLen = vsnprintf(buffer, 1024, formatString, argPtr)) > 1020){
    // We did not fit! Try again...
    free(buffer);

    if((buffer = malloc(actualLen * sizeof(char))) == NULL)
      error("Could not grab space for a pdfprintf call (actual).");

    if(vsnprintf(buffer, actualLen, formatString, argPtr) > actualLen){
      error("Really bad file i/o error.");
    }
  }

  // Record how long it was
  file->byteOffset += strlen(buffer);

#if defined WINDOWS
  // Count how many \n's there are and take into account the \r windows will
  // add (change for unix)

  // POSSIBLE BUG: Binary textstreams may be corrupted by Windows here
  for(counter = 0; counter < strlen(buffer); counter++){
    if(buffer[counter] == '\n') newlineCount++;
    }

  // Add this number to the byteOffset
  file->byteOffset += newlineCount;
#endif

  // Store it
  fprintf(file->file, "%s", buffer);

  // Free the temp string
  //  free(formatString);
  va_end(argPtr);
}

// Append some text to the textstream that we are creating for a given page
void textstreamprintf(object *textobj, char *format, ...){
  va_list   argPtr;
  char      buffer[2048];

  va_start(argPtr, format);
  vsprintf(buffer, format, argPtr);

  // This new object has a textstream with the text in it
  appendtextstream(textobj, buffer, strlen(buffer));

  va_end(argPtr);
}

// Put just one character into the PDf file, while updating the offset so that
// the xref table works later on
void pdfputc(pdf *output, int c){
  fputc(c, output->file);
  output->byteOffset++;
}
