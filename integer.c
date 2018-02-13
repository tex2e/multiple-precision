
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <string.h>
#include <assert.h>
#include "integer.h"

// Clear by zero (n[..] = 0, sign = 1)
void clearByZero(Number *num) {
    int i;
    for (i = 0; i < KETA; i++) {
        num->n[i] = 0;
    }
    setSign(num, 1);
}

// Deep copy
void copyNumber(const Number *from, Number *to) {
    *to = *from;
}

// Display number
// + 0 0 0 0 0 1 2 3 4 5
void dispNumber(const Number *num) {
    int i;
    char format[10];
    char sign = (getSign(num) < 0) ? '-': '+';

    sprintf(format, " %%0%dd", RADIX_LEN);

    printf("%c", sign);
    for (i = KETA - 1; i >= 0; i--) {
        printf(format, num->n[i]);
    }
}

// Display number by suppress zero
// + 1 2 3 4 5
void dispNumberZeroSuppress(const Number *num) {
    int i;
    char format[10];
    char sign = (getSign(num) < 0) ? '-': '+';

    sprintf(format, " %%0%dd", RADIX_LEN);

    printf("%c", sign);
    // skip zero digits
    for (i = KETA - 1; num->n[i] == 0 && i != 0; i--)
        ;

    for (; i >= 0; i--) {
        printf(format, num->n[i]);
    }
}

// Set random number
// num[0..digits] <- random
void setRand(Number *num, int digits) {
    int i;
    int sign;

    if (digits > KETA) {
        printf("setRand: digits > KETA\n\n");
        return;
    }

    clearByZero(num);
    for (i = 0; i < digits; i++) {
        num->n[i] = random() % RADIX;
    }
    sign = (random() % 2 == 0) ? 1: -1;
    setSign(num, sign);
}

// absNum <- |num|
void getAbs(const Number *num, Number *absNum) {
    copyNumber(num, absNum);
    setSign(absNum, 1);
}

// Check if given number is zero
// Return TRUE  if num == 0
// Return FALSE if num != 0
bool isZero(const Number *num) {
    int i;
    for (i = 0; i < KETA; i++) {
        if (num->n[i] != 0) return FALSE;
    }
    return TRUE;
}

// result <- num * 10
//   where num >= 0
// Return  0 if success
// Return -1 if overflow the result
int mulBy10(const Number *num, Number *result) {
    int i;
    int leftDigit, currentDigit;
    clearByZero(result);

    // result = num if num == 0
    if (isZero(num)) {
        copyNumber(num, result);
        return 0;
    }

    i = KETA - 1;
    leftDigit    = num->n[i] * 10 / RADIX;
    currentDigit = num->n[i] * 10 % RADIX;
    result->n[i] = currentDigit;
    if (leftDigit > 0) {
        printf("mulBy10: overflow\n\n");
        return -1;
    }

    for (i = KETA - 2; i >= 0; i--) {
        leftDigit    = num->n[i] * 10 / RADIX;
        currentDigit = num->n[i] * 10 % RADIX;
        result->n[i+1] += leftDigit;
        result->n[i  ] = currentDigit;
    }

    setSign(result, getSign(num));

    return 0;
}

// result <- num * 10e+exponent
//   where num >= 0, exponent >= 0
// Return  0 if success
// Return -1 if exponent is less than 0
// Return -2 if overflow the result
int mulBy10E(int exponent, const Number *num, Number *result) {
    int i;
    Number tmp, tmpResult;
    clearByZero(result);

    if (exponent < 0) return -1;

    // result = num if exponent == 0 or num == 0
    if (exponent == 0 || isZero(num)) {
        copyNumber(num, result);
        return 0;
    }

    copyNumber(num, &tmpResult);
    for (i = 0; i < exponent; i++) {
        mulBy10(&tmpResult, &tmp); copyNumber(&tmp, &tmpResult);
    }
    copyNumber(&tmpResult, result);

    setSign(result, getSign(num));

    return 0;
}

