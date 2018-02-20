
// N = 8:
//   全ての桁が1である自然数をレピュニット（repunit）という。1,11,111,...である。
//   n = 1,2,3,...に対して10進法では R_n = (10^n - 1)/9 という形で表される。
//   なるべく大きなnまで、R_nを素数であるかどうか判定し、合成数であれば素因数分解せよ。

#include <stdio.h>
#include "integer.h"

int main(void) {
    int n;
    int i, factor_i;
    char result[KETA * RADIX_LEN + 2];
    Number tmp, _;
    Number base;
    Number num, numer;
    Number divisor, quot, remain, max;
    Number R_n;
    Number one, two, zero, five;
    digit_t nine_ = 9;
    digit_t rem;
    bool isPrimeNumber;

    setInt(&base, 10 * 10);
    setInt(&one, 1);
    setInt(&two, 2);
    setInt(&zero, 0);
    setInt(&five, 5);

    // R_2..R_18
    for (n = 2; n <= 18; n++) {
        Number factors[100];
        factor_i = 0;

        // R_n = (10^n - 1) / 9
        decrement(&base, &numer);
        divmodByInt(&numer, &nine_, &R_n, &rem);

        getStr(&R_n, result);
        printf("R_%d = %s", n, result);

        // Factorization
        int factorCount = factorizeNumber(&R_n, factors, 100);
        if (factorCount == 0) {
            printf(" is a prime number.\n");
        } else if (factorCount > 0) {
            printf(" = ");
            for (i = 0; i < factorCount; i++) {
                getStr(&factors[i], result);
                printf("%s", result);
                if (i != factorCount - 1) printf(" * ");
            }
            printf("\n");
        } else {
            fprintf(stderr, "R_%d: factorCount is overflow (max: %d).\n", n, 100);
        }

        // next
        mulBy10(&base, &tmp); copyNumber(&tmp, &base);
    }

    return 0;
}
