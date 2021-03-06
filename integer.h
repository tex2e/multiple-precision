#ifndef INTEGER_H
#define INTEGER_H

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <string.h>
#include <limits.h>

// boolean
typedef int bool;
#define FALSE 0
#define TRUE (!FALSE)

// digit size
// Total digit size is KETA * RADIX_LEN
// To change this, gives -D KETA=<number> to gcc.
#ifndef KETA
#define KETA 100
#endif

// digit_t   |    int     |        long
// ----------+------------+--------------------
// digit_max | 2147483647 | 9223372036854775807
// RADIX     |      10000 |          1000000000
// RADIX_LEN |          4 |                   9
//
// RADIX=10 means it uses the ten digits from 0 through 9.
// RADIX=10000 means it uses the thousand digits from 0000 through 9999.
// RADIX_LEN is the number of RADIX width.
// To change these, gives -D RADIX=1000000000 and -D RADIX_LEN=9 to gcc.
//
// Use "long" or "int" as digit type.
// To use long, gives -D NUMBER_DIGIT_INT=TRUE to gcc.
#ifndef NUMBER_DIGIT_INT
// long
#define digit_t long
#define DIV ldiv
#define DIV_t ldiv_t
#define DIGIT_MAX LONG_MAX
#ifndef RADIX
#define RADIX 1000000000
#endif
#ifndef RADIX_LEN
#define RADIX_LEN 9
#endif
#else
// int
#define digit_t int
#define DIV div
#define DIV_t div_t
#define DIGIT_MAX INT_MAX
#ifndef RADIX
#define RADIX 10000
#endif
#ifndef RADIX_LEN
#define RADIX_LEN 4
#endif
#endif

// useful macros
#define MAX(a,b) ((a) > (b) ? a : b)
#define MIN(a,b) ((a) < (b) ? a : b)
#define ABS(a) ((a) < 0 ? -(a) : a)

typedef struct NUMBER {
    digit_t n[KETA];
    char sign;
} Number;

void clearByZero(Number*);
void copyNumber(const Number*, Number*);
void dispNumber(const Number*);
void dispNumberZeroSuppress(const Number*);
void setRand(Number*, int);
void getAbs(const Number*, Number*);
bool isZero(const Number*);
int mulBy10(const Number*, Number*);
int mulBy10E(int, const Number*, Number*);
int divBy10(const Number*, Number*);
int divBy10E(int, const Number*, Number*);
void swapNumber(Number*, Number*);
int setInt(Number*, digit_t);
int getInt(const Number*, digit_t*);
void getStr(const Number*, char*);
void setSign(Number*, int);
int getSign(const Number*);
int compNumber(const Number*, const Number*);
int addPositiveNumber(const Number*, const Number*, Number*);
int subPositiveNumber(const Number*, const Number*, Number*);
int add(const Number*, const Number*, Number*);
int sub(const Number*, const Number*, Number*);
int increment(const Number*, Number*);
int decrement(const Number*, Number*);
int slowMultiplePositiveNumber(const Number*, const Number*, Number*);
int multiplePositiveNumber(const Number*, const Number*, Number*);
int multipleToomCook(const Number*, const Number*, Number*);
int multiple(const Number*, const Number*, Number*);
int slowDivmodPositiveNumber(const Number*, const Number*, Number*, Number*);
int divmodPositiveNumber(const Number*, const Number*, Number*, Number*);
int divmodPositiveNumberByInt(const Number*, digit_t, Number*, digit_t*);
int divmodKunthAlgorithmD(const Number*, const Number*, Number*, Number*);
int divmod(const Number*, const Number*, Number*, Number*);
int power(const Number*, const Number*, Number*);
bool isPrime(const Number*);
int factorizeNumber(const Number*, Number*, int);
int factorialNumber(const Number*, Number*);
int gcdNumber(const Number*, const Number*, Number*);
int lcmNumber(const Number*, const Number*, Number*);
int sqrtNumberInt(const Number*, Number*);
int sqrtNumber(const Number*, int, Number*);
int inverseNumber(const Number*, int, Number*);
int inverseSqrtNumber(const Number*, int, Number*);
int arctan(int, int, Number*);

#endif
