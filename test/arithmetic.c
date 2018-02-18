#include <stdio.h>
#include <limits.h>
#include "integer.h"

#define my_assert(expr) \
    (!(expr) && printf("%s(%d): my_assert failed: "#expr"\n", \
    __FILE__, __LINE__))

void test_add();
void test_add_big();
void test_sub();
void test_sub_big();
void test_mul();
void test_mul_big();
void test_divmod();
void test_divmod_big();
void test_divmodByInt();
void test_divmodByInt_big();
void test_power();
void test_power_big();
void test_isPrime();
void test_factorialNumber();

int main(void) {
    test_add();
    test_add_big();
    test_sub();
    test_sub_big();
    test_mul();
    test_mul_big();
    test_divmod();
    test_divmod_big();
    test_divmodByInt(); // private method
    test_divmodByInt_big(); // private method
    test_power();
    test_power_big();
    test_isPrime();
    test_factorialNumber();

    printf("Finish all tests.\n");
}

void test_add() {
    printf("* test_add\n");
    digit_t expected = 12345 + 67890;
    digit_t result;
    Number a, b, c;
    setInt(&a, 12345);
    setInt(&b, 67890);
    add(&a, &b, &c);
    getInt(&c, &result);
    my_assert((result == expected) && "test 12345 + 67890");
}

void test_add_big() {
    printf("* test_add_big\n");
    char* expected = "802350000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000";
    char result[KETA * RADIX_LEN + 2];
    Number a, b, c;
    Number tmp;
    setInt(&a, 12345); mulBy10E(100, &a, &tmp); copyNumber(&tmp, &a);
    setInt(&b, 67890); mulBy10E(100, &b, &tmp); copyNumber(&tmp, &b);
    add(&a, &b, &c);
    getStr(&c, result);
    my_assert((strcmp(result, expected) == 0) && "test 12345E100 + 67890E100");
}

void test_sub() {
    printf("* test_sub\n");
    digit_t expected = 12345 - 67890;
    digit_t result;
    Number a, b, c;
    setInt(&a, 12345);
    setInt(&b, 67890);
    sub(&a, &b, &c);
    getInt(&c, &result);
    my_assert((result == expected) && "test 12345 - 67890");
}

void test_sub_big() {
    printf("* test_sub_big\n");
    char* expected = "-555450000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000";
    char result[KETA * RADIX_LEN + 2];
    Number a, b, c;
    Number tmp;
    setInt(&a, 12345); mulBy10E(100, &a, &tmp); copyNumber(&tmp, &a);
    setInt(&b, 67890); mulBy10E(100, &b, &tmp); copyNumber(&tmp, &b);
    sub(&a, &b, &c);
    getStr(&c, result);
    my_assert((strcmp(result, expected) == 0) && "test 12345E100 - 67890E100");
}

void test_mul() {
    printf("* test_mul\n");
    digit_t expected = 12345 * 67890;
    digit_t result;
    Number a, b, c;
    setInt(&a, 12345);
    setInt(&b, 67890);
    multiple(&a, &b, &c);
    getInt(&c, &result);
    my_assert((result == expected) && "test 12345 * 67890");
}

void test_mul_big() {
    printf("* test_mul_big\n");
    char* expected = "8381020500000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000";
    char result[KETA * RADIX_LEN + 2];
    Number a, b, c;
    Number tmp;
    setInt(&a, 12345); mulBy10E(50, &a, &tmp); copyNumber(&tmp, &a);
    setInt(&b, 67890); mulBy10E(50, &b, &tmp); copyNumber(&tmp, &b);
    multiple(&a, &b, &c);
    getStr(&c, result);
    my_assert((strcmp(result, expected) == 0) && "test 12345E50 * 67890E50");
}

