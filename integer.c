
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
    digit_t carry;
    DIV_t divResult;
    clearByZero(result);

    // result = num if num == 0
    if (isZero(num)) {
        copyNumber(num, result);
        return 0;
    }

    if (num->n[KETA-1] / (RADIX / 10) != 0) {
        printf("mulBy10: overflow\n\n");
        return -1;
    }

    carry = 0;
    for (i = 0; i < KETA; i++) {
        divResult = DIV(num->n[i] * 10 + carry, RADIX);
        carry        = divResult.quot; // num.n[i] * 10 + carry / RADIX
        result->n[i] = divResult.rem;  // num.n[i] * 10 + carry % RADIX
    }

    setSign(result, getSign(num));
    return 0;
}

// result <- num * 1e+exponent
//   where num >= 0, exponent >= 0
// Return  0 if success
// Return -1 if exponent is less than 0
// Return -2 if overflow the result
int mulBy10E(int exponent, const Number *num, Number *result) {
    int i;
    int shiftCount, mulBy10Count;
    Number tmpResult;

    clearByZero(result);
    clearByZero(&tmpResult);

    if (exponent < 0) return -1;

    // result = num if exponent == 0 or num == 0
    if (exponent == 0 || isZero(num)) {
        copyNumber(num, result);
        return 0;
    }

    // Example: (exponent = 500, RADIX_LEN = 9)
    //   1. shift to left 55 times (which is equals to multiply by 10^{55*9})
    //   2. then, multiply each digit by 10^5.

    shiftCount   = exponent / RADIX_LEN;
    mulBy10Count = exponent % RADIX_LEN;

    if (shiftCount >= 1) {
        // shift to left
        for (i = KETA - 1; i >= shiftCount; i--) {
            tmpResult.n[i] = num->n[i - shiftCount];
        }
        for (i = shiftCount - 1; i >= 0; i--) {
            tmpResult.n[i] = 0;
        }
    } else {
        copyNumber(num, &tmpResult);
    }

    if (mulBy10Count >= 1) {
        // multiply each digit by 10^n
        digit_t carry = 0;
        DIV_t divResult;
        for (i = 0; i < KETA; i++) {
            divResult = DIV(tmpResult.n[i] * (digit_t)pow(10, mulBy10Count) + carry, RADIX);
            carry        = divResult.quot;
            result->n[i] = divResult.rem;
        }
    } else {
        copyNumber(&tmpResult, result);
    }

    setSign(result, getSign(num));
    return 0;
}

// result <- num / 10
// Return num % 10
int divBy10(const Number *num, Number *result) {
    int i;
    digit_t remain;
    DIV_t divResult;
    clearByZero(result);

    if (isZero(num)) {
        copyNumber(num, result);
        return 0;
    }

    remain = 0;
    for (i = KETA - 1; i >= 0; i--) {
        divResult = DIV(num->n[i], 10);
        result->n[i] = divResult.quot + remain * (RADIX / 10);
        remain       = divResult.rem;
    }

    setSign(result, getSign(num));

    return remain;
}

// result <- num / 1e+exponent
//   where num >= 0, exponent >= 0
// Return  0 if success
// Return -1 if exponent is less than 0
// Return -2 if overflow the result
int divBy10E(int exponent, const Number *num, Number *result) {
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
        divBy10(&tmpResult, &tmp); copyNumber(&tmp, &tmpResult);
    }
    copyNumber(&tmpResult, result);

    setSign(result, getSign(num));

    return 0;
}

// Swap a, b
void swapNumber(Number *a, Number *b) {
    Number tmp;
    tmp = *a; *a = *b; *b = tmp;
}

