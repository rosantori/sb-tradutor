#include "file.h"
#include <iostream>

using std::cout;
using std::endl;

vector<string> checkLine (string line)
{
  vector <string> tokens;
  string token;
  char c;

  for( u_int i = 0; i < line.length(); )
  { 
    token = "";
    do
    {   
      c = line.at(i);
      c = toupper(c);
      
      // Ignore the char if is SPACE, HT, LF, CR or ';'
      if (c != ' ' && c!= 9 && c != 10 && c != 13 && c!= ';')
        token+=c;
      
      // If the char is ';', ignore the rest of the line because it is a comment
      if(c == ';')
      {
        if (tokens.size())
          tokens.push_back(token);
        return tokens;
      }

      i++;
    }
    while (c != ' ' && i < line.length());

    if(token == "")
      continue; 
    if(token == ",")
    {
      cout << "Error! The line \"" << line << "\" contains an invalid token \",\". It is going to be ignored." << std::endl;
      continue;

    }

    tokens.push_back(token);
  }

  return tokens;

}

vector<string> nextLine(ifstream& file)
{
  string line;
  bool flag=false;
  vector<string> tokens;
  do
  {
    getline(file, line);
    tokens = checkLine(line);
    flag = (tokens.size()!= 0); 
    //If it's a label alone in a line, finds the others tokens for it
    if(flag && tokens[0].rfind(":") != string::npos && tokens.size() < 2)
    {   
      string label = tokens[0];
      tokens = nextLine(file);
      tokens.emplace(tokens.begin(), label);
    }  
  }
  while(!flag && !file.eof());

  return tokens;
}

string newLine(vector<string> tokens, bool lb)
{
  string line = "";
  for (u_int i=0; i < tokens.size() - 1 ; i ++)
  {
    line+=tokens[i];
    line+= " ";
  }
  
  line+=tokens.back();

  if(lb)
    line+="\n";

  return line;
}
