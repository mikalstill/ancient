#include <vector>
#include <iostream>

class foo{
public:
  foo(int bar);
  foo(const foo& otherfoo);
  ~foo();

  int getNumber();
  void setNumber(int);

private:
  int m_number;
};

foo::foo(int bar): m_number(bar)
{
  cout << "Constructing " << m_number << endl;
}

foo::foo(const foo& otherfoo)
{
  cout << "Copying " << otherfoo.m_number << endl;
  m_number = otherfoo.m_number;
}

foo::~foo()
{
  cout << "Destructor for " << m_number << endl;
}

int foo::getNumber()
{
  return m_number;
}

void foo::setNumber(int bar)
{
  m_number = bar;
}




int main(int argc, char *argv[]){
  cout << "Vector demonstration" << endl << endl;

  vector<foo> myfoos;

  for(int i = 0; i < 20; i++){
    myfoos.push_back(foo(i));
    cout << endl;
  }
}
