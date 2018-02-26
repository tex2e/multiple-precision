
// X = 2: ガウスの公式によりπを小数点以下10000桁求める。
//
//   π = 48 arctan(1/18) + 32 arctan(1/57) - 20 arctan(1/239)
//

#include <stdio.h>
#include "integer.h"

int main(void) {
    int i;
    int digits = 10000;
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

    puts("48 arctan(1/18)");
    arctanLeft = 48;
    arctanBelow = 18;
    arctan(arctanBelow, calcDigits, &arctanResult);
    setInt(&c, arctanLeft);
    multiple(&arctanResult, &c, &term1);
    add(&result, &term1, &tmp); copyNumber(&tmp, &result);

    puts("32 arctan(1/57)");
    arctanLeft = 32;
    arctanBelow = 57;
    arctan(arctanBelow, calcDigits, &arctanResult);
    setInt(&c, arctanLeft);
    multiple(&arctanResult, &c, &term2);
    add(&result, &term2, &tmp); copyNumber(&tmp, &result);

    puts("20 arctan(1/239)");
    arctanLeft = 20;
    arctanBelow = 239;
    arctan(arctanBelow, calcDigits, &arctanResult);
    setInt(&c, arctanLeft);
    multiple(&arctanResult, &c, &term3);
    sub(&result, &term3, &tmp); copyNumber(&tmp, &result);

    divBy10E(extraDigits, &result, &tmp); copyNumber(&tmp, &result);

    puts("pi = ");
    dispNumberZeroSuppress(&result); putchar('\n');

    return 0;
}