// Set int value to Number
// Return  0 if success
// Return -1 if cannot set the value x into the num
int setInt(Number *num, digit_t x) {
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
int getInt(const Number *num, digit_t *x) {
    int i;
    *x = 0;

    for (i = 0; i < KETA; i++) {
        *x += num->n[i] * (digit_t)pow(RADIX, i);
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
    for (i = KETA - 1; i != 0 && a->n[i] == 0 && b->n[i] == 0; i--) ;
    iMax = MIN(i + 1, KETA - 1);

    for (i = 0; i <= iMax; i++) {
        digit_t sum = a->n[i] + b->n[i] + carry;
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
    for (i = KETA - 1; i != 0 && a.n[i] == 0 && b.n[i] == 0; i--) ;
    iMax = i;

    // compute diff
    for (i = 0; i <= iMax; i++) {
        digit_t ai = a.n[i] - borrow;
        digit_t bi = b.n[i];
        digit_t diff;
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
    DIV_t divResult;
    Number tmp;
    Number tmpResult;
    clearByZero(result);
    clearByZero(&tmpResult);

    // to ignore prefix 0 digits
    for (i = KETA - 1; i != 0 && a->n[i] == 0; i--) ;
    aiMax = MIN(i + 1, KETA - 1);
    for (i = KETA - 1; i != 0 && b->n[i] == 0; i--) ;
    biMax = MIN(i + 1, KETA - 1);

    for (bi = 0; bi <= biMax; bi++) {
        Number d;
        clearByZero(&d);

        // if b_bi == 0
        if (b->n[bi] == 0) continue;

        // d_ai = a_ai * b_bi
        for (ai = 0; ai <= aiMax; ai++) {
            digit_t mul = a->n[ai] * b->n[bi] + carry;
            divResult = DIV(mul, RADIX);
            d.n[ai]   = divResult.rem;  // mul % RADIX
            carry     = divResult.quot; // mul / RADIX
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
//   where a >= 0, b >= 0
// Return  0 if success
// Return -1 if overflow
// Using Toom-Cook 3-way multiplication algorithm.
int multipleToomCook(const Number *a, const Number *b, Number *result) {
    int i, di, x_;
    int aiMax, biMax, iMax;
    int d1iMax, d2iMax, d3iMax, divisionLength;
    Number h0[5], h1[4], h2[3], h3[2], h4[1];
    Number m0[5], m1[2], m2[3], m3[4], w[5];
    Number tmp;
    Number x, ux, vx;
    Number a_d1, a_d2, a_d3;
    Number b_d1, b_d2, b_d3;
    Number two, three;
    digit_t _;
    setInt(&two, 2);
    setInt(&three, 3);
    clearByZero(result);

    // 分割数d = 3
    // 必要な係数wの数 = 2d - 1 = 5

    for (i = KETA - 1; i != 0 && a->n[i] == 0; i--) ;
    aiMax = MIN(i + 1, KETA - 1);
    for (i = KETA - 1; i != 0 && b->n[i] == 0; i--) ;
    biMax = MIN(i + 1, KETA - 1);
    iMax = MAX(aiMax, biMax);

    if (aiMax <= 3 || biMax <= 3) {
        return multiplePositiveNumber(a, b, result);
    }

    d1iMax = (int)ceil(iMax / 3.0);
    d2iMax = d1iMax * 2;
    d3iMax = iMax;
    divisionLength = d1iMax;

    clearByZero(&a_d1);
    clearByZero(&a_d2);
    clearByZero(&a_d3);
    clearByZero(&b_d1);
    clearByZero(&b_d2);
    clearByZero(&b_d3);
    for (di = i = 0; i < d1iMax; di++, i++) {
        a_d3.n[di] = a->n[i];
        b_d3.n[di] = b->n[i];
    }
    for (di = 0, i = d1iMax; i < d2iMax; di++, i++) {
        a_d2.n[di] = a->n[i];
        b_d2.n[di] = b->n[i];
    }
    for (di = 0, i = d2iMax; i <= d3iMax; di++, i++) {
        a_d1.n[di] = a->n[i];
        b_d1.n[di] = b->n[i];
    }

    for (x_ = 0; x_ < 5; x_++) {
        setInt(&x, x_);

        // ux = a_d1 * x^2 + a_d2 * x + a_d3
        //    = x (a_d1 x + a_d2) + a_d3
        copyNumber(&a_d1, &ux);
        multiple(&x, &ux, &tmp); copyNumber(&tmp, &ux);
        add(&ux, &a_d2, &tmp); copyNumber(&tmp, &ux);
        multiple(&x, &ux, &tmp); copyNumber(&tmp, &ux);
        add(&ux, &a_d3, &tmp); copyNumber(&tmp, &ux);

        // vx = b_d1 * x^2 + b_d2 * x + b_d3
        //    = x (b_d1 x + b_d2) + b_d3
        copyNumber(&b_d1, &vx);
        multiple(&x, &vx, &tmp); copyNumber(&tmp, &vx);
        add(&vx, &b_d2, &tmp); copyNumber(&tmp, &vx);
        multiple(&x, &vx, &tmp); copyNumber(&tmp, &vx);
        add(&vx, &b_d3, &tmp); copyNumber(&tmp, &vx);

        // h0[x] = ux * vx
        multiple(&ux, &vx, &h0[x_]);
    }

    sub(&h0[1], &h0[0], &h1[0]); // h1[0] = h0[1] - h0[0];
    sub(&h0[2], &h0[1], &h1[1]); // h1[1] = h0[2] - h0[1];
    sub(&h0[3], &h0[2], &h1[2]); // h1[2] = h0[3] - h0[2];
    sub(&h0[4], &h0[3], &h1[3]); // h1[3] = h0[4] - h0[3];
    sub(&h1[1], &h1[0], &tmp); divmodByInt(&tmp, 2, &h2[0], &_); // h2[0] = (h1[1] - h1[0])/2;
    sub(&h1[2], &h1[1], &tmp); divmodByInt(&tmp, 2, &h2[1], &_); // h2[1] = (h1[2] - h1[1])/2;
    sub(&h1[3], &h1[2], &tmp); divmodByInt(&tmp, 2, &h2[2], &_); // h2[2] = (h1[3] - h1[2])/2;
    sub(&h2[1], &h2[0], &tmp); divmodByInt(&tmp, 3, &h3[0], &_); // h3[0] = (h2[1] - h2[0])/3;
    sub(&h2[2], &h2[1], &tmp); divmodByInt(&tmp, 3, &h3[1], &_); // h3[1] = (h2[2] - h2[1])/3;
    sub(&h3[1], &h3[0], &tmp); divmodByInt(&tmp, 4, &h4[0], &_); // h4[0] = (h3[1] - h3[0])/4;

    // m0 = { h4[0], h3[0], h2[0], h1[0], h0[0] }
    copyNumber(&h4[0], &m0[0]);
    copyNumber(&h3[0], &m0[1]);
    copyNumber(&h2[0], &m0[2]);
    copyNumber(&h1[0], &m0[3]);
    copyNumber(&h0[0], &m0[4]);

    // m1 = { m0[0], m0[1] - 3*m0[0] }
    copyNumber(&m0[0], &m1[0]);
    multiple(&m0[0], &three, &tmp); sub(&m0[1], &tmp, &m1[1]);

    // m2 = { m1[0], m1[1] - 2*m1[0], m0[2] - 2*m1[1] }
    copyNumber(&m1[0], &m2[0]);
    multiple(&m1[0], &two, &tmp); sub(&m1[1], &tmp, &m2[1]);
    multiple(&m1[1], &two, &tmp); sub(&m0[2], &tmp, &m2[2]);

    // m3 = { m2[0], m2[1] - 1*m2[0], m2[2] - 1*m2[1], m0[3] - 1*m2[2] };
    copyNumber(&m2[0], &m3[0]);
    sub(&m2[1], &m2[0], &m3[1]);
    sub(&m2[2], &m2[1], &m3[2]);
    sub(&m0[3], &m2[2], &m3[3]);

    // w  = { m3[0], m3[1], m3[2], m3[3], m0[4] };
    copyNumber(&m3[0], &w[0]);
    copyNumber(&m3[1], &w[1]);
    copyNumber(&m3[2], &w[2]);
    copyNumber(&m3[3], &w[3]);
    copyNumber(&m0[4], &w[4]);

    //   w0 x^4 + w1 x^3 + w2 x^2 + w3 x + w4
    // = x (x (x (w0 x + w1) + w2) + w3) + w4   (x = RADIX = 10^RADIX_LEN)
    copyNumber(&w[0], result);
    mulBy10E(divisionLength * RADIX_LEN, result, &tmp); copyNumber(&tmp, result);
    add(result, &w[1], &tmp); copyNumber(&tmp, result);
    mulBy10E(divisionLength * RADIX_LEN, result, &tmp); copyNumber(&tmp, result);
    add(result, &w[2], &tmp); copyNumber(&tmp, result);
    mulBy10E(divisionLength * RADIX_LEN, result, &tmp); copyNumber(&tmp, result);
    add(result, &w[3], &tmp); copyNumber(&tmp, result);
    mulBy10E(divisionLength * RADIX_LEN, result, &tmp); copyNumber(&tmp, result);
    add(result, &w[4], &tmp); copyNumber(&tmp, result);

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
        r = multipleToomCook(a, b, result);
    } else if (plusA && !plusB) {
        r = multipleToomCook(a, &absB, result);
        setSign(result, -1);
    } else if (!plusA && plusB) {
        r = multipleToomCook(&absA, b, result);
        setSign(result, -1);
    } else if (!plusA && !plusB) {
        r = multipleToomCook(&absB, &absA, result);
    } else {
        assert(FALSE); // this block cannot be reached
    }

    if (r == -1) {
        printf("multiple: overflow the result\n\n");
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
    int i, j;
    int iMax, jMax;
    int next_;
    Number tmp;
    Number num; // store a midway state of division
    Number divisor = *b;
    Number division, remain;
    Number result;
    Number next;
    Number digitMax;
    clearByZero(&num);
    clearByZero(&result);
    clearByZero(&next);

    if (isZero(b)) return -1;

    if (compNumber(a, b) < 0) {
        setInt(q, 0);
        copyNumber(a, m);
        return 0;
    }

    // use divmodByInt if b < RADIX
    setInt(&digitMax, RADIX);
    if (compNumber(&divisor, &digitMax) < 0) {
        int r;
        digit_t divisor_;
        digit_t rem_;
        getInt(&divisor, &divisor_);
        r = divmodPositiveNumberByInt(a, divisor_, q, &rem_);
        setInt(m, rem_);
        return r;
    }

    // to ignore prefix 0 digits
    for (i = KETA - 1; a->n[i] == 0 && i >= 0; i--) {}
    iMax = i;

    jMax = RADIX_LEN - 1;

    // get first digit
    setInt(&num, a->n[iMax] / (digit_t)pow(10, jMax));

    for (i = iMax; i >= 0; i--) {
        for (j = jMax; j >= 0; j--) {
            // num / divisor = division ... remain
            // division is between 0 and 9.
            // So, using slow divmod algorithm at this time is not problem.
            slowDivmodPositiveNumber(&num, &divisor, &division, &remain);

            result.n[i] += division.n[0] * (digit_t)pow(10, j);

            // break if last digit
            if (i == 0 && j == 0) break;

            // get next (right) digit
            if (j == 0) {
                next_ = a->n[i-1] / (digit_t)pow(10, jMax);
            } else {
                next_ = a->n[i] / (digit_t)pow(10, j-1) % 10;
            }
            setInt(&next, next_);

            // num = remain * 10 + next
            if (division.n[0] != 0) {
                copyNumber(&remain, &num); // num = remain
            }
            mulBy10(&num, &tmp); copyNumber(&tmp, &num); // num *= 10
            add(&num, &next, &tmp); copyNumber(&tmp, &num); // num += next
        }
    }

    copyNumber(&result, q);
    copyNumber(&remain, m);
    return 0;
}

// quo <- a / b
// mod <- a % b
// Return  0 if success
// Return -1 if divided by zero (b == 0)
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

// a / b = q ... m
//   where a >= 0, b >= 0, b is instance of int
// Return  0 if success
// Return -1 if division by zero (b == 0)
int divmodPositiveNumberByInt(const Number *a, digit_t b, Number *q, digit_t *m) {
    int i, j;
    int begin;
    Number tmp;
    digit_t num; // store a midway state of division
    digit_t divisor = b;
    digit_t division, remain;
    digit_t next;
    Number result;
    num = 0;
    next = 0;
    clearByZero(&result);

    if (b == 0) return -1;

    // to ignore prefix 0 digits
    for (i = KETA - 1; a->n[i] == 0 && i >= 0; i--) {}
    begin = i;

    // get first digit
    num = a->n[begin];

    while (i >= 0) {
        DIV_t divResult = DIV(num, divisor);
        division = divResult.quot; // num / divisor
        remain   = divResult.rem;  // num % divisor

        result.n[i] = division;
        if (i == 0) break;

        // get next (right) digit
        next = a->n[i-1];

        // num = remain * 1e+RADIX_LEN + next
        num = remain * RADIX + next;

        // next
        i--;
    }

    copyNumber(&result, q);
    *m = remain;
    return 0;
}

// a / b = quo ... mod
// Return  0 if success
// Return -1 if divided by zero (b == 0)
int divmodByInt(const Number *a, digit_t b, Number *quo, digit_t *mod) {
    int r = 0;
    bool plusA = (getSign(a) == 1);
    bool plusB = (b >= 0);
    Number absA;
    digit_t absB;
    getAbs(a, &absA);
    absB = (plusB) ? b: -(b);

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
        r = divmodPositiveNumberByInt(a, b, quo, mod);
    } else if (plusA && !plusB) {
        r = divmodPositiveNumberByInt(a, absB, quo, mod);
        setSign(quo, -1);
    } else if (!plusA && plusB) {
        r = divmodPositiveNumberByInt(&absA, b, quo, mod);
        setSign(quo, -1);
        *mod *= -1;
    } else if (!plusA && !plusB) {
        r = divmodPositiveNumberByInt(&absA, absB, quo, mod);
        *mod *= -1;
    } else {
        assert(FALSE); // this block cannot be reached
    }

    return r;
}

// c <- pow(a, b)
//   where b >= 0
// Return  0 if success
int power(const Number *a, const Number *b, Number *c) {
    Number tmp, _;
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

    if (b->n[0] % 2 == 0) {
        Number a2; // a^2
        Number bHalf;
        Number two;
        setInt(&two, 2);
        multiple(a, a, &a2);
        divmod(b, &two, &bHalf, &_);
        return power(&a2, &bHalf, c); // pow(a^2, b/2)
    } else {
        Number bDecrement;
        Number res;
        decrement(b, &bDecrement);
        power(a, &bDecrement, &res); // res = pow(a, b-1)
        multiple(&res, a, &tmp); copyNumber(&tmp, c); // c = res * a
        return 0;
    }
}

// isPrime(num) :: bool
//   where num >= 0
// Return TRUE  if num is prime
// Return FALSE if num is not prime
bool isPrime(const Number *_num) {
    int step;
    Number _;
    Number tmp;
    Number zero, two, four;
    Number num = *_num;
    Number divisor;
    Number max;
    Number remain;
    setInt(&zero, 0);
    setInt(&two, 2);
    setInt(&four, 4);
    clearByZero(&remain);

    sqrtNumber(&num, &max);

    // 徐数列: 2,3,5,7,11,13,17,19,23,25,29,...
    // 最初の3項の次に 2 と 4 を交互に加えていったものを使用
    // 素数ではない 25,36,49 なども徐数列に含まれる

    setInt(&divisor, 2);
    divmod(&num, &divisor, &_, &remain);
    if (compNumber(&remain, &zero) == 0) return FALSE;

    setInt(&divisor, 3);
    divmod(&num, &divisor, &_, &remain);
    if (compNumber(&remain, &zero) == 0) return FALSE;

    setInt(&divisor, 5);
    divmod(&num, &divisor, &_, &remain);
    if (compNumber(&remain, &zero) == 0) return FALSE;

    step = 2;
    while (1) {
        if (compNumber(&divisor, &max) > 0) break;

        if (step == 2) {
            add(&divisor, &two, &tmp); copyNumber(&tmp, &divisor); // divisor += 2
            step = 4; // for next
        } else {
            add(&divisor, &four, &tmp); copyNumber(&tmp, &divisor); // divisor += 4
            step = 2; // for next
        }

        divmod(&num, &divisor, &_, &remain);
        if (compNumber(&remain, &zero) == 0) {
            return FALSE;
        }
    }

    return TRUE;
}

// factors <- factorize(60) == {2,2,3,5}
// Return factor count.
// Return -1 if factor count is bigger than maxFactors.
int factorizeNumber(const Number *_num, Number *factors, int maxFactors) {
    int step;
    int factor_i = 0;
    int prime_i = 0;
    int primes[] = {2,3,5};
    bool isPrimeNumber = TRUE;
    Number tmp;
    Number zero, one, two, four;
    Number num = *_num;
    Number divisor;
    Number max;
    Number quot, remain;
    setInt(&zero, 0);
    setInt(&one, 1);
    setInt(&two, 2);
    setInt(&four, 4);
    clearByZero(&remain);

    sqrtNumber(&num, &max);

    // 徐数列: 2,3,5,7,11,13,17,19,23,25,29,...
    // 最初の3項の次に 2 と 4 を交互に加えていったものを使用
    // 素数ではない 25,36,49 なども徐数列に含まれる

    for (prime_i = 0; prime_i < 3; prime_i++) {
        while (1) {
            setInt(&divisor, primes[prime_i]);
            divmod(&num, &divisor, &quot, &remain);
            if (compNumber(&remain, &zero) == 0) {
                if (factor_i == maxFactors) return -1;
                factors[factor_i++] = divisor;
                copyNumber(&quot, &num);
                isPrimeNumber = FALSE;
                continue;
            }
            break;
        }
    }

    step = 2;
    while (1) {
        if (compNumber(&divisor, &max) > 0) break;

        if (step == 2) {
            add(&divisor, &two, &tmp); copyNumber(&tmp, &divisor); // divisor += 2
            step = 4; // for next
        } else {
            add(&divisor, &four, &tmp); copyNumber(&tmp, &divisor); // divisor += 4
            step = 2; // for next
        }

        if (divisor.n[0] % 5 == 0) continue;

        divmod(&num, &divisor, &quot, &remain);
        if (compNumber(&remain, &zero) == 0) {
            if (factor_i == maxFactors) return -1;
            factors[factor_i++] = divisor;
            copyNumber(&quot, &num);
            isPrimeNumber = FALSE;
            continue;
        }
    }

    if (!isPrimeNumber && compNumber(&num, &one) != 0) {
        if (factor_i == maxFactors) return -1;
        factors[factor_i++] = num;
    }

    return factor_i;
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
// Calculate sqrt(N) with Newton-Raphson method.
//   sqrt(N) = x  ->  x^2 = N  ->  x^2 - N = 0
//   f(x)  = x^2 - N
//   f'(x) = 2x
//   Root-finding algorithm:
//     x_{i+1} = x_i - f(x_i) / f'(x_i)
//             = x_i - (x_i^2 - N) / (2 x_i)
//             = ((2 x_i^2) / (2 x_i)) - (x_i^2 / (2 x_i)) + (N / (2 x_i))
//             = (x_i^2 / (2 x_i)) + (N / (2 x_i))
//             = (x_i / 2) + (N / (2 x_i))
//             = (x_i + N / x_i) / 2
//
int sqrtNumber(const Number *num, Number *result) {
    int i;
    Number _;
    Number tmp, tmp2;
    Number xNext, x, xPrev;
    Number two;
    digit_t rem;
    copyNumber(num, &x);
    clearByZero(&xNext);
    setInt(&two, 2);

    divmod(&x, &two, &tmp, &_); copyNumber(&tmp, &x); // x = x / 2
    copyNumber(&x, &xPrev); // xPrev = x

    while (1) {
        // xNext = (x + (num / x)) / 2
        divmod(num, &x, &tmp, &_);
        add(&x, &tmp, &tmp2);
        divmodByInt(&tmp2, 2, &xNext, &rem);

        if (compNumber(&xNext, &x) == 0) break; // converge
        if (compNumber(&xNext, &xPrev) == 0) {  // oscillate
            if (compNumber(&x, &xNext) < 0) copyNumber(&x, &xNext);
            break;
        }

        // next
        copyNumber(&x, &xPrev); // xPrev = x
        copyNumber(&xNext, &x); // x = xNext
    }

    copyNumber(&xNext, result);
    return 0;
}

// result <- 1/num * 1e+expo
// Return expo where expo > prec
// Calculate 1/N with Newton-Raphson method.
//   1/N = x  ->  1/x - N = 0
//   f(x)  = 1/x - N
//   f'(x) = -1/(x)^2
//   Root-finding algorithm:
//     x_{n+1} = x_i - f(x_i) / f'(x_i)
//             = x_n (2 - N (x_n)^2)
//
int inverseNumber(const Number *num, int prec, Number *result) {
    int i, maxKeta;
    int left, right;
    int expo;
    Number tmp;
    Number x, xNext;
    Number term, term1, term2;
    Number two;
    setInt(&two, 2);
    clearByZero(result);

    for (i = KETA - 1; num->n[i] == 0 && i >= 0; i--) {}
    if (i < 0) return -1;
    maxKeta = i;
    expo = maxKeta * RADIX_LEN + log10(num->n[i]) + 1;

    setInt(&x, 2);

    int validDigits = 3; // TODO: valid digits may vary. this is not good

    while (1) {
        // x_{n+1} = x_n (2 - N x_n)
        multiple(num, &x, &tmp); copyNumber(&tmp, &term2);
        mulBy10E(expo, &two, &term1);
        sub(&term1, &term2, &term);
        multiple(&x, &term, &xNext);
        // printf("xNext_%d = ", i++); dispNumberZeroSuppress(&xNext); putchar('\n');

        if (validDigits > prec) break;

        copyNumber(&xNext, &x);

        // next
        expo *= 2;
        validDigits *= 2;
    }
    divBy10E(expo * 2 - prec, &xNext, result);

    return 0;
}

// result <- arctan(1/x) * 1e+digits
//
// Example: (digits = 1100)
//
//           ans = arctan(1/x)
//           ans = 1/1(1/x)^1 - 1/3(1/x)^3 + 1/5(1/x)^5 - 1/7(1/x)^7 + ...
//   10^1100 ans = 10^1100/1(1/x)^1 - 10^1100/3(1/x)^3 + 10^1100/5(1/x)^5 - ...
//
int arctan(int x, int digits, Number *result) {
    int i;
    int sign;
    Number tmp, _;
    Number x_;
    Number step, quot;
    Number numer, denom; // numer/denom
    Number denom1, denom2;
    Number sum;
    Number one, two;
    setInt(&one, 1);
    setInt(&two, 2);
    clearByZero(&sum);
    clearByZero(result);

    mulBy10E(digits, &one, &numer); // numer = 1e+digits

    sign = 1;
    setInt(&x_, x);
    multiple(&x_, &x_, &step); // step = x * x
    setInt(&denom1, 1);
    setInt(&denom2, x);
    copyNumber(&denom2, &denom); // denom = 1 * x
    while (1) {
        if (compNumber(&denom, &numer) > 0) break;
        multiple(&denom1, &denom2, &denom); // denom = denom1 * denom2
        divmod(&numer, &denom, &quot, &_); // quot = numer / denom
        setSign(&quot, sign);
        add(&sum, &quot, &tmp); copyNumber(&tmp, &sum); // sum += quot
        // next
        sign *= -1;
        add(&denom1, &two, &tmp); copyNumber(&tmp, &denom1); // denom1 += 2
        multiple(&denom2, &step, &tmp); copyNumber(&tmp, &denom2); // denom *= step
    }

    copyNumber(&sum, result);

    return 0;
}