// result <- num / 10
// Return num % 10
int divBy10(const Number *num, Number *result) {
    int i;
    int remain;
    int leftDigit, currentDigit;
    clearByZero(result);

    if (isZero(num)) {
        copyNumber(num, result);
        return 0;
    }

    // remain = num->n[0];
    //
    // for (i = 1; i < KETA; i++) {
    //     result->n[i-1] = num->n[i];
    // }
    // result->n[KETA-1] = 0;
    //
    remain = num->n[0] % 10;

    for (i = 0; i < KETA - 1; i++) {
        leftDigit    = num->n[i+1] % 10;
        currentDigit = leftDigit * (RADIX / 10) + num->n[i] / 10;
        result->n[i] = currentDigit;
    }

    i = KETA - 1;
    currentDigit = num->n[i] / 10;
    result->n[i] = currentDigit;

    setSign(result, getSign(num));

    return remain;
}

// Swap a, b
void swapNumber(Number *a, Number *b) {
    Number tmp;
    tmp = *a; *a = *b; *b = tmp;
}

// Set int value to Number
// Return  0 if success
// Return -1 if cannot set the value x into the num
int setInt(Number *num, int x) {
    int i = 0;
    clearByZero(num);

    // if negative
    if (x < 0) {
        setSign(num, -1);
        x *= -1;
    }

    while (1) {
        // break if overflow
        if (i >= KETA) return -1;
        num->n[i++] = x % RADIX;
        // break if finish
        if (x / RADIX == 0) break;
        x /= RADIX;
    }
    return 0;
}

// Get int value from Number
// Return  0 if success
// Return -1 if digit number of integer is out of int (TODO)
int getInt(const Number *num, int *x) {
    int i;
    *x = 0;

    for (i = 0; i < KETA; i++) {
        *x += num->n[i] * (int)pow(RADIX, i);
    }
    if (getSign(num) == -1) {
        *x *= -1;
    }
    return 0;
}

// Get str from Number
void getStr(const Number *num, char* result) {
    int i;
    char format[10];
    char formatSuffix[10];
    char buffer[RADIX_LEN + 2];
    char str[KETA * RADIX_LEN + 2] = "";
    char* sign = (getSign(num) < 0) ? "-": "";

    sprintf(format, "%%0%dd", RADIX_LEN);
    sprintf(formatSuffix, "%%d");

    strcat(str, sign);

    // skip zero digits
    for (i = KETA - 1; num->n[i] == 0 && i != 0; i--)
        ;

    // Get first digit without 0 padding.
    sprintf(buffer, formatSuffix, num->n[i]);
    strcat(str, buffer);
    i--;

    // Get rest of the digits
    for (; i >= 0; i--) {
        sprintf(buffer, format, num->n[i]);
        strcat(str, buffer);
    }

    strcpy(result, str);
}

void setSign(Number *num, int sign) {
    if (sign != 1 && sign != -1) {
        printf("setSign: invalid argument, sign : %d\n\n", sign);
        return;
    }
    num->sign = sign;
}

int getSign(const Number *num) {
    return num->sign;
}

// Compare given 2 numbers
// Return  1 if num < otherNum
// Return -1 if num > otherNum
// Return  0 if num == otherNum
int compNumber(const Number *num, const Number *otherNum) {
    int i;
    int sign;

    // there are 4 cases
    //   1. +x `compare` +x  =>  compare each digit
    //   2. +x `compare` -x  =>   1
    //   3. -x `compare` +x  =>  -1
    //   4. -x `compare` -x  =>  compare each digit; invert result (*(-1))

    // compare sign
    if (getSign(num) > getSign(otherNum)) return  1;
    if (getSign(num) < getSign(otherNum)) return -1;

    // if negative, invert result
    sign = getSign(num); // sign is 1 or -1

    // compare n[..]
    for (i = KETA - 1; i >= 0; i--) {
        if (num->n[i] > otherNum->n[i]) return  1 * sign;
        if (num->n[i] < otherNum->n[i]) return -1 * sign;
    }
    return 0;
}

// result <- a + b
//   where a >= 0, b >= 0
// Return  0 if successed
// Return -1 if overflow
int addPositiveNumber(const Number *a, const Number *b, Number *result) {
    int i, iMax;
    int carry = 0;
    clearByZero(result);

    // to ignore prefix 0 digits
    for (i = KETA - 1; i != 0 && a->n[i-1] == 0 && b->n[i-1] == 0; i--) ;
    iMax = (i + 1 >= KETA - 1) ? KETA - 1 : i + 1;

    for (i = 0; i <= iMax; i++) {
        int sum = a->n[i] + b->n[i] + carry;
        result->n[i] = sum % RADIX;
        carry = (sum >= RADIX) ? 1: 0;
    }

    // if overflow
    if (carry == 1) return -1;
    return 0;
}

