
// N = 0: シュテルマーの公式によりπを小数点以下1000桁求める。
//
//   π = 24 arctan(1/8) + 8 arctan(1/57) + 4 arctan(1/239)
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
    Number term1, term2, term3, result;
    Number ten;
    Number c;
    Number arctanResult;
    Number prec;
    clearByZero(&term1);
    clearByZero(&term2);
    clearByZero(&term3);
    clearByZero(&result);
    setInt(&ten, 10);

    puts("24 arctan(1/8)");
    arctanLeft = 24;
    arctanBelow = 8;
    arctan(arctanBelow, calcDigits, &arctanResult);
    setInt(&c, arctanLeft);
    multiple(&arctanResult, &c, &term1);
    add(&result, &term1, &tmp); copyNumber(&tmp, &result);

    puts("8 arctan(1/57)");
    arctanLeft = 8;
    arctanBelow = 57;
    arctan(arctanBelow, calcDigits, &arctanResult);
    setInt(&c, arctanLeft);
    multiple(&arctanResult, &c, &term2);
    add(&result, &term2, &tmp); copyNumber(&tmp, &result);

    puts("4 arctan(1/239)");
    arctanLeft = 4;
    arctanBelow = 239;
    arctan(arctanBelow, calcDigits, &arctanResult);
    setInt(&c, arctanLeft);
    multiple(&arctanResult, &c, &term3);
    add(&result, &term3, &tmp); copyNumber(&tmp, &result);

    divBy10E(extraDigits, &result, &tmp); copyNumber(&tmp, &result);

    puts("pi = ");
    dispNumberZeroSuppress(&result); putchar('\n');

    return 0;
}
