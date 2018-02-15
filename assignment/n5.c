
// N = 5:
//   √Nの値を小数点以下1000桁求める。
//   Nの値は適当に決めて良いが、符号なし32ビットの上限値（約42億）より大きいとする。
//   ただし平方数（4や9のように整数の2乗として書かれる数）でないこと。

#include <stdio.h>
#include "integer.h"

int main(void) {
    Number num, tmp, result;

    // num = 49012345 * 10^1000
    setInt(&num, 49012345);
    mulBy10E(1000, &num, &tmp); copyNumber(&tmp, &num);

    puts("sqrt(");
    dispNumberZeroSuppress(&num); putchar('\n');
    puts(")\n=");

    sqrtNumber(&num, &result); // result = sqrt(num)

    dispNumberZeroSuppress(&result); putchar('\n');

    return 0;
}
