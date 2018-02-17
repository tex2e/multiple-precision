
// N = 10:
//   自然数pと2p+1が共に素数の場合、pのことをソフィー・ジェルマン素数という。
//   pが符号なし32ビットの上限値（約42億）よりも大きなソフィー・ジェルマン素数を10個以上示す。

#include <stdio.h>
#include "integer.h"

int main(void) {
    Number tmp, _;
    Number p, p2;
    Number two;
    int sophieGermainPrimeCount = 0;

    setInt(&two, 2);

    setInt(&p, UINT_MAX);
    if (UINT_MAX % 2 == 0) {
        // p should be odd number
        increment(&p, &tmp); copyNumber(&tmp, &p);
    }

    while (1) {
        if (sophieGermainPrimeCount == 10) break;

        if (isPrime(&p)) {
            // p2 = 2 * p + 1
            multiple(&p, &two, &p2);
            increment(&p2, &tmp); copyNumber(&tmp, &p2);

            if (isPrime(&p2)) {
                printf("   p = "); dispNumberZeroSuppress(&p); putchar('\n');
                printf("2p+1 = "); dispNumberZeroSuppress(&p2); putchar('\n');
                printf("\n");
                sophieGermainPrimeCount++;
            }
        }

        // next
        add(&p, &two, &tmp); copyNumber(&tmp, &p); // p += 2
    }

    return 0;
}
