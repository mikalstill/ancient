/******************************************************************************
  Objects.c

  Change Control:                                                      DDMMYYYY
    Michael Still    File created                                      03062000

  Purpose:
    Panda is based on the concept of objects. This file contains all of
    methods needed to maintain the objects that we have.
******************************************************************************/

#include <panda/functions.h>
#include <panda/constants.h>

/******************************************************************************
  Create an object with error checking.
******************************************************************************/

panda_object *
panda_newobject (panda_pdf * doc, int type)
{
  // Do all the magic needed to create an object
  panda_object *created;

  // Get some memory
  created = (panda_object *) panda_xmalloc(sizeof (panda_object));

  // We have no children at the moment
  created->children = (panda_child *) panda_xmalloc(sizeof (panda_child));

  ((panda_child *) created->children)->next = NULL;
  (panda_child *) created->cachedLastChild = NULL;

  // Initialise the dictionary
  created->dict = (panda_dictionary *) panda_xmalloc(sizeof (panda_dictionary));
  created->dict->next = NULL;

  // By default this object is not a pages object
  created->isPages = panda_false;

  if (type == gPlaceholder)
    {
      // This is a placeholder object, therefore it's number is -1
      created->number = -1;

      return created;
    }

  // Initialise the object number
  created->number = doc->nextObjectNumber++;

#if defined DEBUG
  printf ("Created object %d\n", created->number);
#endif

  // We don't have any streams at the moment
  created->binarystream = NULL;
  created->layoutstream = NULL;

  // There is no font defined
  created->currentSetFont = NULL;

  // We are not in text mode within the layout stream at the start
  created->textmode = panda_false;

  // New objects have a generation number of 0 by definition
  created->generation = 0;

  // Add this new object to the end of the linked list that we use to append
  // the xref table onto the end of the PDF
  doc->xrefTail->this = created;

  // Make space for the next one
  doc->xrefTail->next = panda_xmalloc(sizeof (panda_xref));
  doc->xrefTail->next->next = NULL;
  doc->xrefTail = doc->xrefTail->next;

  // Return
  return created;
}

