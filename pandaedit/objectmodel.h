// This file defined the PDF object model in terms of c++

#include <vector>
#include <string>

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#ifndef OBJECTMODEL_H
#define OBJECTMODEL_H

class dictionary;
class object;
class pdf;

class dictitem{
 public:
  enum diType{
    diTypeString = 0,
      diTypeName,
      diTypeArray,
      diTypeObjectReference,
      diTypeDictionary,
      diTypeInt
      };

  dictitem();
  dictitem(diType type, string name);

  void setValue(string value);
  void setValue(int num, int gen);
  void setValue(int integer);
  void setValue(dictionary dict);

  diType getType();
  string getName();

  int getIntValue();
  int getGeneration();
  string getStringValue();

 private:
  diType m_type;
  string m_name;

  int m_int;
  int m_generation;
  string m_string;
  vector<int> m_intArray;
  vector<dictitem> m_dictionary;
};

class dictionary{
 public:
  void add(dictitem item);
  unsigned int size();
  dictitem operator[](unsigned int index);

  bool findItem(string dname, dictitem& item);
  bool getValue(string dname, pdf& thePDF, object& obj);
  bool getValue(string dname, string& value);

  vector<dictitem> getItems();

 private:
  vector<dictitem> m_items;
};

class object{
 public:
  object(int number, int generation);
  object(const object& obj);
  ~object();

  object operator=(const object& other);

  void addStream(char *stream, unsigned int streamLength);
  void addDictionary(dictionary dict);

  bool hasDictItem(dictitem::diType type, string dname);
  bool hasDictItem(dictitem::diType type, string dname, string dvalue);

  dictionary& getDict();
  int getNumber();
  int getGeneration();
  char *getStream();
  unsigned int getStreamLength();

 private:
  int m_number;
  int m_generation;

  char *m_stream;
  unsigned int m_streamLength;

  dictionary m_dictionary;
};

typedef struct{
  int number;
  int generation;
} objectreference;

class objectlist
{
 public:
  objectlist(string input, pdf& thePDF);
  object operator[](unsigned int i);
  unsigned int size();

 private:
  vector<objectreference> m_objects;
  pdf& m_pdf;
};

class pdf{
 public:
  pdf(string filename);
  void setSpecVer(float version);

  void addObject(object theObject);

  bool findObject(dictitem::diType type, string dname, string dvalue, object& obj);
  bool findObject(int number, int generation, object& obj);

  vector<object>& getObjects();

 private:
  string m_filename;
  float m_specVer;

  vector<object> m_objects;
};

#endif
