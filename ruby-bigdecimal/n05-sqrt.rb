
require 'bigdecimal'

N = 98765432100
num = BigDecimal.new(N).sqrt(1010)
result = num.to_s("F") # Don't use exp format like 0.33e10
puts "sqrt(#{N}) ="
puts "#{result.to_i}"
float_part = (num - num.to_i).to_s[1..1001]
puts "#{float_part}"
