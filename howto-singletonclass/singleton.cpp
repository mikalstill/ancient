// This is an example of a singleton class. It exists only once in the program, despite many
// users each having their own "copy" of the class. This is useful for messaging and the like

#include <iostream>
#include "singleton.h"

sing *sing::m_theSing = NULL;

sing& sing::grab()
{
  if(m_theSing == NULL){
    cout << "First use of the singleton" << endl;
    m_theSing = new sing;
  }
  else{
    cout << "Later use of the singleton" << endl;
  }

  return *m_theSing;
}

int sing::getFoo()
{
  if(m_theSing == NULL){
    cout << "You need to call grab somewhere..." << endl;
    return -1;
  }

  return m_theSing->m_foo;
}

void sing::setFoo(int foo)
{
  if(m_theSing == NULL){
    cout << "You need to call grab somewhere..." << endl;
    return;
  }
  
  m_theSing->m_foo = foo;
}
