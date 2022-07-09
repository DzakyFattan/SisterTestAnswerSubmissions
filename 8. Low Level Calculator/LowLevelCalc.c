#include "LoveLevelCalc.h"

void Input(char *str, int *a)
{
  printf("Masukkan Operand / I (tidak berfungsi) (Masukkan \"Keluar\" untuk keluar): ");
  scanf("%s", str);
  if (strcmp(str, "Keluar") == 0 || strcmp(str, "keluar") == 0)
  {
    printf("Terima kasih telah menggunakan Low Level Calculator.\n");
    exit(0);
  }
  else
  {
    *a = atoi(str);
  }
}

int main()
{
  printf("Low Level Calculator\n");

  printf("Masukan yang diterima adalah integer (4 byte).\n");
  printf("Operasi yang dapat dilakukan: \nPenjumlahan (+), Pengurangan (-), Perkalian (*), Pembagian (/), pangkat (^), 1/sqrt(x) (I)\n");
  printf("Penggunaan: Masukkan Operan terlebih dahulu diikuti enter, kemudian menekan tombol operator yang sesuai diikuti enter pula, \nkemudian masukkan operan selanjutnya, begitu seterusnya hingga diakhiri dengan sama dengan dan enter\n");
  printf("Operator (I) merupakan fungsi, sehingga operator ini tidak dapat dimasukkan tepat setelah operan. \nFungsi ini dapat dimasukkan paling awal ataupun tepat setelah operator lain selain (I)\n");
  
  printf("Catatan: fungsi 1/sqrt(x) belum dapat bekerja.\n");
  printf("Catatan 2: Error-handling minim.\n");

  char str[100];
  int op;   // operator
  int a, b; // operan
  int eq = 1;

  while (1)
  {
    memset(str, 0, 100);
    if (eq)
    {
      Input(str, &a);
      eq = 0;
    }
    printf("Masukkan Operator \n[1] +   [2] -   [3] *   \n[4] /   [5] ^   [6] =\n");
    scanf("%d", &op);
    eq = 0;
    if (op < 1 || op > 6)
    {
      printf("Operator tidak valid\n");
      break;
    }
    else if (op == 6)
    {
      printf("Hasil = %d\n", a);
      eq = 1;
      continue;
    }

    Input(str, &b);

    switch (op)
    {
    case 1:
      Add(&a, b);
      break;
    case 2:
      Sub(&a, b);
      break;
    case 3:
      Mul(&a, b);
      break;
    case 4:
      Div(&a, b);
      break;
    case 5:
      Pow(&a, b);
      break;
    }

    printf("Hasil Sementara = %d\n", a);
  }

  return 0;
}

void Add(int *a, int b)
{
  if (b != 0)
  {
    int tempadd = (*a & b) << 1;
    *a = *a ^ b;
    Add(a, tempadd);
  }
}

void Sub(int *a, int b)
{
  if (b != 0)
  {
    int tempsub = (~(*a) & b) << 1;
    *a = *a ^ b;
    Sub(a, tempsub);
  }
}

void Mul(int *a, int b)
{
  int ans = 0;
MULLOOP:
  if (b > 0)
  {
    if (b & 1)
    {
      Add(&ans, *a);
    }
    *a = *a << 1;
    b = b >> 1;
    goto MULLOOP;
  }
  *a = ans;
}

void Div(int *a, int b)
{
  if (b == 0)
  {
    printf("Error: Division by 0\n");
    exit(0);
  }

  int sign;
  if ((*a < 0) ^ (b < 0))
  {
    sign = 1;
  }
  else
  {
    sign = 0;
  }

  Abs(a);
  Abs(&b);

  int ans = 0;

D1:
  Sub(a, b);
  Add(&ans, 1);
  if (*a >= b)
  {
    goto D1;
  }

  if (sign)
  {
    ans = ~ans;
    Add(&ans, 1);
  }
  *a = ans;
}

void Pow(int *a, int b)
{
  int ans = 1;
  int m;
POWLOOP:
  m = (b & 1);

  if (m)
  {
    Mul(&ans, *a);
  }
  Mul(a, *a);
  b >>= 1;

  if (b > 0)
  {
    goto POWLOOP;
  }

  *a = ans;
}

void Abs(int *a)
{
  if (*a < 0)
  {
    int temp = (*a >> 31) ^ *a;
    Sub(&temp, (*a >> 31));
    *a = temp;
  }
}