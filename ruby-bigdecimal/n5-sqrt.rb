
require 'bigdecimal'

num = BigDecimal.new("2e2000")
result = num.sqrt(1000).to_s("F") # Don't use exp format like 0.33e10
puts "sqrt(2e2000) ="
puts result.to_i