panda_dictionary *
panda_adddictitem (panda_dictionary * input, char *name, int valueType, ...)
{
  // Add an item to the dictionary in the object
  panda_dictionary *dictNow;
  va_list argPtr;
  char *value;
  panda_object *objValue;
  panda_objectarray *currentObjectArray;
  panda_dictionary *dictValue, *prevDictValue;
  int overwriting = panda_false;

#if defined DEBUG
  printf ("Added dictionary item %s to object (type = %d)\n", name,
	  valueType);
  fflush (stdout);
#endif

  // Find the end of the dictionary or something with this name already
  dictNow = input;
  while ((dictNow->next != NULL) && (strcmp (dictNow->name, name) != 0))
    {
      dictNow = dictNow->next;
    }

  // Make a new end to the dictionary if needed
  if (dictNow->next == NULL)
    {
      dictNow->next = (panda_dictionary *) panda_xmalloc(sizeof(panda_dictionary));

      // Setup
      dictNow->next->next = NULL;
      dictNow->objectarrayValue = NULL;
      dictNow->dictValue = NULL;
      dictNow->textValue = NULL;

#if defined DEBUG
      printf (" (This is a new dictionary element)\n");
#endif
    }
  else
    {
#if defined DEBUG
      printf (" (Overwriting a dictionary element)\n");
#endif
      overwriting = panda_true;
    }

  // Work with the last argument
  va_start (argPtr, valueType);

  // And add some content to this entry if needed
  if (overwriting == panda_false)
    {
      dictNow->name = (char *) panda_xmalloc((strlen (name) + 1) * sizeof (char));
      strcpy (dictNow->name, name);

      // Record the type
      dictNow->valueType = valueType;
    }
  else
    switch (valueType)
      {
      case panda_objectarrayvalue:
      case panda_dictionaryvalue:
      case panda_textvalue:
      case panda_literaltextvalue:
      case panda_brackettedtextvalue:
      case panda_integervalue:
      case panda_objectvalue:
	break;

      default:
	panda_error ("Overwriting some dictionary types not yet supported.");
      }

  switch (valueType)
    {
    case panda_textvalue:
    case panda_literaltextvalue:
    case panda_brackettedtextvalue:
      // Are we overwriting?
      if ((overwriting == panda_true) && (dictNow->textValue != NULL))
	free (dictNow->textValue);

      // Get the value
      value = va_arg (argPtr, char *);
      dictNow->textValue = 
	(char *) panda_xmalloc((strlen (value) + 3) * sizeof (char));
      dictNow->textValue[0] = '\0';

      // Some stuff for different types
      if (valueType == panda_textvalue)
	strcat (dictNow->textValue, "/");
      if (valueType == panda_brackettedtextvalue)
	strcat (dictNow->textValue, "(");

      // The string
      strcat (dictNow->textValue, value);

      // Some more stuff for different types
      if (valueType == panda_brackettedtextvalue)
	strcat (dictNow->textValue, ")");
      break;

    case panda_integervalue:
      dictNow->intValue = va_arg (argPtr, int);
      break;

    case panda_objectvalue:
      // Are we overwriting?
      if ((overwriting == panda_true) && (dictNow->textValue != NULL))
	free (dictNow->textValue);

      objValue = va_arg (argPtr, panda_object *);

      // We assume we need no more than 20 characters to store this. This
      // should be fine
      dictNow->textValue = (char *) panda_xmalloc(sizeof (char) * 20);

      sprintf (dictNow->textValue, "%d %d R", objValue->number,
	       objValue->generation);
      break;

    case panda_objectarrayvalue:
      objValue = va_arg (argPtr, panda_object *);

      if (dictNow->objectarrayValue == NULL)
	{
	  dictNow->objectarrayValue = 
	    (panda_objectarray *) panda_xmalloc(sizeof (panda_objectarray));
	  dictNow->objectarrayValue->next = NULL;
	}

      // Find the end of the object array list
      currentObjectArray = dictNow->objectarrayValue;
      while (currentObjectArray->next != NULL)
	currentObjectArray = currentObjectArray->next;

      // Make a new array entry
      currentObjectArray->next = 
	(panda_objectarray *) panda_xmalloc(sizeof (panda_objectarray *));

      // Append
      currentObjectArray->number = objValue->number;
      currentObjectArray->generation = objValue->generation;
      currentObjectArray->next->next = NULL;
      break;

    case panda_dictionaryvalue:
      // This is a sub-dictionary
      dictValue = va_arg (argPtr, panda_dictionary *);

      if (overwriting == panda_false)
	{
	  // This is a new dictionary item, just copy the info across
	  dictNow->dictValue = dictValue;

#if defined DEBUG
	  printf ("Added a subdictionary in its full glory\n");
#endif
	}
      else
	{
	  // We are appending to a subdictionary item -- we need to go through all
	  // of the subdictionary items we just got handed and add them to the
	  // subdictionary that is already here
	  while (dictValue->next != NULL)
	    {
#if defined DEBUG
	      printf ("Adding a subdictionary element named %s to existing\n",
		      dictValue->name);
#endif

	      switch (dictValue->valueType)
		{
		case panda_textvalue:
		case panda_brackettedtextvalue:
		case panda_literaltextvalue:
		  panda_adddictitem (dictNow->dictValue, dictValue->name,
			       dictValue->valueType, dictValue->textValue);
		  break;

		case panda_objectvalue:
		  panda_adddictitem (dictNow->dictValue, dictValue->name,
			       panda_literaltextvalue, dictValue->textValue);
		  break;

		case panda_dictionaryvalue:
		  panda_adddictitem (dictNow->dictValue, dictValue->name,
			       dictValue->valueType, dictValue->dictValue);
		  break;

		case panda_integervalue:
		  panda_adddictitem (dictNow->dictValue, dictValue->name,
			       dictValue->valueType, dictValue->intValue);
		  break;

		case panda_objectarrayvalue:
		  panda_adddictitem (dictNow->dictValue, dictValue->name,
			       dictValue->valueType,
			       dictValue->objectarrayValue);
		  break;
		}

	      prevDictValue = dictValue;
	      dictValue = dictValue->next;
	    }
	}
      break;
    }

  // Stop dealing with arguments
  va_end (argPtr);

  // Return the dictionary item we changed (used in the lexer)
  return dictNow;
}

void *
panda_getdictvalue (panda_dictionary * dictValue)
{
  // Return a panda_dictionary value
  switch (dictValue->valueType)
    {
    case panda_dictionaryvalue:
      return dictValue->dictValue;
      break;

      // This line is a little scary -- we are going to make the int look like
      // a pointer for just a little while
    case panda_integervalue:
      return dictValue->intValue;
      break;

    case panda_textvalue:
    case panda_literaltextvalue:
    case panda_objectvalue:
      return dictValue->textValue;
      break;

    case panda_objectarrayvalue:
      return dictValue->objectarrayValue;
      break;
    }

  return NULL;
}

