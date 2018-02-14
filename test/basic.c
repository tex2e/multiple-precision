#include <stdio.h>
#include <limits.h>
#include "integer.h"

#define my_assert(expr) \
    (!(expr) && printf("%s(%d): my_assert failed: "#expr"\n", \
    __FILE__, __LINE__))

void test_mulBy10();
void test_mulBy10E();
void test_divBy10();

int main() {
    test_mulBy10();
    test_mulBy10E();
    test_divBy10();

    printf("Finish all tests.\n");
    return 0;
}

void test_mulBy10() {
    printf("* test_mulBy10\n");
    int expected = 100000;
    int result;
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
    int expected = 100000;
    int result;
    Number a;
    Number tmp;
    setInt(&a, 1);
    mulBy10E(5, &a, &tmp); copyNumber(&tmp, &a);
    getInt(&a, &result);
    my_assert((result == expected) && "test 1 * 1e5");
}

void test_divBy10() {
    printf("* test_mulBy10\n");
    int expected = 10;
    int result;
    Number a;
    Number tmp;
    setInt(&a, 100000);
    divBy10(&a, &tmp); copyNumber(&tmp, &a);
    divBy10(&a, &tmp); copyNumber(&tmp, &a);
    divBy10(&a, &tmp); copyNumber(&tmp, &a);
    divBy10(&a, &tmp); copyNumber(&tmp, &a);
    getInt(&a, &result);
    my_assert((result == expected) && "test 1e5 / 1e4");
}
