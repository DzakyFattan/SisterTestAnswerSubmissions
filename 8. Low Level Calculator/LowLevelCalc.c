#include <stdio.h>

int main()
{
    printf("Low Level Calculator\n");
    printf("Operasi yang dapat dilakukan: \nPenjumlahan (+), Pengurangan (-), Perkalian (*), Pembagian (/), pangkat (^), 1/sqrt(x) (I)\n");
    printf("Penggunaan: Tuliskan ekspresi yang akan dilakukan, antara operator dan operan TIDAK dipisahkan spasi, diakhiri oleh sama dengan (=). maksimal 10 operan.\n");
    printf("Misal: '3*7+5-2/4+I(1)=' (Hasil = 4, ekspresi dievaluasi kiri ke kanan)\n");

    char operasi[50];
    double a, b;
    char op;
    int operan_pos;
    int op_pos;

MAINLOOP:
    int i = 0;
    CLEARSTR:
    operasi[i] = '\0';
    i++;
    if (i < 50)
    {
        goto CLEARSTR;
    }
    printf("%s", operasi);

    operan_pos = 0;
    op_pos = 1;
    a = 8;

    printf(">>> ");
    scanf("%s", operasi);

    if (operasi[0] == '=')
    {
        printf("Tidak ada operasi yang dilakukan\n");
        goto MAINLOOP;
    }
    else if (operasi[0] == '+' || operasi[0] == '-' || operasi[0] == '*' || operasi[0] == '/')
    {
        printf("Tidak ada operan\n");
        goto MAINLOOP;
    }
    else
    {
        OPERASI:
        
        op = operasi[op_pos];

        if (op = '=') {
            printf("%d \n", a);
            goto MAINLOOP;
        }
        else if (op != '+' && op != '-' && op != '*' && op != '/' && op != '^' && op != 'I' && op != '=')
        {
            printf("Terjadi Kesalahan\n");
            goto MAINLOOP;
        }
        else
        {
            if (op == '+')
            {
                operan_pos += 2;
                b = operasi[operan_pos] - '0';
                a = a + b;
                goto OPERASI;
            }
            else if (op == '-')
            {
                printf("%d - ", a);
            }
            else if (op == '*')
            {
                printf("%d * ", a);
            }
            else if (op == '/')
            {
                printf("%d / ", a);
            }
            else if (op == '^')
            {
                printf("%d ^ ", a);
            }
            else if (op == 'I')
            {
                printf("1/%d ", a);
            }
        }
    }
}
