
// N = 4: 自然対数の底eを小数点以下1000桁求める。
//
//         e ~= 1/0! + 1/1! + 1/2! + 1/3! + ... + 1/999! + 1/1000!
//   1000! e ~= 1000! + 1000! + _{1000}P_{999} + _{1000}P_{998}
//              + ... + _{1000}P_{2} + _{1000}P_{1} + 1
//
//   log10(1000!) ~= 2568

#include <stdio.h>
#include "integer.h"

int main(void) {
    int r;
    int digits = 1000;
    Number tmp, _;
    Number thousand;
    Number term;
    Number nFact;
    Number sum;
    Number result;

    clearByZero(&sum);
    clearByZero(&result);
    clearByZero(&nFact);
    setInt(&thousand, digits);
    setInt(&nFact, 1);

    setInt(&sum, 1); // sum = 1
    copyNumber(&thousand, &term); // term = 1000

    while (1) {
        r = multiple(&nFact, &term, &tmp); copyNumber(&tmp, &nFact); // nFact *= term
        if (r != 0) break;

        add(&sum, &nFact, &tmp); copyNumber(&tmp, &sum); // sum += nFact
        decrement(&term, &tmp); copyNumber(&tmp, &term); // term--
        if (isZero(&term)) break;
    }

    mulBy10E(digits, &sum, &tmp); copyNumber(&tmp, &sum); // sum *= 1e+digits

    // 1000! e = sum * 1e+1000
    //       e = sum * 1e+1000 / 1000!

    divmod(&sum, &nFact, &result, &_); // result = sum / nFact

    puts("e = ");
    dispNumberZeroSuppress(&result); putchar('\n');

    return 0;
}
