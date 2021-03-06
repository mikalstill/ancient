// This file defined the PDF object model in terms of c++

#include <vector>
#include <string>
#include "matrix.h"

using namespace std;

#include "raster.h"

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#if defined HAVE_LIBPANDA
#include  <panda/functions.h>
#include  <panda/constants.h>
#endif

#if defined HAVE_LIBMPLOT
#include <libmplot.h>
#endif

#include <wx/gdicmn.h>

#ifndef OBJECTMODEL_H
#define OBJECTMODEL_H

// Forward references
class dictionary;
class object;
class objectlist;
class pdf;

enum
  {
    objNumNoSuch = -1,
    objNumAppended = -2
  };

//typedef struct
//{
//  wxPoint pt;
//  vector<wxPoint> cpt;
//  int modifier;
//} 
//cmdControlPoint;
//
//typedef struct
//{
//  int unique;
//  vector<cmdControlPoint> controlPoints;
//  int liner, lineg, lineb;
//  int fillr, fillg, fillb;
//  unsigned char *rast;
//  matrix ctm;
//
//  string text;
//  string font;
//  int size;
//  matrix textMatrix;
//
  // I don't seem to be able to use object::commandType here, as it gets all
  // circular in it's confusion
//  int type;
//}
//command;

class command
{
 public:
  void executeRaster(plot_state *);
  void executePDF(panda_pdf *);
};

class commandLine: public command
{
  public:

  void executeRaster(plot_state *);
  void executePDF(panda_pdf *);
};

typedef struct
{
  int number;
  int generation;
}
objectreference;

class dictitem
{
public:
  enum diType
  {
    diTypeString = 0,
    diTypeName,
    diTypeArray,
    diTypeObjectReference,
    diTypeDictionary,
    diTypeInt,
    diTypeUnset
  };

  dictitem ();
  dictitem (string name, pdf *thePDF);

  void setValue (string value, bool isName);
  void setValue (int num, int gen);
  void setValue (int integer);
  void setValue (dictionary dict);
  void setValue (objectlist& objs);

  diType getType ();
  string getName ();

  int getIntValue ();
  int getGeneration ();
  string getStringValue ();
  dictionary getDictionaryValue ();
  objectreference getObjectReferenceValue ();

private:
  diType m_type;
  string m_name;
  pdf *m_pdf;

  int m_int;
  int m_generation;
  string m_string;
  vector < int >m_intArray;
  vector < dictitem > m_dictionary;
};

class dictionary
{
public:
  dictionary ();
  dictionary (vector < dictitem > subdict);
  dictionary (object other);

  void add (dictitem item);
  unsigned int size ();
  dictitem operator[] (unsigned int index);

  bool findItem (string dname, dictitem & item);
  bool getValue (string dname, pdf & thePDF, object & obj);
  bool getValue (string dname, string & value);
  bool getValue (string dname, dictionary & subdict);
  bool getValue (string dname, int &value);
  bool getValue (string dname, pdf &thePDF, objectlist & value);

  // TODO mikal: for the object model to be complete, I need the rest
  // of these here...
  bool setValue (string dname, objectlist & value);

  vector < dictitem > getItems ();

private:
  vector < dictitem > m_items;
};

class object
{
public:
  object (int number, int generation);
    object (const object & obj);
   ~object ();

   //   enum commandType{
   //     cLine = 0,
   //     cImage,
   //     cSaveState,
   //     cRestoreState,
   //     cText,
   //     cFont
   //   };

  object operator= (const object & other);

  void addStream (char *stream, unsigned int streamLength);
  void addDictionary (dictionary dict);

  bool hasDictItem (dictitem::diType type, string dname);
  bool hasDictItem (dictitem::diType type, string dname, string dvalue);

  dictionary & getDict ();

  int getNumber ();
  void setNumber(int no);
  int getGeneration ();
  void setGeneration (int no);

  char *getStream (unsigned long &length);
  char *getStream (raster & image, unsigned long &length);
  unsigned long getStreamLength ();

  void appendCommand(command cmd);
  void clearCommands();
  unsigned int getCommandCount();
  command getCommand(unsigned int idx);

  // TODO: There should be one of these to push things into the raster as well
  //  void executeCommand(int index, panda_page *pg);
  //  void rewriteCommand(int index, commandType type, 
  //  vector<cmdControlPoint> controlPoints);
  //  void getCommandLineColor(int index, int& r, int& g, int &b);
  //  void getCommandFillColor(int index, int& r, int& g, int &b);
  //  void getCommandCTM(int index, matrix &ctm);
  //  void getCommandText(int index, string &text, matrix &textMatrix);
  //  void getCommandFontAndSize(int index, string &text, int &size);

  //  vector<cmdControlPoint> getCommandPoints(int index, commandType & type);
  //  int getCommandId(int index);
  //  unsigned char *getCommandRaster(int index);
  //  bool getLastCommand(command& cmd);

  void setHeight(int height);
  void setReadOnly();

private:
  char *applyFilter(string filter, char *instream, unsigned long inlength, 
		    unsigned long& length);
  char *applyFilter(raster& rast, string filter, 
		    char *instream, unsigned long inlength,
		    unsigned long& length);

  int m_number;
  int m_generation;

  char *m_stream;
  unsigned long m_streamLength;
  vector<command> m_commands;
  bool m_changed;
  int m_height;
  bool m_readonly;

  dictionary m_dictionary;
};

class objectlist
{
public:
  objectlist ();
  objectlist (string input, pdf* thePDF);

  bool item (unsigned int i, object &object);
  bool item (unsigned int i, objectreference &objref);

  unsigned int size ();

  void push_back(const objectreference &ref, pdf* thePDF);
  void push_back(const object &obj, pdf* thePDF);
  void push_back(const string objs, pdf* thePDF);

  string getString();

private:
  vector < objectreference > m_objects;
  pdf* m_pdf;
};

class pdf
{
public:
  pdf ();
  pdf (string filename);
  void setSpecVer (float version);
  void addObject (object& theObject);
  void appendPage (object& thePage);

  void appendCommand (objectreference page, command cmd);
  //  void rewriteCommand(objectreference page, int index, 
  //		      object::commandType type, 
  //		      vector<cmdControlPoint> controlPoints);
  void clearCommands(objectreference page);

  bool findObject (int number, int generation, int &inset);
  bool findObject (int number, int generation, object & obj);
  bool findObject (dictitem::diType type, string dname, string dvalue,
		   object & obj);

  vector < object > &getObjects ();
  string getFilename();

  objectlist getPages ();
  bool getCatalogObject(object& obj);
  bool getPagesObject(object& obj);

private:
  string m_filename;
  float m_specVer;
  wxPoint m_previousEnd;

  vector < object > m_objects;
  int m_highestObject;
};

#endif
