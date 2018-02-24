
require 'prime'

sophie_germain_prime_count = 0
num = (1e10 + 1).to_i

puts("Sophie Germain Primes:")
while sophie_germain_prime_count < 10
  if Prime.prime?(num)
    num2 = 2 * num + 1
    if Prime.prime?(num2)
      puts "p = #{num}"
      sophie_germain_prime_count += 1
    end
  end

  # next
  num += 2
end
