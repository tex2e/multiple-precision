
require 'bigdecimal'
require 'bigdecimal/math'

num = BigMath.PI(10001) * BigDecimal("1e10001")
result = num.to_s("F") # Don't use exp format like 0.33e10
puts "pi ="
puts result.to_i
