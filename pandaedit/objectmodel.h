// This file defined the PDF object model in terms of c++

#include <vector>
#include <string>

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

  bool isNamed(string dname);
  diType getType();
  string getName();
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

 private:
  vector<dictitem> m_items;
};

class object{
 public:
  object();
  object(int number, int generation);

  void addStream(char *stream, unsigned int streamLength);
  void addDictionary(dictionary dict);

  bool hasDictItem(string dname, string dvalue);

 private:
  int m_number;
  int m_generation;

  char *m_stream;
  unsigned int m_streamLength;

  dictionary m_dictionary;
};

class pdf{
 public:
  pdf(string filename);
  void setSpecVer(float version);

  void addObject(object theObject);

  bool findObject(string dname, string dvalue, object& obj);

 private:
  string m_filename;
  float m_specVer;

  vector<object> m_objects;
};
