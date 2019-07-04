#include "des.h"

#include <cstdio>

int main()
{
    int output[64] = {0};
    char A[9] = {0};
    char B[9] = {0};

    printf("请输入明文(8字节)\n");
    scanf("%s", A);
    printf("请输入密匙(8字节)\n");
    scanf("%s", B);

    DES_Efun(A, B, output);

    putchar(10);
    printf("对明文进行加密\n");
    printf("密文:\n");
    for (int i = 0; i < 64; i++)
    {
        printf("%d", output[i]);
        if ((i + 1) % 8 == 0)
        {
            putchar(10);
        }
        else if ((i + 1) % 4 == 0)
        {
            putchar(' ');
        }
    }

    putchar(10);
    printf("对密文进行解密\n");

    DES_Dfun(output, B, A);

    printf("明文:\n");
    for (int i = 0; i < 8; i++)
    {
        printf("%c", A[i]);
    }
    putchar(10);

    return 0;
}