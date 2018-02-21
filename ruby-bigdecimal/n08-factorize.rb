
require 'prime'

for n in 2..18
  r_n = (10**n - 1) / 9
  if Prime.prime?(r_n)
    puts "R_#{n} = #{r_n} is a prime number."
  else
    factors = Prime.prime_division(r_n)
      .map { |factor, count| ([factor] * count).join(" * ") }
      .join(" * ")
    puts "R_#{n} = #{r_n} = #{factors}"
  end
end
