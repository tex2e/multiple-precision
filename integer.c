
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <string.h>
#include <assert.h>
#include "integer.h"

// clear by zero (n[..] = 0, sign = 1)
void clearByZero(Number *num) {
    int i;
    for (i = 0; i < KETA; i++) {
        num->n[i] = 0;
    }
    setSign(num, 1);
}

// deep copy to given var
void copyNumber(const Number *from, Number *to) {
    *to = *from;
}

// display number
// + 0 0 0 0 0 1 2 3 4 5
void dispNumber(const Number *num) {
    int i;
    char sign = (getSign(num) < 0) ? '-': '+';

    printf("%c", sign);
    for (i = KETA - 1; i >= 0; i--) {
        printf("%2d", num->n[i]);
    }
}

// display number by suppress zero
// + 1 2 3 4 5
void dispNumberZeroSuppress(const Number *num) {
    int i;
    char sign = (getSign(num) < 0) ? '-': '+';

    printf("%c", sign);

    // skip digits that's zero
    for (i = KETA - 1; num->n[i] == 0 && i != 0; i--)
        ;

    for (; i >= 0; i--) {
        printf("%2d", num->n[i]);
    }
}

// set random number
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
        num->n[i] = random() % 10;
    }
    sign = (random() % 2 == 0) ? 1: -1;
    setSign(num, sign);
}

// absNum <- |num|
void getAbs(const Number *num, Number *absNum) {
    copyNumber(num, absNum);
    setSign(absNum, 1);
}

// check if given number is zero
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
    clearByZero(result);

    // result = num where num == 0
    if (isZero(num)) {
        copyNumber(num, result);
        return 0;
    }

    if (num->n[KETA-1] != 0) {
        printf("mulBy10: overflow\n\n");
        return -1;
    }

    for (i = KETA - 2; i >= 0; i--) {
        result->n[i+1] = num->n[i];
    }
    result->n[0] = 0;

    setSign(result, getSign(num));

    return 0;
}

// result <- num * 10e+exp
//   where num >= 0, exp >= 0
// Return  0 if success
// Return -1 if exp is less than 0
// Return -2 if overflow the result
int mulBy10E(int exp, const Number *num, Number *result) {
    int i;
    clearByZero(result);

    if (exp < 0) return -1;

    // result = num where exp == 0 or num == 0
    if (exp == 0 || isZero(num)) {
        copyNumber(num, result);
        return 0;
    }

    for (i = KETA - 1; i >= KETA - exp; i--) {
        if (num->n[i] != 0) {
            printf("mulBy10E: overflow\n\n");
            return -2;
        }
    }

    for (i = KETA - 1 - exp; i >= 0; i--) {
        result->n[i+exp] = num->n[i];
    }

    for (i = exp - 1; i >= 0; i--) {
        result->n[i] = 0;
    }

    setSign(result, getSign(num));

    return 0;
}

// result <- num / 10
// Return num % 10
int divBy10(const Number *num, Number *result) {
    int i;
    int remain;
    clearByZero(result);

    if (isZero(num)) {
        copyNumber(num, result);
        return 0;
    }

    remain = num->n[0];

    for (i = 1; i < KETA; i++) {
        result->n[i-1] = num->n[i];
    }
    result->n[KETA-1] = 0;

    setSign(result, getSign(num));

    return remain;
}

// Number a, b = b, a
void swapNumber(Number *a, Number *b) {
    Number tmp;
    tmp = *a; *a = *b; *b = tmp;
}

// set int value to integer
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
        num->n[i++] = x % 10;
        // break if finish
        if (x / 10 == 0) break;
        x /= 10;
    }
    return 0;
}

