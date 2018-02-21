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
    Number a, b;
    setInt(&a, 56);
    expo_result = inverseNumber(&a, 20, &b);
    getStr(&b, result);
    my_assert((strcmp(result, expected) == 0) && "test 1/56");
}