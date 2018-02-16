
// N = 8:
//   全ての桁が1である自然数をレピュニット（repunit）という。1,11,111,...である。
//   n = 1,2,3,...に対して10進法では R_n = (10^n - 1)/9 という形で表される。
//   なるべく大きなnまで、R_nを素数であるかどうか判定し、合成数であれば素因数分解せよ。

#include <stdio.h>
#include "integer.h"

int main(void) {
    int n;
    int i, factor_i;
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

    // start from R_2 = 11
    for (n = 2; n < 100; n++) {
        Number factors[100];
        factor_i = 0;

        // R_n = (10^n - 1) / 9
        decrement(&base, &numer);
        divmodByInt(&numer, &nine_, &R_n, &rem);

        // --- Factoring ---
        // TODO: functionarize

        sqrtNumber(&R_n, &max); // max = sqrt(R_n)
        copyNumber(&R_n, &num); // num = R_n

        printf("R_%d = ", n); dispNumberZeroSuppress(&num); putchar('\n');

        setInt(&divisor, 3);
        isPrimeNumber = TRUE;
        while (1) {
            if (compNumber(&divisor, &max) > 0) break;

            // skip if divisor != 5 && divisor % 5 == 0
            if (compNumber(&divisor, &five) != 0 && divisor.n[0] % 10 == 5) {
                add(&divisor, &two, &tmp); copyNumber(&tmp, &divisor); // divisor += 2
                continue;
            }

            divmod(&num, &divisor, &quot, &remain);
            if (compNumber(&remain, &zero) == 0) {
                factors[factor_i++] = divisor;
                copyNumber(&quot, &num);
                isPrimeNumber = FALSE;
                continue;
            }

            add(&divisor, &two, &tmp); copyNumber(&tmp, &divisor); // divisor += 2
        }
        if (!isPrimeNumber && compNumber(&num, &one) != 0) {
            factors[factor_i++] = num;
        }

        if (isPrimeNumber) {
            printf("R_%d is prime number.\n", n);
        } else {
            printf("R_%d is not a prime number.\n", n);
            puts("Factors:");
            for (i = 0; i < factor_i; i++) {
                dispNumberZeroSuppress(&factors[i]); putchar('\n');
            }
        }

        printf("\n");

        // next
        mulBy10(&base, &tmp); copyNumber(&tmp, &base);
    }

    return 0;
}
