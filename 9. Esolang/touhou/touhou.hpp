#ifndef TOUHOU_HPP
#define TOUHOU_HPP

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <stack>
using namespace std;

#define STACK_SIZE 512

class Touhou
{
private:
  stack<int> progstack;    // the stack
  vector<string> progcode; // the compiled code
  
  int ip; // instruction pointer
  int ac; // accumulator

  bool running;

public:
  Touhou(vector<string> lines);
  ~Touhou();

  void run();

  void stackTrace();

  void halt();
  void push();
  void pop();
  void print();
  void scan();
  void add();
  void sub();
  void mul();
  void div();
  void mod();
  void andf();
  void notf();
  void orf();
  void shr();
  void shl();
  void jmp();
  void swap();
  void pushac();
  void popac();
  void je();
  void jne();
  void jl();
  void jg();
};

#endif