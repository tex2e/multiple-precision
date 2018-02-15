
// N = 1: マチンの公式によりπを小数点以下1000桁求める。
//
//   π = 16 arctan(1/5) - 4 arctan(1/239)
//

#include <stdio.h>
#include "integer.h"

int main(void) {
    int i;
    int digits = 1000;
    int extraDigits = 100;
    int calcDigits = digits + extraDigits;
    int arctanLeft, arctanBelow;
    Number tmp;
    Number term1, term2, result;
    Number ten;
    Number c;
    Number arctanResult;
    Number prec;
    clearByZero(&term1);
    clearByZero(&term2);
    clearByZero(&result);
    setInt(&ten, 10);

    puts("16 arctan(1/5)");
    arctanLeft = 16;
    arctanBelow = 5;
    arctan(arctanBelow, calcDigits, &arctanResult);
    setInt(&c, arctanLeft);
    multiple(&arctanResult, &c, &term1);
    add(&result, &term1, &tmp); copyNumber(&tmp, &result);

    puts("4 arctan(1/239)");
    arctanLeft = 4;
    arctanBelow = 239;
    arctan(arctanBelow, calcDigits, &arctanResult);
    setInt(&c, arctanLeft);
    multiple(&arctanResult, &c, &term2);
    sub(&result, &term2, &tmp); copyNumber(&tmp, &result);

    divBy10E(extraDigits, &result, &tmp); copyNumber(&tmp, &result);

    puts("pi = ");
    dispNumberZeroSuppress(&result); putchar('\n');

    return 0;
}
