
// N = 7: ln(2)の値を小数点以下1000桁求める。
//
//                ∞
//   log(2) = 1/3 Σ 1/(-27)^n ( 3/(6n+1) - 2/(6n+3) - 1/(6n+4) )
//               n=0
//
//     (3/(6n+1) - 2/(6n+3) - 1/(6n+4))
//   = (42 n + 25) / (6 (36 n^3 + 48 n^2 + 19 n + 2))
//

#include <stdio.h>
#include "integer.h"

int main(void) {
    int i;
    int digits = 1000;
    int extraDigits = 10;
    int calcDigits = digits + extraDigits;
    Number tmp, _;
    Number n1, n2, n3;
    Number term, quot;
    Number numer, denom; // numer/denom
    Number one, fortytwo, twentyfive;
    Number thirtysix, fortyeight, nineteen, two, six;
    Number minusTwentyseven;
    Number denom1;
    Number result;
    Number prec;
    digit_t three = 3;
    digit_t rem;
    setInt(&one, 1);
    setInt(&fortytwo, 42);
    setInt(&twentyfive, 25);
    setInt(&thirtysix, 36);
    setInt(&fortyeight, 48);
    setInt(&nineteen, 19);
    setInt(&two, 2);
    setInt(&six, 6);
    setInt(&minusTwentyseven, -27);
    setInt(&denom1, 1);

    clearByZero(&n1);
    clearByZero(&n2);
    clearByZero(&n3);
    clearByZero(&result);

    mulBy10E(calcDigits, &one, &prec); // prec = 1e+digits

    while (1) {
        clearByZero(&numer);
        clearByZero(&denom);

        multiple(&n1, &n1, &n2); // n^2 = n^1 * n^1
        multiple(&n2, &n1, &n3); // n^3 = n^2 * n^1

        // numer = 42 n + 25
        multiple(&fortytwo, &n1, &term);
        add(&numer, &term, &tmp); copyNumber(&tmp, &numer);
        add(&numer, &twentyfive, &tmp); copyNumber(&tmp, &numer);

        // denom = 6 (36 n^3 + 48 n^2 + 19 n + 2)
        multiple(&thirtysix, &n3, &term);
        add(&denom, &term, &tmp); copyNumber(&tmp, &denom);
        multiple(&fortyeight, &n2, &term);
        add(&denom, &term, &tmp); copyNumber(&tmp, &denom);
        multiple(&nineteen, &n1, &term);
        add(&denom, &term, &tmp); copyNumber(&tmp, &denom);
        add(&denom, &two, &tmp); copyNumber(&tmp, &denom);
        multiple(&denom, &six, &tmp); copyNumber(&tmp, &denom);

        multiple(&numer, &prec, &tmp); copyNumber(&tmp, &numer); // numer *= prec

        multiple(&denom, &denom1, &tmp); copyNumber(&tmp, &denom); // denom *= (-27)^n

        if (compNumber(&numer, &denom) < 0) break;
        divmod(&numer, &denom, &quot, &_);
        add(&result, &quot, &tmp); copyNumber(&tmp, &result);

        // next
        increment(&n1, &tmp); copyNumber(&tmp, &n1);
        multiple(&denom1, &minusTwentyseven, &tmp); copyNumber(&tmp, &denom1);
    }

    divmodByInt(&result, &three, &tmp, &rem); copyNumber(&tmp, &result); // result /= 3

    divBy10E(extraDigits, &result, &tmp); copyNumber(&tmp, &result);

    puts("ln(2) = ");
    dispNumberZeroSuppress(&result); putchar('\n');
    return 0;
}