// result <- a - b
//   where a >= 0, b >= 0
// Return  0 if successed
// Return -1 if underflow
int subPositiveNumber(const Number *_a, const Number *_b, Number *result) {
    int i, iMax;
    int borrow = 0;
    Number a, b;
    clearByZero(result);

    // a must be bigger than b
    if (compNumber(_a, _b) < 0) { // if _a < _b
        a = *_b;
        b = *_a;
        setSign(result, -1); // flag negative
    } else {
        a = *_a;
        b = *_b;
    }

    // to ignore prefix 0 digits
    for (i = KETA - 1; i != 0 && a.n[i-1] == 0 && b.n[i-1] == 0; i--) ;
    iMax = (i + 1 >= KETA - 1) ? KETA - 1 : i + 1;

    // compute diff
    for (i = 0; i <= iMax; i++) {
        int ai = a.n[i] - borrow;
        int bi = b.n[i];
        int diff;
        if (ai >= bi) {
            borrow = 0;
            diff = ai - bi;
        } else {
            borrow = 1;
            diff = RADIX + ai - bi;
        }
        result->n[i] = diff;
    }

    // if result is zero, set positive sign.
    if (isZero(result)) setSign(result, 1);

    // if underflow
    if (borrow == 1) return -1;
    return 0;
}

// result <- a + b
// Return  0 if successed
// Return -1 if underflow
// Return -2 if number has invalid sign
int add(const Number *a, const Number *b, Number *result) {
    int r;
    bool plusA = (getSign(a) == 1);
    bool plusB = (getSign(b) == 1);
    Number absA;
    Number absB;
    getAbs(a, &absA);
    getAbs(b, &absB);

    // there are 4 cases
    //   1. +a + +b  =>    a +  b   => add(a, b)
    //   2. +a + -b  =>    a - |b|  => sub(a, abs(b))
    //   3. -a + +b  =>    b - |a|  => sub(b, abs(a))
    //   4. -a + -b  =>  -(a +  b ) => setSign(add(a, b), -1)
    if (plusA && plusB) {
        r = addPositiveNumber(a, b, result);
    } else if (plusA && !plusB) {
        r = subPositiveNumber(a, &absB, result);
    } else if (!plusA && plusB) {
        r = subPositiveNumber(b, &absA, result);
    } else if (!plusA && !plusB) {
        r = addPositiveNumber(&absA, &absB, result);
        setSign(result, -1);
    } else {
        assert(FALSE); // this block cannot be reached
    }

    if (r == -1) {
        printf("add: overflow the result\n\n");
    }
    return r;
}

// result <- a - b
// Return  0 if successed
// Return -1 if underflow
int sub(const Number *a, const Number *b, Number *result) {
    int r; // return value
    bool plusA = (getSign(a) == 1);
    bool plusB = (getSign(b) == 1);
    Number absA;
    Number absB;
    getAbs(a, &absA);
    getAbs(b, &absB);

    // there are 4 cases
    //   1. +a - +b  =>     a  -  b    => sub(a, b)
    //   2. +a - -b  =>     a  + |b|   => add(a, abs(b))
    //   3. -a - +b  =>  -(|a| +  b )  => setSign(add(abs(a), b), -1)
    //   4. -a - -b  =>    |b| - |a|   => sub(abs(b), abs(a))
    if (plusA && plusB) {
        r = subPositiveNumber(a, b, result);
    } else if (plusA && !plusB) {
        r = addPositiveNumber(a, &absB, result);
    } else if (!plusA && plusB) {
        r = addPositiveNumber(b, &absA, result);
        setSign(result, -1);
    } else if (!plusA && !plusB) {
        r = subPositiveNumber(&absB, &absA, result);
    } else {
        assert(FALSE); // this block cannot be reached
    }

    if (r == -1) {
        printf("sub: underflow the result\n\n");
        return -1;
    }
    return 0;
}

// result <- num + 1
int increment(const Number *num, Number *result) {
    Number one;
    setInt(&one, 1);
    return add(num, &one, result);
}

// result <- num - 1
int decrement(const Number *num, Number *result) {
    Number one;
    setInt(&one, 1);
    return sub(num, &one, result);
}

