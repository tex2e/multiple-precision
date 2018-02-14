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

#ifndef KETA
#define KETA 30
#endif

#ifndef RADIX
#define RADIX 10
#endif
#ifndef RADIX_LEN
#define RADIX_LEN 1
#endif

typedef struct NUMBER {
    int n[KETA];
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
void swapNumber(Number*, Number*);
int setInt(Number*, int);
int getInt(const Number*, int*);
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
int multiple(const Number*, const Number*, Number*);
int slowDivmodPositiveNumber(const Number*, const Number*, Number*, Number*);
int divmodPositiveNumber(const Number*, const Number*, Number*, Number*);
int divmod(const Number*, const Number*, Number*, Number*);
int power(const Number*, const Number*, Number*);
bool isPrime(const Number*);
int factorialNumber(const Number*, Number*);
int gcdNumber(const Number*, const Number*, Number*);
int lcmNumber(const Number*, const Number*, Number*);
int sqrtNumber(const Number*, Number*);

#endif
