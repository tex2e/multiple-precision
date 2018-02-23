
a = 123_450_000_000_000
b = 6_789_000_000

a = + 109739200_000000000_000000000_000000000
b = + 104876113_635753874

RADIX = 1000000000

def divmod_kunthD(_a, _b, radix:)
  a, b = _a, _b

  radix_len = Math.log10(radix).to_i
  as = a.to_s.reverse.scan(/.{1,#{radix_len}}/)
    .map(&:reverse).map(&:to_i)
  bs = b.to_s.reverse.scan(/.{1,#{radix_len}}/)
    .map(&:reverse).map(&:to_i)
  qs = [] # ans
  s = as.length - 1
  t = bs.length - 1

  k = radix / (bs[t] + 1)
  a *= k
  b *= k
  puts "k = #{k}"

  isFirst = 1
  while 1
    as = a.to_s.reverse.scan(/.{1,#{radix_len}}/)
      .map(&:reverse).map(&:to_i)
    bs = b.to_s.reverse.scan(/.{1,#{radix_len}}/)
      .map(&:reverse).map(&:to_i)
    s = as.length - 1 + isFirst
    t = bs.length - 1
    as.append(0)
    puts "as = #{as}, bs = #{bs}, s = #{s}, t = #{t}"
    u = (as[s] < bs[t]) ? s - t - 1 : s - t;
    qh, rh = (as[s] * radix + as[s-1]).divmod( bs[t] )

    # q'b[t-1] > Dr' + a[s-2]
    puts "adjustment"
    puts "u = #{u}, qh = #{qh}, rh = #{rh}"
    puts "left = #{qh * bs[t-1]}, right = #{radix * rh + as[s-2]}"
    while rh < radix && qh * bs[t-1] > radix * rh + as[s-2]
      qh -= 1
      rh += bs[t]
      puts "u = #{u}, qh = #{qh}, rh = #{rh}"
      puts "left = #{qh * bs[t-1]}, right = #{radix * rh + as[s-2]}"
    end

    qs[u] = qh
    a -= b * qh * radix**u
    puts "aNext = #{a}"
    if a < 0
      qs[u] -= 1
      a += b * radix**u
      puts "adjusted aNext = #{a}"
    end

    if u <= 0
      return [qs.reverse, a / k]
    end

    isFirst = 0
  end
end

res = divmod_kunthD(a, b, radix: RADIX)
puts "result   = #{res}"
puts "expected = #{a.divmod(b)}"


__END__
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
    clearByZero(m);

    if (isZero(&b)) return -1;

    if (compNumber(&a, &b) < 0) {
        setInt(q, 0);
        copyNumber(&a, m);
        return 0;
    }

    // use divmodByInt if b < RADIX
    setInt(&digitMax, RADIX);
    if (compNumber(&b, &digitMax) < 0) {
        int r;
        digit_t divisor_;
        digit_t rem_;
        getInt(&b, &divisor_);
        r = divmodPositiveNumberByInt(_a, divisor_, q, &rem_);
        setInt(m, rem_);
        return r;
    }

    for (i = KETA - 1; a.n[i] == 0 && i >= 0; i--) {}
    s = i; // aiMax
    for (i = KETA - 1; b.n[i] == 0 && i >= 0; i--) {}
    t = i; // biMax

    k_ = RADIX / (1 + b.n[t]);
    setInt(&k, k_);
    multiple(&a, &k, &tmp); copyNumber(&tmp, &a);
    multiple(&b, &k, &tmp); copyNumber(&tmp, &b);

    // printf("k = "); dispNumberZeroSuppress(&k); putchar('\n');
    // printf("a = "); dispNumberZeroSuppress(&a); putchar('\n');

    // Calculate quotient.
    while (1) {
        for (i = KETA - 1; a.n[i] == 0 && i >= 0; i--) {}
        s = i; // aiMax

        // printf("a = "); dispNumberZeroSuppress(&a); putchar('\n');
        // printf("b = "); dispNumberZeroSuppress(&b); putchar('\n');
        // printf("s = %d, t = %d\n", s, t);

        u = (a.n[s] < b.n[t]) ? s - t - 1 : s - t;
        divResult = DIV(a.n[s] * RADIX + a.n[s-1], b.n[t]);
        qh_ = divResult.quot; // qh = (a.n[s] * RADIX + a.n[s-1]) / b.n[t];
        rh_ = divResult.rem;  // qh = (a.n[s] * RADIX + a.n[s-1]) % b.n[t];

        // q' b[t-1] > D r' + a[s-2]
        // printf("adjustment\n");
        // printf("u = %d, qh = %ld, rh = %ld\n", u, qh_, rh_);
        while (qh_ * b.n[t-1] > RADIX * rh_ + a.n[s-2]) {
            qh_ -= 1;
            rh_ += b.n[t];
            // printf("u = %d, qh = %ld, rh = %ld\n", u, qh_, rh_);
        }

        q->n[u] = qh_;
        // printf("u = %d, qh = %ld\n", u, qh_);
        // printf("q = "); dispNumberZeroSuppress(q); putchar('\n');

        // a -= b * qh * radix^u
        Number b_radix_u; // b * radix^u
        mulBy10E(u * RADIX_LEN, &one, &term);
        multiple(&b, &term, &b_radix_u);

        setInt(&qh, qh_);
        multiple(&b_radix_u, &qh, &term);
        sub(&a, &term, &tmp); copyNumber(&tmp, &a);
        // printf("aNext = "); dispNumberZeroSuppress(&a); putchar('\n');
        if (getSign(&a) == -1) {
            q->n[u] -= 1;
            add(&a, &b_radix_u, &tmp); copyNumber(&tmp, &a);
            // printf("adjusted aNext = "); dispNumberZeroSuppress(&a); putchar('\n');
        }
        if (getSign(&a) == -1) {
            fprintf(stderr, "divmodKunthAlgorithmD: something get wrong.\n");
            return -1;
        }

        // printf("u = %d\n", u);
        if (u <= 0) break;
        // puts("---");
    }

    // Calculate remainder.
    divmodPositiveNumberByInt(&a, k_, m, &rem_);

    return 0;
}