// result <- a * b
// [deprecated]: This function is too slow to culculate the multiplication.
int slowMultiplePositiveNumber(const Number *a, const Number *b, Number *result) {
    Number tmp;
    Number i;
    copyNumber(a, result);
    for (setInt(&i, 1); compNumber(&i, b) < 0; ) {      // for (i = 1; i < b; )
        add(result, a, &tmp); copyNumber(&tmp, result); // result += a
        increment(&i, &tmp); copyNumber(&tmp, &i);      // i++
    }
    return 0;
}

// result <- a * b
//   where a >= 0, b >= 0
// Return  0 if success
// Return -1 if overflow
int multiplePositiveNumber(const Number *a, const Number *b, Number *result) {
    int i;
    int ai, bi;
    int aiMax, biMax;
    int carry = 0;
    int r; // return value
    Number tmp;
    Number tmpResult;
    clearByZero(result);
    clearByZero(&tmpResult);

    // to ignore prefix 0 digits
    for (i = KETA - 1; i != 0 && a->n[i-1] == 0; i--) ;
    aiMax = i;
    for (i = KETA - 1; i != 0 && b->n[i-1] == 0; i--) ;
    biMax = i;

    for (bi = 0; bi <= biMax; bi++) {
        Number d;
        clearByZero(&d);

        // if b_bi == 0
        if (b->n[bi] == 0) continue;

        // d_ai = a_ai * b_bi
        for (ai = 0; ai <= aiMax; ai++) {
            int mul = a->n[ai] * b->n[bi] + carry;
            d.n[ai] = mul % RADIX;
            carry   = mul / RADIX;
        }

        // d *= 10 ** (bi * RADIX_LEN)
        r = mulBy10E(bi * RADIX_LEN, &d, &tmp);
        if (r != 0) return -1; // overflow
        copyNumber(&tmp, &d);

        // tmpResult += d
        r = add(&tmpResult, &d, &tmp);
        if (r != 0) return -1; // overflow
        copyNumber(&tmp, &tmpResult);
    }

    copyNumber(&tmpResult, result);

    return 0;
}

// result <- a * b
// Return  0 if success
// Return -1 if overflow
int multiple(const Number *a, const Number *b, Number *result) {
    int r; // return value
    bool plusA = (getSign(a) == 1);
    bool plusB = (getSign(b) == 1);
    Number absA;
    Number absB;
    getAbs(a, &absA);
    getAbs(b, &absB);

    // there are 4 cases
    //   1. +a * +b  =>     a  *  b    => mul(a, b)
    //   2. +a * -b  =>  -( a  * |b|)  => setSign(mul(a, getAbs(b)), -1)
    //   3. -a * +b  =>  -(|a| *  b )  => setSign(mul(getAbs(a), b), -1)
    //   4. -a * -b  =>    |a| * |b|   => mul(getAbs(a), getAbs(b))
    if (plusA && plusB) {
        r = multiplePositiveNumber(a, b, result);
    } else if (plusA && !plusB) {
        r = multiplePositiveNumber(a, &absB, result);
        setSign(result, -1);
    } else if (!plusA && plusB) {
        r = multiplePositiveNumber(&absA, b, result);
        setSign(result, -1);
    } else if (!plusA && !plusB) {
        r = multiplePositiveNumber(&absB, &absA, result);
    } else {
        assert(FALSE); // this block cannot be reached
    }

    if (r == -1) {
        printf("multiple: underflow the result\n\n");
    }
    return r;
}

// a / b = q ... m
//   where a >= 0, b >= 0
// Return  0 if success
// Return -1 if division by zero
//
// [deprecated]: This function is too slow to culculate the division.
int slowDivmodPositiveNumber(const Number *_a, const Number *b, Number *q, Number *m) {
    Number tmp;
    Number a = *_a;
    Number k;
    clearByZero(&k);

    if (isZero(b)) return -1;

    while (1) {
        if (compNumber(&a, b) < 0) break;          // if (a < b) break;
        sub(&a, b, &tmp); copyNumber(&tmp, &a);    // a -= b
        increment(&k, &tmp); copyNumber(&tmp, &k); // k++
    }
    copyNumber(&k, q);
    copyNumber(&a, m);

    return 0;
}

