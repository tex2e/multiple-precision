
# FIXME: when radix >= 1000 something go wrong

a = 123_450_000_000_000
b = 6_789_000_000

RADIX = 10

def fast_divmod(_a, _b, radix:)
  a, b = _a, _b

  radix_len = Math.log10(radix).to_i
  as = a.to_s.reverse.scan(/.{1,#{radix_len}}/)
    .map(&:reverse).map(&:to_i).reverse
  bs = b.to_s.reverse.scan(/.{1,#{radix_len}}/)
    .map(&:reverse).map(&:to_i).reverse
  qs = [] # ans
  s = as.length - 1
  t = bs.length - 1
  k = radix / (1 + bs[t])

  a *= k
  b *= k

  while 1
    as = a.to_s.reverse.scan(/.{1,#{radix_len}}/)
      .map(&:reverse).map(&:to_i).reverse
    bs = b.to_s.reverse.scan(/.{1,#{radix_len}}/)
      .map(&:reverse).map(&:to_i).reverse
    s = as.length - 1
    t = bs.length - 1
    p [a, b, as, bs, s, t]
    u = 0
    qu = 0
    if as[s] < bs[t]
      puts "(3)"
      u = s - t - 1
      qu = (as[s] * radix + as[s-1]) / bs[t]
      puts "#{u}, #{qu}"
      while b * qu * radix**u > a
        qu -= 1
      end
    else
      if a < b * radix**(s - t)
        puts "(2)"
        u = s - t - 1
        qu = radix - 1
        while b * qu * radix**u > a
          qu -= 1
        end
      else
        puts "(1)"
        u = s - t
        qu = 1
      end
    end
    if qs[u].nil?
      qs[u] = qu
    else
      qs[u] += qu
    end
    a -= b * qu * radix**u
    while a < 0
      qs[u] -= 1
      a += b * radix**u
    end
    if u <= 0
      return [qs.reverse, a / k]
    end
  end
end

res = fast_divmod(a, b, radix: RADIX)
puts "result   = #{res}"
puts "expected = #{a.divmod(b)}"
