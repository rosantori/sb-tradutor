#include "translator.h"
#include "file.h"

#include <fstream>
#include <iostream>
#include <vector>

using std::ofstream;
using std::cout;
using std::endl;
using std::vector;

bool translator(string name)
{
  ifstream file;
  ofstream transFile;

  file.open((name+".pre").c_str());
  if(!file.is_open()) 
  {
    cout << "Error! CANNOT OPEN " << name+".pre" << endl;
    return false;
  }

  vector<string> tokens;

  transFile.open(name+".s");
  if(!transFile.is_open())
  {
    cout << "Error! CANNOT CREATE " << name+".s" <<endl;
    return false;
  }

  transFile << "global _start\n";

  tokens = nextLine(file);
  bool SECTION;
  if (!(tokens[1].compare("DATA"))) SECTION = DATA;
  else {
    SECTION = TEXT;
    transFile << "section .text\n_start:\n";
  }
  
  string label = "";

  while(!file.eof() && !file.bad())
  {   
    label="";
    
    tokens = nextLine(file);
    if(file.eof() || file.bad())
      break;

    for(u_int i=0; i<tokens.size(); i++)
    {
      if(tokens[i].rfind(',') != string::npos )
        tokens[i] = tokens[i].substr(0, tokens[i].find(','));
    }

    if(tokens[0].rfind(':') != string::npos) {
      label = tokens[0];
      tokens.erase(tokens.begin());
    }
    if(!tokens[0].compare("SECTION"))
    {
      transFile << "\n";
      if( SECTION == DATA) {
        SECTION = TEXT;
        transFile << "section .text\n_start:\n";
      }
      else { 
        transFile << "\n"+_LeerInteiro;
        transFile << "\n"+_EscreverInteiro;
        transFile << "\n"+_LeerChar;
        transFile << "\n"+_EscreverChar;
        transFile << "\n"+_LeerString;
        transFile << "\n"+_EscreverString;
        transFile << "\n"+_overflow;
        transFile << "\n"+_stop;
        transFile << "\n"+_dataBss;
  
        SECTION = DATA;
        }

      continue;
    }

    if(SECTION == TEXT)
    {
      #if DEBUG
      if (label != "") {
        cout << "Contém uma label em: "+label+ newLine(tokens)+"\n";
      } else cout << "Não contém label em : "+newLine(tokens)+"\n";
      #endif

      if(tokens[0] == "ADD") 
      {
        #if DEBUG 
          transFile << ";"+newLine(tokens, true);
        #endif

        if(label != "") transFile << label+" ";

        transFile << "add EAX, dword["+tokens[1];
        if(tokens.size() > 2 && tokens[2] == "+")
        {
          const int offset = 4* stoi(tokens[3]);
          transFile << " + " + std::to_string(offset);
        }
        transFile << "]\n\n";
      }
      else if(tokens[0] == "SUB") 
      {
        #if DEBUG 
          transFile << ";"+newLine(tokens, true);
        #endif

        if(label != "") transFile << label+" ";

        transFile << "sub EAX, dword["+tokens[1];
        if(tokens.size() > 2 && tokens[2] == "+")
        {
          const int offset = 4* stoi(tokens[3]);
          transFile << " + " + std::to_string(offset);
        }
        transFile << "]\n\n";

      }
      else if(tokens[0] == "MULT") 
      {
        #if DEBUG 
          transFile << ";"+newLine(tokens, true);
        #endif

        if(label != "") transFile << label+" ";

        transFile << "imul dword["+tokens[1];
        if(tokens.size() > 2 && tokens[2] == "+")
        {
          const int offset = 4* stoi(tokens[3]);
          transFile << " + " + std::to_string(offset);
        }
        transFile << "]\njo _overflow\n\n";


      }
      else if(tokens[0] == "DIV") 
      {
        #if DEBUG 
          transFile << ";"+newLine(tokens, true);
        #endif

        if(label != "") transFile << label+" ";

        transFile << "cdq\nidiv dword ["+tokens[1];
        if(tokens.size() > 2 && tokens[2] == "+")
        {
          const int offset = 4* stoi(tokens[3]);
          transFile << " + " + std::to_string(offset);
        }
        transFile << "]\n\n";
      }
      else if(tokens[0] == "JMP") 
      {
        #if DEBUG 
          transFile << ";"+newLine(tokens, true);
        #endif

        if(label != "") transFile << label+" ";

        transFile << "jmp "+tokens[1];
        if(tokens.size() > 2 && tokens[2] == "+")
        {
          const int offset = 4* stoi(tokens[3]);
          transFile << " + " + std::to_string(offset);
        }
        transFile << "\n\n";

      }
      else if(tokens[0] == "JMPN") 
      {
        #if DEBUG 
          transFile << ";"+newLine(tokens, true);
        #endif

        if(label != "") transFile << label+" ";

        transFile << "cmp EAX,  0\njl "+tokens[1];
        if(tokens.size() > 2 && tokens[2] == "+")
        {
          const int offset = 4* stoi(tokens[3]);
          transFile << " + " + std::to_string(offset);
        }
        transFile << "\n\n";

      }
      else if(tokens[0] == "JMPP") 
      {
        #if DEBUG 
          transFile << ";"+newLine(tokens, true);
        #endif

        if(label != "") transFile << label+" ";

        transFile << "cmp EAX,  0\njg "+tokens[1];
        if(tokens.size() > 2 && tokens[2] == "+")
        {
          const int offset = 4* stoi(tokens[3]);
          transFile << " + " + std::to_string(offset);
        }
        transFile << "\n\n";

      }
      else if(tokens[0] == "JMPZ") 
      {
        #if DEBUG 
          transFile << ";"+newLine(tokens, true);
        #endif

        if(label != "") transFile << label+" ";

        transFile << "cmp EAX,  0\nje "+tokens[1];
        if(tokens.size() > 2 && tokens[2] == "+")
        {
          const int offset = 4* stoi(tokens[3]);
          transFile << " + " + std::to_string(offset);
        }
        transFile << "\n\n";

      }
      else if(tokens[0] == "COPY") 
      {
        #if DEBUG 
          transFile << ";"+newLine(tokens, true);
        #endif

        if(label != "") transFile << label+" ";

        transFile << "mov EBX,  dword ["+tokens[1];
        if(tokens.size() > 2 && tokens[2] == "+")
        {
          const int offset = 4* stoi(tokens[3]);
          transFile << " + " + std::to_string(offset);
          tokens.erase(tokens.begin()+2, tokens.begin()+4);
        }
        transFile << "]\nmov dword ["+tokens[2];
        if(tokens.size() > 3 && tokens[3] == "+")
        {
          const int offset = 4* stoi(tokens[4]);
          transFile << " + " + std::to_string(offset);
        }
        transFile << "], EBX\n\n";

      }
      else if(tokens[0] == "LOAD") 
      {
        #if DEBUG 
          transFile << ";"+newLine(tokens, true);
        #endif

        if(label != "") transFile << label+" ";

        transFile << "mov EAX, dword ["+tokens[1];
        if(tokens.size() > 2 && tokens[2] == "+")
        {
          const int offset = 4* stoi(tokens[3]);
          transFile << " + " + std::to_string(offset);
        }
        transFile << "]\n\n";

      }
      else if(tokens[0] == "STORE") 
      {
        #if DEBUG 
          transFile << ";"+newLine(tokens, true);
        #endif

        if(label != "") transFile << label+" ";

        transFile << "mov dword ["+tokens[1];
        if(tokens.size() > 2 && tokens[2] == "+")
        {
          const int offset = 4* stoi(tokens[3]);
          transFile << " + " + std::to_string(offset);
        }
        transFile << "], EAX\n\n";
      }
      else if(tokens[0] == "INPUT") 
      {
        #if DEBUG 
          transFile << ";"+newLine(tokens, true);
        #endif

        if(label != "") transFile << label+" ";
        transFile << "push dword ESI\n";
        transFile << "push dword EAX\n";
        transFile << "push dword EBX\n";
        transFile << "push dword ECX\n";
        transFile << "push dword EDX\n";
        transFile << "push dword "+tokens[1];
        if(tokens.size() > 2 && tokens[2] == "+")
        {
          const int offset = 4* stoi(tokens[3]);
          transFile << " + " + std::to_string(offset);
        }
        transFile << "\n";
        transFile << "call LeerInteiro\n";
        transFile << "push dword EAX\n";
        transFile << "mov EAX, 4\nmov EBX, 1\nmov ECX, _msgReading1_\nmov EDX, _sizeMsgReading1_\nint 80h\n";
        transFile << "call EscreverInteiro\n";
        transFile << "mov EAX, 4\nmov EBX, 1\nmov ECX, _msgReading2_\nmov EDX, _sizeMsgReading2_\nint 80h\n";
        transFile << "pop dword EDX\n";
        transFile << "pop dword ECX\n";
        transFile << "pop dword EBX\n";
        transFile << "pop dword EAX\n";
        transFile << "pop dword ESI\n\n";


      }
      else if(tokens[0] == "OUTPUT") 
      {
        #if DEBUG 
          transFile << ";"+newLine(tokens, true);
        #endif

        if(label != "") transFile << label+" ";
        transFile << "push dword ESI\n";
        transFile << "push dword EAX\n";
        transFile << "push dword EBX\n";
        transFile << "push dword ECX\n";
        transFile << "push dword EDX\n";
        transFile << "mov EAX, dword ["+tokens[1];
        if(tokens.size() > 2 && tokens[2] == "+")
        {
          const int offset = 4* stoi(tokens[3]);
          transFile << " + " + std::to_string(offset);
        }
        transFile << "]\n";
        transFile << "push EAX\n";
        transFile << "call EscreverInteiro\n";
        transFile << "pop dword EDX\n";
        transFile << "pop dword ECX\n";
        transFile << "pop dword EBX\n";
        transFile << "pop dword EAX\n";
        transFile << "pop dword ESI\n\n";


      }
      else if(tokens[0] == "C_INPUT") 
      {
        #if DEBUG 
          transFile << ";"+newLine(tokens, true);
        #endif

        if(label != "") transFile << label+" ";
        transFile << "push dword ESI\n";
        transFile << "push dword EAX\n";
        transFile << "push dword EBX\n";
        transFile << "push dword ECX\n";
        transFile << "push dword EDX\n";
        transFile << "push dword dword "+tokens[1];
        if(tokens.size() > 2 && tokens[2] == "+")
        {
          const int offset = 4* stoi(tokens[3]);
          transFile << " + " + std::to_string(offset);
        }
        transFile << "\n";
        transFile << "call LeerChar\n";
        transFile << "push dword EAX\n";
        transFile << "mov EAX, 4\nmov EBX, 1\nmov ECX, _msgReading1_\nmov EDX, _sizeMsgReading1_\nint 80h\n";
        transFile << "call EscreverInteiro\n";
        transFile << "mov EAX, 4\nmov EBX, 1\nmov ECX, _msgReading2_\nmov EDX, _sizeMsgReading2_\nint 80h\n";
        transFile << "pop dword EDX\n";
        transFile << "pop dword ECX\n";
        transFile << "pop dword EBX\n";
        transFile << "pop dword EAX\n";
        transFile << "pop dword ESI\n\n";

      }
      else if(tokens[0] == "C_OUTPUT") 
      {
        #if DEBUG 
          transFile << ";"+newLine(tokens, true);
        #endif

        if(label != "") transFile << label+" ";
        transFile << "push dword ESI\n";
        transFile << "push dword EAX\n";
        transFile << "push dword EBX\n";
        transFile << "push dword ECX\n";
        transFile << "push dword EDX\n";
        transFile << "push dword "+tokens[1];
        if(tokens.size() > 2 && tokens[2] == "+")
        {
          const int offset = 4* stoi(tokens[3]);
          transFile << " + " + std::to_string(offset);
        }
        transFile << "\n";
        transFile << "call EscreverChar\n";
        transFile << "pop dword EDX\n";
        transFile << "pop dword ECX\n";
        transFile << "pop dword EBX\n";
        transFile << "pop dword EAX\n";
        transFile << "pop dword ESI\n\n";



      }
      else if(tokens[0] == "S_INPUT") 
      {
        #if DEBUG 
          transFile << ";"+newLine(tokens, true);
        #endif

        if(label != "") transFile << label+" ";
        transFile << "push dword ESI\n";
        transFile << "push dword EAX\n";
        transFile << "push dword EBX\n";
        transFile << "push dword ECX\n";
        transFile << "push dword EDX\n";
        transFile << "push dword "+tokens[1];
        if(tokens.size() > 2 && tokens[2] == "+")
        {
          const int offset = 4* stoi(tokens[3]);
          tokens.erase(tokens.begin()+2, tokens.begin()+2);
          transFile << " + " + std::to_string(offset);
        }
        transFile << "\npush dword "+tokens[2]+"\n";
        transFile << "call LeerString\n";
        transFile << "push dword EAX\n";
        transFile << "mov EAX, 4\nmov EBX, 1\nmov ECX, _msgReading1_\nmov EDX, _sizeMsgReading1_\nint 80h\n";
        transFile << "call EscreverInteiro\n";
        transFile << "mov EAX, 4\nmov EBX, 1\nmov ECX, _msgReading2_\nmov EDX, _sizeMsgReading2_\nint 80h\n";
        transFile << "pop dword EDX\n";
        transFile << "pop dword ECX\n";
        transFile << "pop dword EBX\n";
        transFile << "pop dword EAX\n";
        transFile << "pop dword ESI\n\n";
      }
      else if(tokens[0] == "S_OUTPUT") 
      {
        #if DEBUG 
          transFile << ";"+newLine(tokens, true);
        #endif

        if(label != "") transFile << label+" ";
        transFile << "push dword ESI\n";
        transFile << "push dword EAX\n";
        transFile << "push dword EBX\n";
        transFile << "push dword ECX\n";
        transFile << "push dword EDX\n";
        transFile << "push dword "+tokens[1];
        if(tokens.size() > 2 && tokens[2] == "+")
        {
          const int offset = 4* stoi(tokens[3]);
          tokens.erase(tokens.begin()+2, tokens.begin()+2);
          transFile << " + " + std::to_string(offset);
        }
        transFile << "\npush dword "+tokens[2]+"\n";
        transFile << "call EscreverString\n";
        transFile << "pop dword EDX\n";
        transFile << "pop dword ECX\n";
        transFile << "pop dword EBX\n";
        transFile << "pop dword EAX\n";
        transFile << "pop dword ESI\n\n";

      }
      else if(tokens[0] == "STOP") 
      {
        #if DEBUG 
          transFile << ";"+newLine(tokens, true);
        #endif

        if(label != "") transFile << label+" ";

        transFile << "mov EAX, 1\nmov EBX, 0\nint 80h\n";
      }
      else 
      { 
        cout << "ERROR! "+tokens[0]+" is not a function!\n";
        break;
      }

    } 
    else if(SECTION == DATA)
    {
      static bool sectionBSS = false;
      static bool sectionDATA = false;
      if (tokens[0] == "CONST") {
        if(!sectionDATA)
        {
          #if debug
          cout << ".data\n";
          #endif
          transFile << "section .data\n";
          sectionBSS = false;
          sectionDATA = true;
        }

        transFile << label+" dd ";
        if (tokens.size() > 1) transFile << tokens[1]+"\n\n";
        else {
          cout << "ERROR\n";
          return false;
        }

      } else if (tokens[0] == "SPACE"){
        if(!sectionBSS)
        {
          #if debug
          cout << ".bss\n";
          #endif

          transFile << "section .bss\n";
          sectionBSS = true;
          sectionDATA = false;
        }
        // label: resd tamanho
        transFile << label+" resd ";
        if (tokens.size() > 1) transFile << tokens[1]+"\n";
        else transFile << "1\n\n";
      } else cout << "Something wrong with section data \n";

      #if debug
      if(label != "") cout << label+" "+tokens[0]+"\n";
      else cout << "Erro\n";
      #endif
    }
  }

  if(SECTION == TEXT) {
    transFile << "\n"+_LeerInteiro;
    transFile << "\n"+_EscreverInteiro;
    transFile << "\n"+_LeerChar;
    transFile << "\n"+_EscreverChar;
    transFile << "\n"+_LeerString;
    transFile << "\n"+_EscreverString;
    transFile << "\n"+_overflow;
    transFile << "\n"+_stop;
    transFile << "\n"+_dataBss;
  }

  #if DEBUG
  cout << "Second step completed." << endl;
  #endif

  transFile.close();
  file.close();
  return true;
}