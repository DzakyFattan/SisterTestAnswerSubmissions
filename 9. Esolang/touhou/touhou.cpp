#include "touhou.hpp"

using namespace std;
using SFunc = void (Touhou::*)(void);

Touhou::Touhou(vector<string> lines)
{
  progcode = lines;
  ip = 0;
  ac = 0;
  running = true;
}

Touhou::~Touhou()
{
  progstack.empty();
}

void Touhou::run()
{
  map<string, SFunc> mappedFunc;
  mappedFunc["Yukari"] = &Touhou::halt;
  mappedFunc["Sakuya"] = &Touhou::push;
  mappedFunc["Youmu"] = &Touhou::pop;
  mappedFunc["Reimu"] = &Touhou::print;
  mappedFunc["Marisa"] = &Touhou::scan;
  mappedFunc["Sanae"] = &Touhou::add;
  mappedFunc["Kanako"] = &Touhou::sub;
  mappedFunc["Nitori"] = &Touhou::mul;
  mappedFunc["Remilia"] = &Touhou::div;
  mappedFunc["Yuyuko"] = &Touhou::mod;
  mappedFunc["Aya"] = &Touhou::andf;
  mappedFunc["Momiji"] = &Touhou::notf;
  mappedFunc["Hatate"] = &Touhou::orf;
  mappedFunc["Sukuna"] = &Touhou::shr;
  mappedFunc["Seija"] = &Touhou::shl;
  mappedFunc["Sumireko"] = &Touhou::jmp;
  mappedFunc["Yuuka"] = &Touhou::swap;
  mappedFunc["Koishi"] = &Touhou::pushac;
  mappedFunc["Satori"] = &Touhou::popac;
  mappedFunc["Kokoro"] = &Touhou::je;
  mappedFunc["Mamizou"] = &Touhou::jne;
  mappedFunc["Kasen"] = &Touhou::jl;
  mappedFunc["Suika"] = &Touhou::jg;

  while (this->running)
  {
    try
    {
      stackTrace();
      string func = progcode[ip];
      // cout << func << endl;
      if (mappedFunc.find(func) == mappedFunc.end())
      {
        cout << "Syntax Error on " + func + " at instruction " + to_string(ip);
      }
      (this->*mappedFunc[func])();
      if (func.c_str() != "Sumireko" and func.c_str() != "Kokoro" and func.c_str() != "Mamizou" and func.c_str() != "Kasen" and func.c_str() != "Suika")
      {
        ip++;
      }
    }
    catch (const exception &e)
    {
      cout << "Error: " << e.what() << endl;
      this->halt();
    }
    if (ip >= progcode.size())
    {
      this->halt();
    }
    if (progstack.size() > STACK_SIZE)
    {
      cout << "Stack Overflow" << endl;
      this->halt();
    }
  }
}

void Touhou::stackTrace()
{
  cout << "Printing Stack Trace..." << endl;
  cout << "ip : " << ip << endl;
  cout << "ac : " << ac << endl;
  if (progstack.empty())
  {
    cout << "Stack is empty\n" << endl;
  }
  else
  {
    cout << "Stack : [ ";
    for (stack<int> temp = progstack; !temp.empty(); temp.pop())
    {
      if (temp.size() == 1)
        cout << temp.top() << " ]\n"
             << endl;
      else
        cout << temp.top() << ", ";
    }
  }
}

void Touhou::halt()
{
  this->running = false;
}

void Touhou::push()
{
  string param = progcode[ip + 1];
  int value = stoi(param);
  progstack.push(value);
}

void Touhou::pop()
{
  if (progstack.empty())
  {
    throw runtime_error("Stack is empty");
  }
  progstack.pop();
}

void Touhou::print()
{
  cout << "print" << endl;
  if (progstack.empty())
  {
    throw runtime_error("Stack is empty");
  }
  char c = progstack.top();
  cout << c << endl;
}

void Touhou::scan()
{
  int value;
  cin >> value;
  progstack.push(value);
}

void Touhou::add()
{
  if (progstack.size() < 2)
    throw runtime_error("Stack don't have enough elements");
  int a = progstack.top();
  progstack.pop();
  int b = progstack.top();
  progstack.pop();
  progstack.push(a + b);
}

void Touhou::sub()
{
  if (progstack.size() < 2)
    throw runtime_error("Stack don't have enough elements");
  int a = progstack.top();
  progstack.pop();
  int b = progstack.top();
  progstack.pop();
  progstack.push(b - a);
}

void Touhou::mul()
{
  if (progstack.size() < 2)
    throw runtime_error("Stack don't have enough elements");
  int a = progstack.top();
  progstack.pop();
  int b = progstack.top();
  progstack.pop();
  progstack.push(a * b);
}

void Touhou::div()
{
  if (progstack.size() < 2)
    throw runtime_error("Stack don't have enough elements");
  int a = progstack.top();
  progstack.pop();
  int b = progstack.top();
  progstack.pop();
  progstack.push(b / a);
}

void Touhou::mod()
{
  if (progstack.size() < 2)
    throw runtime_error("Stack don't have enough elements");
  int a = progstack.top();
  progstack.pop();
  int b = progstack.top();
  progstack.pop();
  progstack.push(b % a);
}

void Touhou::andf()
{
  if (progstack.size() < 2)
    throw runtime_error("Stack don't have enough elements");
  int a = progstack.top();
  progstack.pop();
  int b = progstack.top();
  progstack.pop();
  progstack.push(a & b);
}

void Touhou::notf()
{
  if (progstack.size() < 1)
    throw runtime_error("Stack is Empty");
  int a = progstack.top();
  progstack.pop();
  progstack.push(~a);
}

void Touhou::orf()
{
  if (progstack.size() < 2)
    throw runtime_error("Stack don't have enough elements");
  int a = progstack.top();
  progstack.pop();
  int b = progstack.top();
  progstack.pop();
  progstack.push(a | b);
}

void Touhou::shl()
{
  ac <<= 8;
}

void Touhou::shr()
{
  ac >>= 8;
}

void Touhou::jmp()
{
  string param = progcode[ip + 1];
  int value = stoi(param);
  ip = value;
}

void Touhou::swap()
{
  if (progstack.empty())
  {
    throw runtime_error("Stack is empty");
  }
  int temp = progstack.top();
  progstack.pop();
  progstack.push(ac);
  ac = temp;
}

void Touhou::pushac()
{
  progstack.push(ac);
}

void Touhou::popac()
{
  if (progstack.empty())
  {
    throw runtime_error("Stack is empty");
  }
  ac = progstack.top();
  progstack.pop();
}

void Touhou::je()
{
  if ((char)ac == progstack.top())
  {
    jmp();
  }
  else
  {
    ip++;
  }
}

void Touhou::jne()
{
  if ((char)ac != progstack.top())
  {
    jmp();
  }
  else
  {
    ip++;
  }
}

void Touhou::jl()
{
  if (ac < progstack.top())
  {
    jmp();
  }
  else
  {
    ip++;
  }
}

void Touhou::jg()
{
  if (ac > progstack.top())
  {
    jmp();
  }
  else
  {
    ip++;
  }
}