
#include <stdio.h>
#include <criterion/criterion.h>

unsigned long long next_bigger_number(unsigned long long n){
  long long res = n;
  int i = 10;

  //Find the rightmost digit that is smaller than its right digit
  for(; n / i > 0; i *= 10){
    int current = n / i % 10;
    int prev = 10 * n / i % 10;
    if(current < prev) break;
  }

  //No digit found means all digits are greater than its right digit
  //means no bigger number possible, return -1
  if(n / i == 0 ) return -1;

  //Find the smallest digit to the right of found digit that is greater
  //than the found digit and swap them
  int j = i / 10;
  int current = n / i % 10;
  int smallest = n / j % 10;
  int smallestPos = j;
  while(j > 0){
    int x = n / j % 10;
    if(x < smallest && x > current) smallest = x, smallestPos = j;
    j /= 10;
  }
  res = n + (smallest - current) * (i - smallestPos);

  //Sort all the digits right of the found digit
  for(int x = i/10; x > 0; x /= 10){
    for(int y = x/10; y > 0; y /= 10){
      int a = res / x % 10;
      int b = res / y % 10;
      if(a > b) res = res + (b - a) * (x - y);
    }
  }

  return res;
}

Test(the_multiply_function, should_pass_all_the_tests_provided) {
  printf("Sample Test Cases\n");
  cr_assert_eq(next_bigger_number(12), 21);
  cr_assert_eq(next_bigger_number(513), 531);
  cr_assert_eq(next_bigger_number(2017), 2071);
  cr_assert_eq(next_bigger_number(414), 441);
  cr_assert_eq(next_bigger_number(144), 414);
}
