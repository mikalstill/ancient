// Main file for the demo

#include <iostream>
#include "singleton.h"

int main(int argc, char *argv[]){
  cout << "Singleton demonstration demonstration" << endl << endl;

  sing sings[5];
  cout << "Build five of the singleton" << endl;
  for(int i = 0; i < 5; i++){
    sings[i] = sing::grab();
  }

  cout << "Use each of the five to set the value, and use the other four to read it back" << endl;
  for(int i = 0; i < 5; i++){
    cout << "Singleton " << i << " sets value to " << i << endl;
    sings[i].setFoo(i);

    for(int j = 0; j < 5; j++){
      if(i != j){
	cout << "Singleton " << j << " reads back " << sings[j].getFoo() << endl;
      }
    }
  }
}
