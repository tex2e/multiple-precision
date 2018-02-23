#include <stdio.h>
#include <limits.h>
#include "integer.h"

#define my_assert(expr) \
    (!(expr) && printf("%s(%d): my_assert failed: "#expr"\n", \
    __FILE__, __LINE__))

void test_inverseNumber();
void test_divmodKunthAlgorithmD();

int main(void) {
    test_inverseNumber();
    test_divmodKunthAlgorithmD();

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

void test_divmodKunthAlgorithmD() {
    printf("* test_divmodKunthAlgorithmD\n");
    {
        char* div_expected = "18183826778612461334511710119310649580203269995581";
        char* mod_expected = "591000000000000000000000000000000000000000000000000000";
        char div_result[KETA * RADIX_LEN + 2];
        char mod_result[KETA * RADIX_LEN + 2];
        Number a, b, c, d;
        Number tmp;
        setInt(&a, 12345); mulBy10E(100, &a, &tmp); copyNumber(&tmp, &a);
        setInt(&b, 67890); mulBy10E( 50, &b, &tmp); copyNumber(&tmp, &b);
        divmodKunthAlgorithmD(&a, &b, &c, &d);
        getStr(&c, div_result);
        getStr(&d, mod_result);
        my_assert((strcmp(div_result, div_expected) == 0) && "test 12345E100 / 67890E50");
        my_assert((strcmp(mod_result, mod_expected) == 0) && "test 12345E100 %% 67890E50");
    }
    {
        char* div_expected = "18183";
        char* mod_expected = "5613000000";
        char div_result[KETA * RADIX_LEN + 2];
        char mod_result[KETA * RADIX_LEN + 2];
        Number a, b, c, d;
        Number tmp;
        setInt(&a, 12345); mulBy10E(10, &a, &tmp); copyNumber(&tmp, &a);
        setInt(&b, 67890); mulBy10E( 5, &b, &tmp); copyNumber(&tmp, &b);
        divmodKunthAlgorithmD(&a, &b, &c, &d);
        getStr(&c, div_result);
        getStr(&d, mod_result);
        // printf("div_expected = %s\n", div_expected);
        // printf("div_result   = %s\n", div_result);
        // printf("mod_expected = %s\n", mod_expected);
        // printf("mod_result   = %s\n", mod_result);
        my_assert((strcmp(div_result, div_expected) == 0) && "test 12345E10 / 67890E5");
        my_assert((strcmp(mod_result, mod_expected) == 0) && "test 12345E10 %% 67890E5");
    }
}
