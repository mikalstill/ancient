#include "pandaedit.h"
#include "objectmodel.h"
#include "stringArray.h"

extern pdf *thePDF;
extern object *currentObject;
extern stack<dictionary> currentDictionary;
extern stack<string> currentDictionaryName;
extern progressCallback gProgress;

void pandaedit_begindocument(int event, va_list argptr){
  thePDF = new pdf(va_arg(argptr, char *));
}

void pandaedit_specversion(int event, va_list argptr){
  string version = (char *) va_arg(argptr, char *);
  thePDF->setSpecVer(atof(version.c_str()));
}

// Setup an object
void pandaedit_objstart(int event, va_list argptr){
  int objnum, objgen;
  objnum = va_arg(argptr, int);
  objgen = va_arg(argptr, int);

  // Setup a global for the current object
  if(currentObject) delete currentObject;
  currentObject = new object(objnum, objgen);

  // Setup a stack of dictionaries for this object
  while(currentDictionary.size() > 0)
    currentDictionary.pop();
  while(currentDictionaryName.size() > 0)
    currentDictionaryName.pop();

  dictionary temp;
  currentDictionary.push(temp);
  currentDictionaryName.push("TOP");

  if(gProgress != NULL)
    gProgress();
}

// Now that we have finished the object, push it into the PDF
void pandaedit_objend(int event, va_list argptr){
  if(currentObject != NULL){
    if(!currentDictionary.empty()){
      printf("DEBUG: Adding dictionary because of object end\n");
      currentObject->addDictionary(currentDictionary.top());
      currentDictionary.pop();
    }

    thePDF->addObject(*currentObject);
  }
  else{
    printf("DEBUG: Attempt to finish a NULL object\n");
  }
}

void pandaedit_dictitem_string(int event, va_list argptr){
  char *name, *value;
  
  name = va_arg(argptr, char *);
  value = va_arg(argptr, char *);

  if(currentDictionary.empty()){
    fprintf(stderr, "Adding item to empty dictionary\n");
    return;
  }

  dictitem temp(dictitem::diTypeString, name);
  temp.setValue(string(value));
  currentDictionary.top().add(temp);
}

void pandaedit_dictitem_name(int event, va_list argptr){
  char *name, *value;
  
  name = va_arg(argptr, char *);
  value = va_arg(argptr, char *);
 
  if(currentDictionary.empty()){
    fprintf(stderr, "Adding item to empty dictionary\n");
    return;
  }

  dictitem temp(dictitem::diTypeName, name);
  temp.setValue(string(value));
  currentDictionary.top().add(temp);
}

void pandaedit_dictitem_arraystart(int event, va_list argptr){
  char *name;
  
  name = va_arg(argptr, char *);
  printf("  Array %s starts\n", name);
}

void pandaedit_dictitem_arrayitem(int event, va_list argptr){
  char *value;
  
  value = va_arg(argptr, char *);
  printf("  [Array] %s\n", value);
}

void pandaedit_dictitem_arrayend(int event, va_list argptr){
  char *name;
  
  name = va_arg(argptr, char *);
  printf("  Array %s ends\n", name);
}

void pandaedit_dictitem_object(int event, va_list argptr){
  char *name, *value;
  
  name = va_arg(argptr, char *);
  value = va_arg(argptr, char *);

  if(currentDictionary.empty()){
    fprintf(stderr, "Adding item to empty dictionary\n");
    return;
  }

  dictitem temp(dictitem::diTypeObjectReference, name);
  stringArray token(value, " ");
  temp.setValue(atoi(token[0].c_str()), atoi(token[1].c_str()));
  currentDictionary.top().add(temp);
}

void pandaedit_dictitem_dict(int event, va_list argptr){
  char *name;

  name = va_arg(argptr, char *);
  printf("DEBUG: Subdictionary \"%s\" starts\n", name);
  dictionary temp;
  currentDictionary.push(temp);
}

void pandaedit_dictitem_dictend(int event, va_list argptr){
  char *name;

  name = va_arg(argptr, char *);

  if(currentDictionary.empty()){
    printf("DEBUG: A non-existant dictionary ended\n");
    return;
  }    

   if(name == NULL){
     if(currentObject != NULL){
       printf("DEBUG: Adding dictionary because of subdictionary end (but it isn't a subdictionary)\n");
       currentObject->addDictionary(currentDictionary.top());
     }
     else{
       printf("DEBUG: Attempt to add a dictionary to a null object\n");
     }
     currentDictionary.pop();
   }
   else{
     printf("DEBUG: Adding a subdictionary named %s to the parent dictionary\n", name);
     dictitem temp(dictitem::diTypeDictionary, name);
     temp.setValue(currentDictionary.top());
     currentDictionary.pop();
     currentDictionary.top().add(temp);

     if(!currentDictionaryName.empty())
       currentDictionaryName.pop();    
   }
}

void pandaedit_dictitem_int(int event, va_list argptr){
  int value;
  char *name;

  name = va_arg(argptr, char *);
  value = va_arg(argptr, int);

  if(currentDictionary.empty()){
    fprintf(stderr, "Adding item to empty dictionary\n");
    return;
  }

  dictitem temp(dictitem::diTypeInt, name);
  temp.setValue(value);
  currentDictionary.top().add(temp);
}

void pandaedit_stream(int event, va_list argptr){
  char *streamData;
  int streamDataLen;

  streamData = va_arg(argptr, char *);
  streamDataLen = va_arg(argptr, int);
  printf("DEBUG: Parser found a stream of length %d\n", streamDataLen);
  currentObject->addStream(streamData, streamDataLen);
}
