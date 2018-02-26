
// N = 5:
//   √Nの値を小数点以下1000桁求める。
//   Nの値は適当に決めて良いが、符号なし32ビットの上限値（約42億）より大きいとする。
//   ただし平方数（4や9のように整数の2乗として書かれる数）でないこと。

#include <stdio.h>
#include "integer.h"

int main(void) {
    int i;
    int digits = 1000;
    int extraDigits = 20;
    int calcDigits = digits + extraDigits;
    int intPart;
    char numStr[KETA * RADIX_LEN + 2];
    Number num, tmp, result;

    // num = 98765432100
    setInt(&num, 987654321);
    mulBy10E(2, &num, &tmp); copyNumber(&tmp, &num);

    getStr(&num, numStr);
    printf("sqrt( %s ) = \n", numStr);

    intPart = sqrtNumber(&num, calcDigits, &result); // result = sqrt(num)

    divBy10E(extraDigits, &result, &tmp); copyNumber(&tmp, &result);

    dispNumberZeroSuppress(&result); putchar('\n');

    printf("Integer Part = ");
    getStr(&result, numStr);
    for (i = 0; i < intPart; i++) {
        printf("%c", numStr[i]);
    }
    printf("\n");

    return 0;
}
