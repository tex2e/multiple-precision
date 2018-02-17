#include <stdio.h>
#include <limits.h>
#include "integer.h"

#define my_assert(expr) \
    (!(expr) && printf("%s(%d): my_assert failed: "#expr"\n", \
    __FILE__, __LINE__))

void test_mulBy10();
void test_mulBy10E();
void test_divBy10();
void test_divBy10E();

int main() {
    test_mulBy10();
    test_mulBy10E();
    test_divBy10();
    test_divBy10E();

    printf("Finish all tests.\n");
    return 0;
}

void test_mulBy10() {
    printf("* test_mulBy10\n");
    digit_t expected = 100000;
    digit_t result;
    Number a;
    Number tmp;
    setInt(&a, 1);
    mulBy10(&a, &tmp); copyNumber(&tmp, &a);
    mulBy10(&a, &tmp); copyNumber(&tmp, &a);
    mulBy10(&a, &tmp); copyNumber(&tmp, &a);
    mulBy10(&a, &tmp); copyNumber(&tmp, &a);
    mulBy10(&a, &tmp); copyNumber(&tmp, &a);
    getInt(&a, &result);
    my_assert((result == expected) && "test 1 * 1e5");
}

void test_mulBy10E() {
    printf("* test_mulBy10E\n");
    digit_t expected = 100000;
    digit_t result;
    Number a;
    Number tmp;
    setInt(&a, 1);
    mulBy10E(5, &a, &tmp); copyNumber(&tmp, &a);
    getInt(&a, &result);
    my_assert((result == expected) && "test 1 * 1e5");
}

void test_divBy10() {
    printf("* test_mulBy10\n");
    digit_t expected = 10;
    digit_t rem_expected = 7;
    digit_t result;
    digit_t rem_result;
    Number a;
    Number tmp;
    setInt(&a, 107000);
    divBy10(&a, &tmp); copyNumber(&tmp, &a);
    divBy10(&a, &tmp); copyNumber(&tmp, &a);
    divBy10(&a, &tmp); copyNumber(&tmp, &a);
    rem_result = divBy10(&a, &tmp); copyNumber(&tmp, &a);
    getInt(&a, &result);
    my_assert((result == expected) && "test 1.07e5 / 1e4");
    my_assert((rem_result == rem_expected) && "test 1.07e5 %% 1e4");
}

void test_divBy10E() {
    printf("* test_mulBy10E\n");
    digit_t expected = 10;
    digit_t result;
    Number a;
    Number tmp;
    setInt(&a, 100000);
    divBy10E(4, &a, &tmp); copyNumber(&tmp, &a);
    getInt(&a, &result);
    my_assert((result == expected) && "test 1e5 / 1e4");
}
