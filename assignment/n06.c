
// N = 6: 次のBailey, Borwein, Plouffeの公式によりπを小数点以下1000桁求める。
//
//        ∞
//   π =  Σ 1/16^n ( 4/(8n+1) - 2/(8n+4) - 1/(8n+5) - 1/(8n+6) )
//       n=0
//
//    4/(8n+1) - 2/(8n+4) - 1/(8n+5) - 1/(8n+6)
//  = (120 n^2 + 151 n + 47) / ((2 n + 1) (4 n + 3) (8 n + 1) (8 n + 5))
//  = (120 n^2 + 151 n + 47) / (512 n^4 + 1024 n^3 + 712 n^2 + 194 n + 15)
//  = (n (120 n + 151) + 47) / (n (n (n (512 n + 1024) + 712) + 194) + 15)

#include <stdio.h>
#include "integer.h"

int main(void) {
    int digits = 1000;
    int extraDigits = 10;
    int calcDigits = digits + extraDigits;
    Number tmp, _;
    Number n, num16n;
    Number term, quot;
    Number numer, denom; // numer/denom
    Number one, sixteen, onetwenty, onefiftyone, fortyseven;
    Number fivetwelve, tentwentyfour, seventwelve, oneninetyfour, fifteen;
    Number result;
    Number prec;
    setInt(&one, 1);
    setInt(&sixteen, 16);
    setInt(&onetwenty, 120);
    setInt(&onefiftyone, 151);
    setInt(&fortyseven, 47);
    setInt(&fivetwelve, 512);
    setInt(&tentwentyfour, 1024);
    setInt(&seventwelve, 712);
    setInt(&oneninetyfour, 194);
    setInt(&fifteen, 15);

    clearByZero(&n);
    clearByZero(&result);

    mulBy10E(calcDigits, &one, &prec); // prec = 1e+digits

    while (1) {
        clearByZero(&numer);
        clearByZero(&denom);

        // numer = (n (120 n + 151) + 47)
        copyNumber(&onetwenty, &numer);
        multiple(&n, &numer, &tmp); copyNumber(&tmp, &numer);
        add(&numer, &onefiftyone, &tmp); copyNumber(&tmp, &numer);
        multiple(&n, &numer, &tmp); copyNumber(&tmp, &numer);
        add(&numer, &fortyseven, &tmp); copyNumber(&tmp, &numer);

        // denom = (n (n (n (512 n + 1024) + 712) + 194) + 15)
        copyNumber(&fivetwelve, &denom);
        multiple(&n, &denom, &tmp); copyNumber(&tmp, &denom);
        add(&denom, &tentwentyfour, &tmp); copyNumber(&tmp, &denom);
        multiple(&n, &denom, &tmp); copyNumber(&tmp, &denom);
        add(&denom, &seventwelve, &tmp); copyNumber(&tmp, &denom);
        multiple(&n, &denom, &tmp); copyNumber(&tmp, &denom);
        add(&denom, &oneninetyfour, &tmp); copyNumber(&tmp, &denom);
        multiple(&n, &denom, &tmp); copyNumber(&tmp, &denom);
        add(&denom, &fifteen, &tmp); copyNumber(&tmp, &denom);

        multiple(&numer, &prec, &tmp); copyNumber(&tmp, &numer); // numer *= prec

        power(&sixteen, &n, &num16n);
        multiple(&denom, &num16n, &tmp); copyNumber(&tmp, &denom); // denom *= 16^n

        if (compNumber(&numer, &denom) < 0) break;
        divmod(&numer, &denom, &quot, &_);
        add(&result, &quot, &tmp); copyNumber(&tmp, &result);

        // next
        increment(&n, &tmp); copyNumber(&tmp, &n);
    }

    divBy10E(extraDigits, &result, &tmp); copyNumber(&tmp, &result);

    puts("pi = ");
    dispNumberZeroSuppress(&result); putchar('\n');
    return 0;
}
