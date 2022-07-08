#include "touhou.hpp"
#include <fstream>
#include <string>
#include <sstream> 

using namespace std;

int main(int argc, char **args)
{
  if (argc < 2)
  {
    cout << "usage : ./2hu <filename>\n";
    exit(0);
  }

  vector<string> lines;
  string line;
  //check file extension
  string filename = args[1];
  if (filename.find(".2hu") == string::npos)
  {
    cout << "Error: file extension must be .2hu\n";
    exit(0);
  }
  
  ifstream in;
  in.open(args[1]);
  if (!in.is_open())
  {
    cout << "Could not open file " << args[1] << endl;
    exit(1);
  }
  while (getline(in, line))
  {
    // split line into words
    string word;
    stringstream ss(line);
    while (getline(ss, word, ' '))
    {
      lines.push_back(word);
      cout << word << endl;
    }
  }
  in.close();

  if (lines.empty())
  {
    cout << "No code found in file " << args[1] << endl;
    exit(1);
  }

  Touhou touhou(lines);
  cout << "lesgo?\n";
  touhou.run();

  return 0;
}