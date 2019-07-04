#ifndef DES_H
#define DES_H

void DES_Efun(const char input[8], char inKey[8], int output[64]);
void DES_Dfun(const int input[64], char inKey[8], char output[8]);

#endif // DES_H