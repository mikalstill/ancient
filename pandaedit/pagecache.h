#include <string>
#include "objectmodel.h"

class pageCache{
 public:
  string& operator[](int pageno);

 private:
  vector<string> m_filenames;
};
