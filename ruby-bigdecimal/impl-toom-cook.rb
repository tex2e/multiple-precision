
require 'bigdecimal'

u = 123456789009876
v = 678900987654321

puts "ans = #{u * v}"

RADIX_LEN = 5

def U(x)
  12345 * x**2 + 67890 * x +  9876
end

def V(x)
  67890 * x**2 +  9876 * x + 54321
end

# 分割数d = 3
# 必要な係数wの数 = 2d - 1

def W(x)
  h0_0 = U(0) * V(0)
  h0_1 = U(1) * V(1)
  h0_2 = U(2) * V(2)
  h0_3 = U(3) * V(3)
  h0_4 = U(4) * V(4)

  h1_0 = h0_1 - h0_0
  h1_1 = h0_2 - h0_1
  h1_2 = h0_3 - h0_2
  h1_3 = h0_4 - h0_3

  h2_0 = (h1_1 - h1_0) / 2
  h2_1 = (h1_2 - h1_1) / 2
  h2_2 = (h1_3 - h1_2) / 2

  h3_0 = (h2_1 - h2_0) / 3
  h3_1 = (h2_2 - h2_1) / 3

  h4_0 = (h3_1 - h3_0) / 4

  puts("#{h0_0}  #{h0_1}  #{h0_2}  #{h0_3}  #{h0_4}")
  puts("  #{h1_0}  #{h1_1}  #{h1_2}  #{h1_3}")
  puts("    #{h2_0}  #{h2_1}  #{h2_2}")
  puts("      #{h3_0}  #{h3_1}")
  puts("        #{h4_0}")
  puts
  puts("W(x) = #{h4_0} x^4 + #{h3_0} x^3 + #{h2_0} x^2 + #{h1_0} x + #{h0_0}")

  m = []
  m[0] = [h4_0, h3_0, h2_0, h1_0, h0_0]
  m[1] = [m[0][0], m[0][1] - 3 * m[0][0]]
  m[2] = [m[1][0], m[1][1] - 2 * m[1][0], m[0][2] - 2 * m[1][1]]
  m[3] = [m[2][0], m[2][1] - 1 * m[2][0], m[2][2] - 1 * m[2][1], m[0][3] - 1 * m[2][2]]
  m[4] = [m[3][0], m[3][1], m[3][2], m[3][3], m[0][4]]

  h4_0, h3_0, h2_0, h1_0, h0_0 = m[4]

  pp m

  h4_0 * x**4 + h3_0 * x**3 + h2_0 * x**2 + h1_0 * x + h0_0
end

result = W(10 ** RADIX_LEN)
puts "Toom-Cook result = #{result}"
puts "ok." if u * v == result


__END__
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
