/******************************************************************************
  text.c

  Change Control:                                                      DDMMYYYY
    Michael Still    File created                                      22062000
                     Improvements to text handling                     30062000

  Purpose:
    Functions related to displaying text on a PDF page.
******************************************************************************/

#include "constants.h"
#include "functions.h"

void textbox(pdf *output, page *thisPage, int top, int left, int bottom,
       int right, char *text){
  // Add a box with some text in it into the PDF page
  object      *textobj;
  char        *currentToken, *strtokVictim = NULL, delim[10];
  int         internalTop, internalLeft, displayedFirstPart = gFalse;
  object      *subdict, *subsubdict, *fontObj;

  /***************************************************************************
     Some text handling
  ***************************************************************************/

  // Make an object to store the text
  //  textobj = newobject(output, gNormal);
  //  addchild(thisPage->obj, textobj);
  //  adddictitem(thisPage->obj, "Contents", gObjValue, textobj);
  textobj = thisPage->contents;

  // Is there a font setup?
  if(output->currentFont == NULL){
    setfont(output,
      createfont(output, "Helvetica", 1, "MacRomanEncoding"));
  }

  // Is there a text size setup?
  if(output->currentFontSize == -1)
    setfontsize(output, 16);

  /***************************************************************************
    This font has to be referred to in the resources entry in the dictionary.

    The resources dictionary looks a lot like:
      /Resources <<
        /Font <<
          /...Name... ...Obj Reference...
          >>
        >>

    An example line is /MikalsFont 54 0 R
  ***************************************************************************/

  // Find the font object needed
  if((fontObj = getfontobj(output, output->currentFont)) == NULL)
    error("Could not find the font requested.");

  // We make an object not just a dictionary because this is what
  // adddictitem needs
  subsubdict = newobject(output, gPlaceholder);
  adddictitem(subsubdict, output->currentFont, gObjValue, fontObj);
  
  subdict = newobject(output, gPlaceholder);
  adddictitem(subdict, "Font", gDictionaryValue, subsubdict->dict);

  // And put this into the PDF
  adddictitem(thisPage->obj, "Resources", gDictionaryValue, subdict->dict);
  
  /***************************************************************************
    PDF deals in points, with the bottom left hand side of the page being at
    0,0. I think this is anti-intuitive for most users, so I convert to that
    coordinate system from a normal one here. I define the point 0,0 as being
    the top left of the page. Spacing is still done in points though...

    Further more, the specification for PDF requires that you give the
    location of the bottom of the line of text, not the top. We change this
    my playing games with the known height of the line.
  ***************************************************************************/

  // We know the width and height from the page object
  internalTop = thisPage->height - top - output->currentFontSize;
  internalLeft = left;

  /***************************************************************************
    Back to inserting the text
  ***************************************************************************/

  // Build the command buffer
  // This is a little dodgy at the moment because I do not understand the
  // PS matrix environment well enough to be writing this sort of code. I am
  // going to have to have a look into this a little more...
  textstreamprintf(textobj, "1 0 0 1 %d %d Tm\n",
    internalLeft, internalTop);

  // There are now a whole bunch of options that may or may not need to be set
  if(output->currentFontMode != 0){
    textstreamprintf(textobj, "%d Tr\n", output->currentFontMode);
  }

  if(output->currentCharacterSpacing != 0){
    textstreamprintf(textobj, "%.2f Tc\n", 
      output->currentCharacterSpacing);
  }

  if(output->currentWordSpacing != 0){
    textstreamprintf(textobj, "%.2f Tw\n", 
      output->currentWordSpacing);
  }

  if(output->currentHorizontalScaling != 1){
    textstreamprintf(textobj, "%.0f Tz\n",
      output->currentHorizontalScaling * 100);
  }

  if(output->currentLeading != 0){
    textstreamprintf(textobj, "%.2f TL\n", output->currentLeading);
  }

  // Set the font that we want to use
  textstreamprintf(textobj, "/%s %d Tf\n", 
    output->currentFont, output->currentFontSize);

  /***************************************************************************
    PDFs are funny in that we have to specify where line breaks are to occur.
    We allow the programmer to specify line breaks by putting the characters
    '\n' into the string that we is having us put into the PDF. We therefore
    need to find this character sequence and handle it...

                        ---------------------------

    We also now support super and subscript text options, which means we need
    a portable way for the programmer to express these to us. I have decided
    to go with a scheme in which the character sequence:
           \4U<<>>

    means superscript by the amount which exists at <<>>, which is a one digit
    number expressed in ASCII

    And the sequence:
	   \5D<<>>

    means subscript by the amount passed.

    Finally, we need to be able to get back into normal mode, which is done
    with a:
           \6N
  ***************************************************************************/

  // Get the first token
  if((strtokVictim = (char *) malloc(sizeof(char) * strlen(text))) == NULL)
    error("Could not make space for temporary copy of textbox text.");
  strcpy(strtokVictim, text);

  // Build the delimiter string
  sprintf(delim, "\n%c%c%c", 4, 5, 6);

  currentToken = strtok(strtokVictim, delim);

  while(currentToken != NULL){
    // If we haven't displayed that first part that would otherwise be missed
    // do so now
    if(displayedFirstPart == gFalse){
      textstreamprintf(textobj, "(%s) '\n", strtokVictim);
      displayedFirstPart = gTrue;
    }

    switch(text[currentToken - strtokVictim - 1]){
    case '\n':
      textstreamprintf(textobj, "(%s) '\n", currentToken);
      break;

    case 4:
      textstreamprintf(textobj, "%c Ts (%s) Tj\n",
	currentToken[0], currentToken + 1);
      break;

    case 5:
      textstreamprintf(textobj, "-%c Ts (%s) Tj\n",
	currentToken[0], currentToken + 1);
      break;

    case 6:
      textstreamprintf(textobj, "0 Ts (%s) Tj\n", currentToken);
      break;
    }

    currentToken = strtok(NULL, delim);
  }
}
