#include <iostream>
#include <windows.h>
#include <stdio.h>
#include <tchar.h>
#include <psapi.h>

void print_ps_name(DWORD pid)
{
  TCHAR pname[MAX_PATH] = TEXT("<unknown>");
  HANDLE phandle = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, pid);
  if (phandle != NULL)
  {
    HMODULE pmod;
    DWORD temp;
    if (EnumProcessModules(phandle, &pmod, sizeof(pmod), &temp))
      GetModuleBaseName(phandle, pmod, pname, sizeof(pname) / sizeof(TCHAR));
  }
  _tprintf(TEXT("%s (PID %u)\n"), pname, pid);
  CloseHandle(phandle);
}

void print_ps()
{
  DWORD ps_list[1024], ps_memctr, ps_ctr;
  if (!EnumProcesses(ps_list, sizeof(ps_list), &ps_memctr))
    return;
  ps_ctr = ps_memctr / sizeof(DWORD);
  for (int i = 0; i < ps_ctr; i++)
    if (ps_list[i] != 0)
      print_ps_name(ps_list[i]);
}

int main()
{
  print_ps();
  // TODO : Add memory editor
  return 0;
}