panda_dictionary *
panda_getdict (panda_dictionary * head, char *name)
{
  while ((strcmp (head->name, name) != 0) && (head->next != NULL))
    head = head->next;

  if (head->next == NULL)
    return NULL;
  return head;
}

void
panda_freeobject (panda_pdf * output, panda_object * freeVictim)
{

#if defined DEBUG
  printf ("Cleaning up object number %d\n", freeVictim->number);
#endif

  // We should skip placeholder objects (I think)
  if (freeVictim->number != -1)
    {
      // Free the object and all it's bits -- free of a NULL does nothing! But
      // not in dmalloc!!!
      if (freeVictim->layoutstream != NULL)
	free (freeVictim->layoutstream);
      if (freeVictim->binarystream != NULL)
	free (freeVictim->binarystream);
      if (freeVictim->currentSetFont != NULL)
	free (freeVictim->currentSetFont);

      panda_freedictionary (freeVictim->dict);
    }

  // free(freeVictim);
}

void
panda_freedictionary (panda_dictionary * freeDict)
{
  panda_dictionary *now, *prev;
  int endoftheline = panda_true;

  // Still need to free the dictionary... This can be made more efficient
  while (freeDict->next != NULL)
    {
      now = freeDict;
      prev = NULL;

      while (now->next != NULL)
	{
	  prev = now;
	  now = now->next;
	}

      if (endoftheline == panda_false)
	{
	  free (now->name);
	  if (now->textValue != NULL)
	    free (now->textValue);
	  if (now->dictValue != NULL)
	    panda_freedictionary (now->dictValue);
	}
      else
	endoftheline = panda_false;

      free (now);

      if (prev != NULL)
	prev->next = NULL;
    }

  // And free that initial dictionary element
  if (freeDict != NULL)
    {
      free (freeDict->name);
      if (freeDict->textValue != NULL)
	free (freeDict->textValue);
      if (freeDict->dictValue != NULL)
	panda_freedictionary (freeDict->dictValue);

      free (freeDict);
    }
}


void
panda_writeobject (panda_pdf * output, panda_object * dumpTarget)
{
  // Do all that is needed to dump a PDF object to disk
  unsigned long count;

#if defined DEBUG
  printf ("Writing object number %d\n", dumpTarget->number);
#endif

  // We don't dump place holder objects (number = -1)
  if (dumpTarget->number != -1)
    {
      // Remember the byte offset that was the start of this object -- this is
      // needed for the XREF later
      dumpTarget->byteOffset = output->byteOffset;

    /*************************************************************************
      Do we have a layoutstream? If we do, work out the length of the stream 
      and save that as a dictionary element.

      We also handle binarystreams here.

    *************************************************************************/

#if defined DEBUG
      printf ("Layoutstream is %s\n",
	      (dumpTarget->layoutstream == NULL) ? "NULL" : "not NULL");
      printf ("Binarystream is %s\n",
	      (dumpTarget->binarystream == NULL) ? "NULL" : "not NULL");
#endif

      if (dumpTarget->layoutstream != NULL)
	{
	  panda_adddictitem (dumpTarget->dict, "Length", panda_integervalue,
		       strlen (dumpTarget->layoutstream) - 1);
	}

      // We cannot have a layoutstream and a binary stream in the same object
      else if (dumpTarget->binarystream != NULL)
	{
	  panda_adddictitem (dumpTarget->dict, "Length", panda_integervalue,
		       dumpTarget->binarystreamLength);
	}

      // We are going to dump the named object (and only the named object) to 
      // disk
      panda_printf (output, "%d %d obj\n",
		 dumpTarget->number, dumpTarget->generation);

      panda_writedictionary (output, dumpTarget, dumpTarget->dict);

      // Do we have a layoutstream?
      if (dumpTarget->layoutstream != NULL)
	{
	  panda_print (output, "stream\n");
	  panda_printf (output, "%s", dumpTarget->layoutstream);
	  panda_print (output, "endstream\n");
	}

      // Do we have a binary stream? We cannot have both cause how would be 
      // differentiate?
      else if (dumpTarget->binarystream != NULL)
	{
	  panda_print (output, "stream\n");

	  for (count = 0; count < dumpTarget->binarystreamLength; count++)
	    panda_putc (output, dumpTarget->binarystream[count]);

	  panda_print (output, "\nendstream\n");
	}

      panda_print (output, "endobj\n");
    }
}

