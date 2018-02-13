
require 'bigdecimal'

num = BigDecimal.new("123") ** BigDecimal.new("45")
result = num.to_s("F") # Don't use exp format like 0.33e10
puts result.to_i

# d, m = BigDecimal.new("12345E100").divmod( BigDecimal.new("67890E50") )
# result  = d.to_s("F") # Don't use exp format like 0.33e10
# result2 = m.to_s("F") # Don't use exp format like 0.33e10
# puts result.to_i
# puts "---"
# puts result2.to_i
