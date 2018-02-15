
// N = 6: 次のBailey, Borwein, Plouffeの公式によりπを小数点以下1000桁求める。
//
//        ∞
//   π =  Σ 1/16^n ( 4/(8n+1) - 2/(8n+4) - 1/(8n+5) - 1/(8n+6) )
//       n=0
//
//    4/(8n+1) - 2/(8n+4) - 1/(8n+5) - 1/(8n+6)
//  = 4/(8n+1) - 1/(4n+2) - 1/(8n+5) - 1/(8n+6)
//  = {4(4n+2)(8n+5)(8n+6) -(8n+1)(8n+5)(8n+6) -(8n+1)(4n+2)(8n+6) -(8n+1)(4n+2)(8n+5)}
//    / (8n+1)(4n+2)(8n+5)(8n+6)
//  = { (1024 n^3 + 1920 n^2 + 1184 n + 240) - (512 n^3 + 768 n^2 + 328 n + 30)
//      - (256 n^3 + 352 n^2 + 136 n + 12) - (256 n^3 + 320 n^2 + 116 n + 10) }
//    / (8n+1)(4n+2)(8n+5)(8n+6)
//  = { (1024 - 512 - 256 - 256) n^3 + (1920 - 768 - 352 - 320) n^2
//      + (1184 - 328 - 136 - 116) n + (240 - 30 - 12 - 10) }
//    / (8n+1)(4n+2)(8n+5)(8n+6)
//  = (480 n^2 + 604 n^1 + 188) / (8n+1)(4n+2)(8n+5)(8n+6)
//  = 4(120 n^2 + 151 n^1 + 47) / (8n+1)(4n+2)(8n+5)(8n+6)
//  = (120 n^2 + 151 n^1 + 47) / (8n+1)(2n+1)(8n+5)(4n+3)
//  = (120 n^2 + 151 n^1 + 47) / (512 n^4 + 1024 n^3 + 712 n^2 + 194 n + 15)
//

#include <stdio.h>
#include "integer.h"

int main(void) {
    int digits = 1000;
    int extraDigits = 10;
    int calcDigits = digits + extraDigits;
    Number tmp, _;
    Number n1, n2, n3, n4, num16n;
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

    clearByZero(&n1);
    clearByZero(&n2);
    clearByZero(&n3);
    clearByZero(&n4);
    clearByZero(&result);

    mulBy10E(calcDigits, &one, &prec); // prec = 1e+digits

    while (1) {
        clearByZero(&numer);
        clearByZero(&denom);

        multiple(&n1, &n1, &n2);
        multiple(&n2, &n1, &n3);
        multiple(&n2, &n2, &n4);

        // numer = 120 n^2 + 151 n^1 + 47
        multiple(&onetwenty, &n2, &term); // term = 120 * n^2
        add(&numer, &term, &tmp); copyNumber(&tmp, &numer); // numer += term
        multiple(&onefiftyone, &n1, &term); // term = 151 * n
        add(&numer, &term, &tmp); copyNumber(&tmp, &numer); // numer += term
        add(&numer, &fortyseven, &tmp); copyNumber(&tmp, &numer); // numer += 47

        // denom = 512 n^4 + 1024 n^3 + 712 n^2 + 194 n + 15
        multiple(&fivetwelve, &n4, &term); // term = 512 n^4
        add(&denom, &term, &tmp); copyNumber(&tmp, &denom); // denom += term
        multiple(&tentwentyfour, &n3, &term); // term = 1024 n^3
        add(&denom, &term, &tmp); copyNumber(&tmp, &denom); // denom += term
        multiple(&seventwelve, &n2, &term); // term = 712 n^2
        add(&denom, &term, &tmp); copyNumber(&tmp, &denom); // denom += term
        multiple(&oneninetyfour, &n1, &term); // term = 194 n
        add(&denom, &term, &tmp); copyNumber(&tmp, &denom); // denom += term
        add(&denom, &fifteen, &tmp); copyNumber(&tmp, &denom); // denom += 15

        multiple(&numer, &prec, &tmp); copyNumber(&tmp, &numer); // numer *= prec

        power(&sixteen, &n1, &num16n);
        multiple(&denom, &num16n, &tmp); copyNumber(&tmp, &denom); // denom *= 16^n

        if (compNumber(&numer, &denom) < 0) break;
        divmod(&numer, &denom, &quot, &_);
        add(&result, &quot, &tmp); copyNumber(&tmp, &result);

        // next
        increment(&n1, &tmp); copyNumber(&tmp, &n1);
    }

    divBy10E(extraDigits, &result, &tmp); copyNumber(&tmp, &result);

    puts("pi = ");
    dispNumberZeroSuppress(&result); putchar('\n');
    return 0;
}
