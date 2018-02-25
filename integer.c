
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

// a / b = q ... m
//   where a >= 0, b >= 0
// Return  0 if success
// Return -1 if division by zero (b == 0)
int divmodKunthAlgorithmD(const Number *_a, const Number *_b, Number *q, Number *m) {
    int i;
    int s, t;
    int u;
    digit_t k_, qh_, rh_;
    digit_t rem_;
    Number tmp, _;
    Number a = *_a;
    Number b = *_b;
    Number k, radix;
    Number numer, denom; // numer/denom
    Number qh, rh;
    Number leftside, rightside;
    Number term, term1, term2;
    Number one;
    Number digitMax;
    DIV_t divResult;

    setInt(&radix, RADIX);
    setInt(&one, 1);
    clearByZero(q);

    if (isZero(&b)) return -1;

    if (compNumber(&a, &b) < 0) {
        setInt(q, 0);
        copyNumber(&a, m);
        return 0;
    }

    for (i = KETA - 1; a.n[i] == 0 && i >= 0; i--) {}
    s = i; // aiMax
    for (i = KETA - 1; b.n[i] == 0 && i >= 0; i--) {}
    t = i; // biMax

    // use divmodByInt if b < RADIX^2
    if (t <= 1) {
        int r;
        digit_t divisor_;
        digit_t rem_;
        getInt(&b, &divisor_);
        r = divmodPositiveNumberByInt(_a, divisor_, q, &rem_);
        setInt(m, rem_);
        return r;
    }

    k_ = RADIX / (1 + b.n[t]);
    setInt(&k, k_);
    multiple(&a, &k, &tmp); copyNumber(&tmp, &a);
    multiple(&b, &k, &tmp); copyNumber(&tmp, &b);

    // printf("\n\n=====\n");
    // printf("k = "); dispNumberZeroSuppress(&k); putchar('\n');
    // printf("a = "); dispNumberZeroSuppress(&a); putchar('\n');

    // Calculate quotient.
    int isFirst = 1;
    while (1) {
        for (i = KETA - 1; a.n[i] == 0 && i >= 0; i--) {}
        s = i; // aiMax
        s += isFirst;

        // printf("a = "); dispNumberZeroSuppress(&a); putchar('\n');
        // printf("b = "); dispNumberZeroSuppress(&b); putchar('\n');
        // printf("s = %d, t = %d\n", s, t);

        u = (a.n[s] < b.n[t]) ? s - t - 1 : s - t;
        // printf("qh, rh = divmod(%ld, %ld)\n", a.n[s] * RADIX + a.n[s-1], b.n[t]);
        divResult = DIV(a.n[s] * RADIX + a.n[s-1], b.n[t]);
        qh_ = divResult.quot; // qh = (a.n[s] * RADIX + a.n[s-1]) / b.n[t];
        rh_ = divResult.rem;  // rh = (a.n[s] * RADIX + a.n[s-1]) % b.n[t];

        // q' b[t-1] > D r' + a[s-2]
        // printf("adjustment\n");
        // printf("u = %d, qh = %ld, rh = %ld\n", u, qh_, rh_);
        while (rh_ < RADIX && qh_ * b.n[t-1] > RADIX * rh_ + a.n[s-2]) {
            qh_ -= 1;
            rh_ += b.n[t];
            // printf("u = %d, qh = %ld, rh = %ld\n", u, qh_, rh_);
        }

        q->n[u] = qh_;
        // printf("u = %d, qh = %ld\n", u, qh_);
        // printf("q = "); dispNumberZeroSuppress(q); putchar('\n');

        // a -= b * qh * radix^u
        Number b_radix_u; // b * radix^u
        mulBy10E(u * RADIX_LEN, &b, &b_radix_u);

        setInt(&qh, qh_);
        multiple(&b_radix_u, &qh, &term);
        sub(&a, &term, &tmp); copyNumber(&tmp, &a);
        if (getSign(&a) == -1) {
            q->n[u] -= 1;
            // printf("aNext = "); dispNumberZeroSuppress(&a); putchar('\n');
            add(&a, &b_radix_u, &tmp); copyNumber(&tmp, &a);
            // printf("adjusted aNext = "); dispNumberZeroSuppress(&a); putchar('\n');
        }
        if (getSign(&a) == -1) {
            fprintf(stderr, "divmodKunthAlgorithmD: something get wrong.\n");
            printf("a = "); dispNumberZeroSuppress(_a); putchar('\n');
            printf("b = "); dispNumberZeroSuppress(_b); putchar('\n');
            puts("---");
            exit(1);
            return -1;
        }

        // printf("u = %d\n", u);
        if (u <= 0) break;
        // puts("---");
        isFirst = 0;
    }

    // Calculate remainder.
    divmodPositiveNumberByInt(&a, k_, m, &rem_);

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
    Number quoRes, modRes;
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
        r = divmodKunthAlgorithmD(a, b, &quoRes, &modRes);
    } else if (plusA && !plusB) {
        r = divmodKunthAlgorithmD(a, &absB, &quoRes, &modRes);
        setSign(&quoRes, -1);
    } else if (!plusA && plusB) {
        r = divmodKunthAlgorithmD(&absA, b, &quoRes, &modRes);
        setSign(&quoRes, -1);
        setSign(&modRes, -1);
    } else if (!plusA && !plusB) {
        r = divmodKunthAlgorithmD(&absA, &absB, &quoRes, &modRes);
        setSign(&modRes, -1);
    } else {
        assert(FALSE); // this block cannot be reached
    }
    copyNumber(&quoRes, quo);
    copyNumber(&modRes, mod);

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
bool isPrime(const Number *num) {
    Number factors[1];
    int r = factorizeNumber(num, factors, 0);
    // r == 0  means there is no prime factors in num. Therefore it's a prime number.
    // r == -1 means factorizeNumber found a prime factor but cannot store in +factors+.
    return (r == 0);
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

    sqrtNumberInt(&num, &max);

    // 徐数列: 2,3,5,7,11,13,17,19,23,25,29,...
    // 最初の3項の次に 2 と 4 を交互に加えていったものを使用
    // 素数ではない 25,36,49 なども徐数列に含まれる
    // 5の倍数を除くと 20% の計算時間の節約

    for (prime_i = 0; prime_i < 3; prime_i++) {
        while (1) {
            setInt(&divisor, primes[prime_i]);
            divmod(&num, &divisor, &quot, &remain);
            if (compNumber(&remain, &zero) == 0) {
                if (factor_i >= maxFactors) return -1;
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
int sqrtNumberInt(const Number *num, Number *result) {
    Number _;
    Number tmp, tmp2;
    Number xNext, x, xPrev;
    Number two;
    copyNumber(num, &x);
    clearByZero(&xNext);
    setInt(&two, 2);

    divmod(&x, &two, &tmp, &_); copyNumber(&tmp, &x); // x = x / 2
    copyNumber(&x, &xPrev); // xPrev = x

    while (1) {
        // xNext = (x + (num / x)) / 2
        divmod(num, &x, &tmp, &_);
        add(&x, &tmp, &tmp2);
        divmod(&tmp2, &two, &xNext, &_);

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

// result <- sqrt(num)
//   where num >= 0
// Return  0 if success
// Return -1 if num < 0
// Calculate sqrt(N) with Newton-Raphson method.
int sqrtNumber(const Number *num, int prec, Number *result) {
    int i;
    int numLength, numd2Length;
    Number tmp, _;
    Number inverseNum;

    for (i = KETA - 1; num->n[i] == 0 && i > 0; i--) {}
    numLength = i * RADIX_LEN + log10(num->n[i]) + 1; // num's digit length
    numd2Length = ceil(numLength / 2.0);

    inverseSqrtNumber(num, prec, &inverseNum);
    multiple(num, &inverseNum, result);
    return numd2Length;
}

// result <- 1/sqrt(num) * 1e+prec
//   where num >= 0
// Return  0 if success
// Return -1 if num < 0
// Calculate 1/sqrt(N) with Newton-Raphson method.
//   f(x)  = 1/x^2 - N
//   f'(x) = -2/(x)^3
//   Root-finding algorithm:
//     x_{i+1} = x_i - f(x_i) / f'(x_i)
//             = (x_i / 2) (3 - N (x_i)^2)
//
int inverseSqrtNumber(const Number *num, int prec, Number *result) {
    int i;
    int xNextLength, shift;
    int numLength;
    Number tmp, _;
    Number term;
    Number x, xNext, xPrev;
    Number two, three, bigTwo, bigThree;
    Number x2, nx2;
    Number n = *num;
    Number tmp1, tmp2, tmp3, tmp4;

    for (i = KETA - 1; num->n[i] == 0 && i > 0; i--) {}
    numLength = i * RADIX_LEN + ceil(log10(num->n[i])); // num's digit length

    setInt(&two, 2);
    setInt(&three, 3);

    mulBy10E(prec, &three, &bigThree); // 3 * 10^prec
    mulBy10E(prec - numLength - 1, &two, &x); // 2 * 10^(prec - num_length - 1)

    while (1) {
        // printf("%2d) ", j++); dispNumberZeroSuppress(&x); putchar('\n');

        // nx2 = (&n * x**2) / 10**prec
        multiple(&x, &x, &x2);
        multiple(&n, &x2, &nx2);
        divBy10E(prec, &nx2, &tmp); copyNumber(&tmp, &nx2);

        // xNext = (x * (3 * 10**prec - nx2) / 2) / 10**prec
        sub(&bigThree, &nx2, &tmp1);
        multiple(&tmp1, &x, &tmp2); // tmp1 *= x
        divmod(&tmp2, &two, &tmp3, &_); // term /= 2
        divBy10E(prec, &tmp3, &xNext);

        if (compNumber(&xNext, &x) == 0) break; // converge

        // next
        copyNumber(&xNext, &x);
    }

    copyNumber(&xNext, result);
    return ceil(numLength / 2.0);
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
