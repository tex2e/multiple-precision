
require 'bigdecimal'

num = BigDecimal.new("49012345e1000")
result = num.sqrt(1000).to_s("F") # Don't use exp format like 0.33e10
print result.to_i
