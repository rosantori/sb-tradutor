#include <iostream>
#include <cstring>
#include "pre.h"
#include "translator.h"

using std::cout;
using std::endl;
using std::to_string;

int main(int argc, char *argv[]) 
{
  //Check if terminal entry is  OK
  if (argc != 2)
  {
    cout << "ENTRY MUST BE \'./tradutor fileName.asm\'\n";
    return 1;
  }

  string fileName(argv[1]);

  string name = fileName.substr(0, fileName.size()-4);

  if(pre(fileName, name)) {
    #if DEBUG
      cout << "First stage completed.\n";
    #endif

    if (translator(name)) cout << "File \'"+name+".s\' created successfully.\n";
    else cout << "Failed to translate file.\n";
    
     #if !DEBUG
      int failed = remove((name+".pre").c_str());
      if (failed) cout << "Failed to delete "+fileName+".pre.\nError: "+to_string(failed)+".\n";
      else cout << "Success.";
    #endif
  } 
  else 
  {
    cout << "Failed to translate file.\n";
  }

  return 0;
}

