#include <stdio.h>
#include <limits.h>

// KETA = 100
#include "integer.h"

#define my_assert(expr) \
    (!(expr) && printf("%s(%d): my_assert failed: "#expr"\n", \
    __FILE__, __LINE__))

void test_add();
void test_sub();
void test_mul();
void test_divmod();

int main(void) {
    test_add();
    test_sub();
    test_mul();
    test_divmod();

    printf("Finish all tests.\n");
}

void test_add() {
    printf("* test_add\n");
    int expected = 12345 + 67890 + 1;
    int result;
    Number a, b, c;
    clearByZero(&a);
    clearByZero(&b);
    clearByZero(&c);
    setInt(&a, 12345);
    setInt(&b, 67890);
    add(&a, &b, &c);
    getInt(&c, &result);
    my_assert((result == expected) && "test 12345 + 67890");
}

void test_sub() {
    printf("* test_sub\n");
    int expected = 12345 - 67890;
    int result;
    Number a, b, c;
    clearByZero(&a);
    clearByZero(&b);
    clearByZero(&c);
    setInt(&a, 12345);
    setInt(&b, 67890);
    sub(&a, &b, &c);
    getInt(&c, &result);
    my_assert((result == expected) && "test 12345 - 67890");
}

void test_mul() {
    printf("* test_mul\n");
    int expected = 12345 * 67890;
    int result;
    Number a, b, c;
    clearByZero(&a);
    clearByZero(&b);
    clearByZero(&c);
    setInt(&a, 12345);
    setInt(&b, 67890);
    multiple(&a, &b, &c);
    getInt(&c, &result);
    my_assert((result == expected) && "test 12345 * 67890");
}

void test_divmod() {
    printf("* test_divmod\n");
    int div_expected = (1048576 + 4) / 16;
    int div_result;
    int mod_expected = (1048576 + 4) % 16;
    int mod_result;
    Number a, b, c, d;
    clearByZero(&a);
    clearByZero(&b);
    clearByZero(&c);
    setInt(&a, 1048576 + 4);
    setInt(&b, 16);
    divmod(&a, &b, &c, &d);
    getInt(&c, &div_result);
    getInt(&d, &mod_result);
    my_assert((div_result == div_expected) && "test (2^20 + 4) / 2^4");
    my_assert((mod_result == mod_expected) && "test (2^20 + 4) %% 2^4");
}