void
panda_writedictionary (panda_pdf * output, panda_object * obj, panda_dictionary * incoming)
{
  // Recursively write the dictionary out (including sub-dictionaries)
  panda_objectarray *currentObjectArray;
  panda_dictionary *dictNow;
  int atBegining = panda_true;
  panda_child *currentKid;

#if defined DEBUG
  printf ("Starting to write a dictionary\n");
#endif

  // The start of the dictionary
  panda_print (output, "<<\n");

  // Enumerate the dictionary elements
  dictNow = incoming;

  while (dictNow->next != NULL)
    {
      switch (dictNow->valueType)
	{
	case panda_textvalue:
	case panda_objectvalue:
	case panda_literaltextvalue:
	case panda_brackettedtextvalue:
#if defined DEBUG
	  printf ("Writing a text value named %s into the dictionary\n",
		  dictNow->name);
#endif

	  panda_printf (output, "\t/%s %s\n", dictNow->name, dictNow->textValue);

	  // If the type is type, then possibly output the Kids line for the pages
	  // object
	  if ((strcmp (dictNow->name, "Type") == 0)
	      && (obj->isPages == panda_true))
	    {
	      panda_print (output, "\t/Kids [");

	      // Do the dumping
	      currentKid = obj->children;

	      while (currentKid->next != NULL)
		{
		  if (atBegining == panda_false)
		    panda_print (output, " ");
		  else
		    atBegining = panda_false;

		  panda_printf (output, "%d %d R",
			     currentKid->me->number,
			     currentKid->me->generation);

		  // Next
		  currentKid = currentKid->next;
		}

	      // End it all
	      panda_print (output, "]\n");
	    }
	  break;

	case panda_integervalue:
#if defined DEBUG
	  printf ("Writing a int value with name %s into the dictionary\n",
		  dictNow->name);
#endif

	  panda_printf (output, "\t/%s %d\n", dictNow->name, dictNow->intValue);
	  break;

	case panda_objectarrayvalue:
#if defined DEBUG
	  printf
	    ("Writing an object array value with name %s into dictionary\n",
	     dictNow->name);
#endif

	  // Start the array in the file
	  atBegining = panda_true;

	  panda_printf (output, "\t/%s [", dictNow->name);

	  // Go through the array list until the end
	  currentObjectArray = dictNow->objectarrayValue;
	  while (currentObjectArray->next != NULL)
	    {
	      if (atBegining == panda_true)
		atBegining = panda_false;
	      else
		panda_print (output, " ");

	      panda_printf (output, "%d %d R",
			 currentObjectArray->number,
			 currentObjectArray->generation);

	      currentObjectArray = currentObjectArray->next;
	    }

	  // Finish the array
	  panda_print (output, "]\n");
	  break;

	case panda_dictionaryvalue:
	  // These are handled recursively
	  if (dictNow->dictValue == NULL)
	    panda_error ("Subdictionary value erroneously NULL.");

#if defined DEBUG
	  printf ("Output the subdictionary starting with the name %s\n",
		  dictNow->dictValue->name);
#endif

	  panda_printf (output, "\t/%s ", dictNow->name);

	  panda_writedictionary (output, output->dummyObj, dictNow->dictValue);
	  break;

	default:
	  panda_error ("Unknown dictionary type");
	}

      dictNow = dictNow->next;
    }

  panda_print (output, ">>\n");
}

void
panda_addchild (panda_object * parentObj, panda_object * panda_childObj)
{
  panda_child *currentChild = parentObj->children;

  // Find the end of the list of children
  if (parentObj->cachedLastChild != NULL)
    currentChild = parentObj->cachedLastChild;

  while (currentChild->next != NULL)
    currentChild = currentChild->next;

  // Make a new end
  currentChild->next = (panda_child *) panda_xmalloc(sizeof (panda_child));
  currentChild->next->next = NULL;

  // Make me be the child object
  currentChild->me = panda_childObj;

  // Cache it
  parentObj->cachedLastChild = currentChild;
}

void
panda_traverseobjects (panda_pdf * output, panda_object * dumpTarget, int direction,
		 traverseFunct function)
{
  // Write out an object and all of it's children. This may be done with
  // recursive calls and writeobject()
  panda_child *currentChild;

  // No children
  if (((panda_child *) dumpTarget->children)->next == NULL)
    {
      function (output, dumpTarget);
      return;
    }

  // Otherwise, for me and then for all children
  if (direction == panda_down)
    function (output, dumpTarget);

  currentChild = dumpTarget->children;
  while (currentChild->next != NULL)
    {
      panda_traverseobjects (output, currentChild->me, direction, function);
      currentChild = currentChild->next;
    }

  if (direction == panda_up)
    function (output, dumpTarget);
}
