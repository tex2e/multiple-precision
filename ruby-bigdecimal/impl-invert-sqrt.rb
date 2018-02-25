
require 'bigdecimal'
require 'bigdecimal/math'

# Calc 1/sqrt(num) with float
def inverse_sqrt_float(num)
  f = Proc.new do |x|
    x * (3 - num * x**2) / 2
  end

  x = 0.2
  puts "--- Float ---"
  for i in 1..100
    puts "%2d) #{x}" % i
    x_next = f.(x)
    break if x_next - x == 0
    x = x_next
  end
end

# Calc 1/sqrt(num) with integer
def inverse_sqrt_integer(num, prec:)
  f = Proc.new do |x|
    res = (x * (3 * 10**(prec*2) - num * x**2) / 2)
    shift = res.to_s.length - prec
    if shift > 0
      res /= 10**shift
    end
    res
  end

  x = 2
  puts "--- Integer ---"
  for i in 1..100
    puts "%2d) #{x}" % i
    x_next = f.(x)
    break if x_next - x == 0
    x = x_next
  end
end


N = 2
inverse_sqrt_float(N)
puts
inverse_sqrt_integer(N, prec: 30)
puts
puts "--- Expected ---"
num = BigDecimal("1") / BigDecimal("2").sqrt(30)
result = num.to_s("F") # Don't use exp format like 0.33e10
puts "1/sqrt(#{N}) = #{result}"
puts