// get int value from integer
// Return  0 if success
// Return -1 if digit number of integer is out of int (DEVELOPMENT)
int getInt(const Number *num, int *x) {
    int i;
    *x = 0;

    for (i = 0; i < KETA; i++) {
        *x += num->n[i] * (int)pow(10.0, i);
    }
    if (getSign(num) == -1) {
        *x *= -1;
    }
    return 0;
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

// compare a given 2 numbers
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
    int i;
    int carry = 0;
    clearByZero(result);

    for (i = 0; i < KETA; i++) {
        int sum = a->n[i] + b->n[i] + carry;
        result->n[i] = sum % 10;
        carry = (sum >= 10) ? 1: 0;
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
    int i;
    int borrow = 0;
    Number a, b;

    clearByZero(result);

    // a must be bigger than b
    // if _a < _b
    if (compNumber(_a, _b) < 0) {
        a = *_b;
        b = *_a;
        setSign(result, -1); // flag negative
    } else {
        a = *_a;
        b = *_b;
    }

    // compute diff
    for (i = 0; i < KETA; i++) {
        int ai = a.n[i] - borrow;
        int bi = b.n[i];
        int diff;
        if (ai >= bi) {
            borrow = 0;
            diff = ai - bi;
        } else {
            borrow = 1;
            diff = 10 + ai - bi;
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
// this function is too slow to culculate the multiplication.
// the logic is same as below regarded the type Number as int.
//
//     int slowSimpleMultiple(int a, int b, int *result) {
//         int i;
//         *result = a;
//         for (i = 1; i < b; i++) {
//             *result += a;
//         }
//         return 0;
//     }
//
int slowMultiplePositiveNumber(const Number *a, const Number *b, Number *result) {
    Number tmp;
    Number i;
    copyNumber(a, result);
    for (setInt(&i, 1); compNumber(&i, b) < 0; ) {
        add(result, a, &tmp); copyNumber(&tmp, result); // result += a
        increment(&i, &tmp); copyNumber(&tmp, &i); // i++
    }
    return 0;
}

// result <- a * b
//   where a >= 0, b >= 0
// Return  0 if success
// Return -1 if overflow
int multiplePositiveNumber(const Number *a, const Number *b, Number *result) {
    int ai, bi;
    int carry = 0;
    int r; // return value
    Number tmp;
    Number tmpResult;
    clearByZero(result);
    clearByZero(&tmpResult);

    for (bi = 0; bi < KETA-1; bi++) {
        Number d;
        clearByZero(&d);

        // if b_bi == 0
        if (b->n[bi] == 0) continue;

        // d_ai = a_ai * b_bi
        for (ai = 0; ai < KETA-1; ai++) {
            int mul = a->n[ai] * b->n[bi] + carry;
            d.n[ai] = mul % 10;
            carry   = mul / 10;
        }

        // d *= 10 ** bi
        r = mulBy10E(bi, &d, &tmp);
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
// this function is too slow to culculate the division.
// the logic is same as below regarded the type Number as int.
//
//     int slowDivmod(int x, int y, int *q, int *m) {
//         int k;
//         if (y == 0) return -1;
//         k = 0;
//         while (1) {
//             if (x < y) break;
//             x -= y;
//             k++;
//         }
//         *q = k;
//         *m = x;
//         return 0;
//     }
//
int slowDivmodPositiveNumber(const Number *_a, const Number *b, Number *q, Number *m) {
    Number tmp;
    Number a = *_a;
    Number k;
    clearByZero(&k);

    if (isZero(b)) return -1;

    while (1) {
        if (compNumber(&a, b) < 0) break;
        sub(&a, b, &tmp); copyNumber(&tmp, &a); // a -= b
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
    Number division, remain;
    Number result;
    Number next;
    clearByZero(&num);
    clearByZero(&result);
    clearByZero(&next);

    if (isZero(b)) return -1;

    for (i = KETA - 1; a->n[i] == 0 && i >= 0; i--) {}
    begin = i;

    setInt(&num, a->n[begin]);

    while (i >= 0) {
        slowDivmodPositiveNumber(&num, b, &division, &remain);

        result.n[i] = division.n[0];
        if (i == 0) break;

        setInt(&next, a->n[i-1]);

        // isZero(&division)
        if (division.n[0] != 0) {
            clearByZero(&num);
            add(&num, &remain, &tmp); copyNumber(&tmp, &num); // num += remain
        }

        mulBy10(&num, &tmp); copyNumber(&tmp, &num); // num *= 10
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
    setInt(&division, 3);
    clearByZero(&remain);

    divmod(&num, &two, &max, &_);

    if (num.n[0] % 2 == 0) {
        return FALSE;
    }

    while (1) {
        if (compNumber(&division, &max) >= 0) break;

        divmod(&num, &division, &_, &remain);
        if (compNumber(&remain, &zero) == 0) {
            return FALSE;
        }

        add(&division, &two, &tmp); copyNumber(&tmp, &division); // division += 2
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
