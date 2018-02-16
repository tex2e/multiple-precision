
// N = 9: 次のAdamchik, Wagonの公式その1によりπを小数点以下1000桁求める。
//        ∞
//   π =  Σ (-1)^n/4^n ( 2/(4n+1) + 2/(4n+2) + 1/(4n+3) )
//       n=0
//
//    2/(4n+1) + 2/(4n+2) + 1/(4n+3)
//  = (2 (20 n^2 + 21 n + 5)) / ((2 n + 1) (4 n + 1) (4 n + 3))
//  = (2 (20 n^2 + 21 n + 5)) / (32 n^3 + 48 n^2 + 22 n + 3)
//

#include <stdio.h>
#include "integer.h"

int main(void) {
    int sign = 1;
    int digits = 1000;
    int extraDigits = 10;
    int calcDigits = digits + extraDigits;
    Number tmp, _;
    Number n1, n2, n3, num4n;
    Number term, quot;
    Number numer, denom; // numer/denom
    Number one, four, two, twenty, twentyone, five;
    Number thirtytwo, fortyeight, twentytwo, three;
    Number result;
    Number prec;
    setInt(&one, 1);
    setInt(&four, 4);
    setInt(&two, 2);
    setInt(&twenty, 20);
    setInt(&twentyone, 21);
    setInt(&five, 5);
    setInt(&thirtytwo, 32);
    setInt(&fortyeight, 48);
    setInt(&twentytwo, 22);
    setInt(&three, 3);

    clearByZero(&n1);
    clearByZero(&n2);
    clearByZero(&n3);
    setInt(&num4n, 1);
    clearByZero(&result);

    mulBy10E(calcDigits, &one, &prec); // prec = 1e+digits

    while (1) {
        clearByZero(&numer);
        clearByZero(&denom);

        multiple(&n1, &n1, &n2); // n^2 = n^1 * n^1
        multiple(&n2, &n1, &n3); // n^3 = n^2 * n^1

        // numer = 2 (20 n^2 + 21 n + 5)
        multiple(&twenty, &n2, &term);
        add(&numer, &term, &tmp); copyNumber(&tmp, &numer);
        multiple(&twentyone, &n1, &term);
        add(&numer, &term, &tmp); copyNumber(&tmp, &numer);
        add(&numer, &five, &tmp); copyNumber(&tmp, &numer);
        multiple(&numer, &two, &tmp); copyNumber(&tmp, &numer);

        // denom = 32 n^3 + 48 n^2 + 22 n + 3
        multiple(&thirtytwo, &n3, &term);
        add(&denom, &term, &tmp); copyNumber(&tmp, &denom);
        multiple(&fortyeight, &n2, &term);
        add(&denom, &term, &tmp); copyNumber(&tmp, &denom);
        multiple(&twentytwo, &n1, &term);
        add(&denom, &term, &tmp); copyNumber(&tmp, &denom);
        add(&denom, &three, &tmp); copyNumber(&tmp, &denom);

        multiple(&numer, &prec, &tmp); copyNumber(&tmp, &numer); // numer *= prec

        multiple(&denom, &num4n, &tmp); copyNumber(&tmp, &denom); // denom *= 4^n

        if (compNumber(&numer, &denom) < 0) break;
        divmod(&numer, &denom, &quot, &_);
        setSign(&quot, sign);
        add(&result, &quot, &tmp); copyNumber(&tmp, &result);

        // next
        sign *= -1;
        increment(&n1, &tmp); copyNumber(&tmp, &n1);
        multiple(&num4n, &four, &tmp); copyNumber(&tmp, &num4n);
    }

    divBy10E(extraDigits, &result, &tmp); copyNumber(&tmp, &result);

    puts("pi = ");
    dispNumberZeroSuppress(&result); putchar('\n');
    return 0;
}
