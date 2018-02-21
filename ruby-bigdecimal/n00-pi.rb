
require 'bigdecimal'
require 'bigdecimal/math'

# num = BigMath::atan(BigDecimal.new("1") / 8, 1000) * BigDecimal.new("1e1001")
# result = num.to_s("F") # Don't use exp format like 0.33e10
# puts "arctan(1/8) ="
# puts result.to_i
#
# num = BigMath::atan(BigDecimal.new("1") / 57, 1000) * BigDecimal.new("1e1001")
# result = num.to_s("F") # Don't use exp format like 0.33e10
# puts "arctan(1/57) ="
# puts result.to_i
#
# num = BigMath::atan(BigDecimal.new("1") / 239, 1000) * BigDecimal.new("1e1001")
# result = num.to_s("F") # Don't use exp format like 0.33e10
# puts "arctan(1/239) ="
# puts result.to_i

num = BigMath.PI(1001) * BigDecimal("1e1001")
result = num.to_s("F") # Don't use exp format like 0.33e10
puts "pi ="
puts result.to_i