// a / b = q ... m
//   where a >= 0, b >= 0
// Return  0 if success
// Return -1 if division by zero (b == 0)
int divmodPositiveNumber(const Number *a, const Number *b, Number *q, Number *m) {
    int i;
    int begin;
    Number tmp;
    Number num; // store a midway state of division
    Number divisor = *b;
    Number division, remain;
    Number result;
    Number next;
    bool useInt = FALSE;
    clearByZero(&num);
    clearByZero(&result);
    clearByZero(&next);

    if (isZero(b)) return -1;

    for (i = KETA - 1; a->n[i] == 0 && i >= 0; i--) {}
    begin = i;

    setInt(&num, a->n[begin]);

    while (i >= 0) {
        slowDivmodPositiveNumber(&num, &divisor, &division, &remain);

        result.n[i] = division.n[0];
        if (i == 0) break;

        setInt(&next, a->n[i-1]);

        if (division.n[0] != 0) {
            clearByZero(&num);
            add(&num, &remain, &tmp); copyNumber(&tmp, &num); // num += remain
        }

        mulBy10E(RADIX_LEN, &num, &tmp); copyNumber(&tmp, &num); // num *= 10
        add(&num, &next, &tmp); copyNumber(&tmp, &num); // num += next

        i--;
    }

    copyNumber(&result, q);
    copyNumber(&remain, m);
    return 0;
}

// a / b = quo ... mod
// Return  0 if success
// Return -1 if division by zero (b == 0)
int divmod(const Number *a, const Number *b, Number *quo, Number *mod) {
    int r = 0;
    bool plusA = (getSign(a) == 1);
    bool plusB = (getSign(b) == 1);
    Number absA;
    Number absB;
    getAbs(a, &absA);
    getAbs(b, &absB);

    // there are 4 cases
    //   1. +a / +b  =>     a  /  b    => div(a, b)
    //      +a % +b  =>     a  %  b    => mod(a, b)
    //   2. +a / -b  =>  -( a  / |b|)  => setSign(div(a, getAbs(b)), -1)
    //      +a % -b  =>     a  % |b|   =>         mod(a, getAbs(b))
    //   3. -a / +b  =>  -(|a| /  b )  => setSign(div(getAbs(a), b), -1)
    //      -a % +b  =>  -(|a| %  b )  => setSign(mod(getAbs(a), b), -1)
    //   4. -a / -b  =>    |a| / |b|   =>         div(getAbs(a), getAbs(b))
    //      -a % -b  =>  -(|a| % |b|)  => setSign(mod(getAbs(a), getAbs(b)), -1)
    if (plusA && plusB) {
        r = divmodPositiveNumber(a, b, quo, mod);
    } else if (plusA && !plusB) {
        r = divmodPositiveNumber(a, &absB, quo, mod);
        setSign(quo, -1);
    } else if (!plusA && plusB) {
        r = divmodPositiveNumber(&absA, b, quo, mod);
        setSign(quo, -1);
        setSign(mod, -1);
    } else if (!plusA && !plusB) {
        r = divmodPositiveNumber(&absA, &absB, quo, mod);
        setSign(mod, -1);
    } else {
        assert(FALSE); // this block cannot be reached
    }

    return r;
}

// c <- pow(a, b)
//   where b >= 0
// Return  0 if success
int power(const Number *a, const Number *b, Number *c) {
    Number tmp;
    Number i;
    Number zero, one;
    Number result;
    clearByZero(&zero);
    setInt(&one, 1);
    clearByZero(&i);
    clearByZero(&result);

    // pow(a, b) where a == 0
    if (compNumber(a, &zero) == 0) {
        setInt(c, 0);
        return 0;
    }

    // pow(a, b) where a == 1
    if (compNumber(a, &one) == 0) {
        setInt(c, 1);
        return 0;
    }

    // pow(a, b) where b == 0
    if (compNumber(b, &zero) == 0) {
        setInt(c, 1);
        return 0;
    }

    // pow(a, b)
    setInt(&result, 1);
    while (1) {
        if (compNumber(&i, b) >= 0) break;

        multiple(&result, a, &tmp); copyNumber(&tmp, &result); // result *= a
        increment(&i, &tmp); copyNumber(&tmp, &i); // i++
    }
    copyNumber(&result, c);

    return 0;
}