void test_divmod() {
    printf("* test_divmod\n");
    digit_t div_expected = 1234500 / 67890;
    digit_t div_result;
    digit_t mod_expected = 1234500 % 67890;
    digit_t mod_result;
    Number a, b, c, d;
    setInt(&a, 1234500);
    setInt(&b, 67890);
    divmod(&a, &b, &c, &d);
    getInt(&c, &div_result);
    getInt(&d, &mod_result);
    my_assert((div_result == div_expected) && "test 1234500 / 67890");
    my_assert((mod_result == mod_expected) && "test 1234500 %% 67890");
}

void test_divmod_big() {
    printf("* test_divmod_big\n");
    char* div_expected = "18183826778612461334511710119310649580203269995581";
    char* mod_expected = "591000000000000000000000000000000000000000000000000000";
    char div_result[KETA * RADIX_LEN + 2];
    char mod_result[KETA * RADIX_LEN + 2];
    Number a, b, c, d;
    Number tmp;
    setInt(&a, 12345); mulBy10E(100, &a, &tmp); copyNumber(&tmp, &a);
    setInt(&b, 67890); mulBy10E( 50, &b, &tmp); copyNumber(&tmp, &b);
    divmod(&a, &b, &c, &d);
    getStr(&c, div_result);
    getStr(&d, mod_result);
    my_assert((strcmp(div_result, div_expected) == 0) && "test 12345E100 / 67890E50");
    my_assert((strcmp(mod_result, mod_expected) == 0) && "test 12345E100 %% 67890E50");
}

void test_divmodByInt() {
    printf("* test_divmodByInt\n");
    digit_t div_expected = 18;
    digit_t div_result;
    digit_t mod_expected = 12480;
    digit_t mod_result;
    Number a, c;
    digit_t b, d;
    setInt(&a, 1234500);
    b = 67890;
    divmodByInt(&a, &b, &c, &d);
    getInt(&c, &div_result);
    mod_result = d;
    my_assert((div_result == div_expected) && "test 1234500 / 67890");
    my_assert((mod_result == mod_expected) && "test 1234500 %% 67890");
}

void test_divmodByInt_big() {
    printf("* test_divmodByInt_big\n");
    char* div_expected = "1818382677861246133451171011931064958020326999558108705258506407423773751657092355280600972160848431";
    char div_result[KETA * RADIX_LEN + 2];
    digit_t mod_expected = 19410;
    digit_t mod_result;
    Number tmp;
    Number a, c;
    digit_t b, d;
    setInt(&a, 12345); mulBy10E(100, &a, &tmp); copyNumber(&tmp, &a);
    b = 67890;
    divmodByInt(&a, &b, &c, &d);
    getStr(&c, div_result);
    mod_result = d;
    my_assert((strcmp(div_result, div_expected) == 0) && "test 12345E100 / 67890");
    my_assert((mod_result == mod_expected) && "test 12345E100 %% 67890");
}

void test_power() {
    printf("* test_power\n");
    digit_t expected = pow(2, 20);
    digit_t result;
    Number a, b, c;
    setInt(&a, 2);
    setInt(&b, 20);
    power(&a, &b, &c);
    getInt(&c, &result);
    my_assert((result == expected) && "test 2^20");
}

void test_power_big() {
    printf("* test_power_big\n");
    char* expected = "11110408185131956285910790587176451918559153212268021823629073199866111001242743283966127048043";
    char result[KETA * RADIX_LEN + 2];
    Number a, b, c;
    setInt(&a, 123);
    setInt(&b, 45);
    power(&a, &b, &c);
    getStr(&c, result);
    my_assert((strcmp(result, expected) == 0) && "test 123 ** 45");
}

void test_isPrime() {
    printf("* test_isPrime\n");
    Number a, b;
    setInt(&a, 1234567);
    setInt(&b, 127);
    my_assert(!isPrime(&a) && "test !isPrime(1234567)");
    my_assert(isPrime(&b) && "test isPrime(127)");
}

void test_factorialNumber() {
    printf("* test_factorialNumber\n");
    char* expected = "3628800";
    char result[KETA * RADIX_LEN + 2];
    Number a, b;
    setInt(&a, 10);
    factorialNumber(&a, &b);
    getStr(&b, result);
    my_assert((strcmp(result, expected) == 0) && "test 10!");
}
