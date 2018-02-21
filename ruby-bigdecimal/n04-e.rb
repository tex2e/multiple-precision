
require 'bigdecimal'
require 'bigdecimal/math'

num = BigMath.E(1001) * BigDecimal("1e1000")
result = num.to_s("F") # Don't use exp format like 0.33e10
puts "e ="
puts result.to_i