// isPrime(num) :: bool
//   where num >= 0
// Return TRUE  if num is prime
// Return FALSE if num is not prime
bool isPrime(const Number *_num) {
    Number _;
    Number tmp;
    Number zero, two;
    Number num = *_num;
    Number division;
    Number max;
    Number remain;
    setInt(&zero, 0);
    setInt(&two, 2);
    setInt(&division, 1);
    clearByZero(&remain);

    divmod(&num, &two, &max, &_);

    if (num.n[0] % 2 == 0) {
        return FALSE;
    }

    while (1) {
        add(&division, &two, &tmp); copyNumber(&tmp, &division); // division += 2

        if (compNumber(&division, &max) >= 0) break;

        divmod(&num, &division, &_, &remain);
        if (compNumber(&remain, &zero) == 0) {
            return FALSE;
        }
    }

    return TRUE;
}

// result <- num!
int factorialNumber(const Number *_num, Number *_result) {
    Number tmp;
    Number num = *_num;
    Number result;
    setInt(&result, 1); // result = 1
    while (!isZero(&num)) {
        multiple(&result, &num, &tmp); copyNumber(&tmp, &result); // result *= num
        decrement(&num, &tmp); copyNumber(&tmp, &num); // num--
    }
    copyNumber(&result, _result);
    return 0;
}

// c <- gcd(a, b)
int gcdNumber(const Number *a, const Number *b, Number *c) {
    Number _;
    Number num1 = *a;
    Number num2 = *b;
    Number remain;

    // num1 must be bigger than num2
    if (compNumber(&num1, &num2) > 0) {
        swapNumber(&num1, &num2);
    }

    while (1) {
        divmod(&num1, &num2, &_, &remain);
        // break if remain is zero
        if (isZero(&remain)) break;

        // shift
        copyNumber(&num2, &num1);
        copyNumber(&remain, &num2);
    }

    copyNumber(&num2, c);
    return 0;
}

// c <- lcm(a, b)
int lcmNumber(const Number *a, const Number *b, Number *c) {
    Number _;
    Number axb;    // = a * b
    Number gcd_ab; // = gcd(a, b)
    Number result;
    multiple(a, b, &axb);
    gcdNumber(a, b, &gcd_ab);
    divmod(&axb, &gcd_ab, &result, &_);
    copyNumber(&result, c);
    return 0;
}

// result <- sqrt(num)
//   where num >= 0
// Return  0 if success
// Return -1 if num < 0
int slowSqrtNumber(const Number *num, Number *result) {
    Number tmp;
    Number zero, two;
    Number subCount;
    Number N = *num;
    Number a;
    clearByZero(&zero);
    clearByZero(&subCount);
    setInt(&a, 1);
    setInt(&two, 2);

    // return -1 if num < 0
    if (compNumber(num, &zero) < 0) return -1;

    while (1) {
        if (compNumber(&N, &a) < 0) break;
        sub(&N, &a, &tmp); copyNumber(&tmp, &N); // N -= a
        add(&a, &two, &tmp); copyNumber(&tmp, &a); // a += 2
        increment(&subCount, &tmp); copyNumber(&tmp, &subCount); // subCount++
    }

    copyNumber(&subCount, result);
    return 0;
}

// result <- sqrt(num)
//   where num >= 0
// Return  0 if success
// Return -1 if num < 0
// Calculate sqrt with Newton-Raphson method.
int sqrtNumber(const Number *num, Number *result) {
    int i;
    Number _;
    Number tmp, tmp2;
    Number xNext, x, xPrev;
    Number two;

    copyNumber(num, &x);
    clearByZero(&xNext);
    setInt(&two, 2);

    divmod(&x, &two, &tmp, &_); copyNumber(&tmp, &x); // x = x / 2

    while (1) {
        // xPrev = x
        copyNumber(&x, &xPrev);
        // x = xNext
        copyNumber(&xNext, &x);

        // xNext = (x + (num / x)) / 2
        divmod(num, &x, &tmp, &_);
        add(&x, &tmp, &tmp2);
        divmod(&tmp2, &two, &xNext, &_);

        if (compNumber(&xNext, &x) == 0) break; // converge
        if (compNumber(&xNext, &xPrev) == 0) {  // oscillate
            if (compNumber(&x, &xNext) < 0) copyNumber(&x, &xNext);
        }
    }

    copyNumber(&xNext, result);
    return 0;
}
