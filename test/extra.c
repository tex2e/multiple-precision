#include <stdio.h>
#include <limits.h>
#include "integer.h"

#define my_assert(expr) \
    (!(expr) && printf("%s(%d): my_assert failed: "#expr"\n", \
    __FILE__, __LINE__))

void test_inverseNumber();

int main(void) {
    test_inverseNumber();

    printf("Finish all tests.\n");
}

void test_inverseNumber() {
    printf("* test_inverseNumber\n");
    int expo_expected = 20;
    int expo_result;
    char* expected = "1785714285714285714";
    char result[KETA * RADIX_LEN + 2];
    Number tmp;
    Number a, b;
    setInt(&a, 56);
    expo_result = inverseNumber(&a, 40, &b);
    divBy10E(20, &b, &tmp); copyNumber(&tmp, &b);
    getStr(&b, result);
    // printf("expected = %s\n", expected);
    // printf("result   = %s\n", result);
    my_assert((strcmp(result, expected) == 0) && "test 1/56");
}
