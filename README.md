# Discount_Calculator

A c++ program that calculates discount for various promotions.

## Requirements

- c++11

## Compile
```
$ g++ main.cpp class.cpp -o main 
```
## Execute
```
$ ./main
```

## Input
4 files for the list of promotions, orders, products and users.
### promotion_list
```
// Format: promotion_id buy  get  off off_limit no_clear_limit month_clear_limit
           1            100  0    20  0         -1             -1
           2            150  -1   0   0         -1             -1
           3            200  30   0   0         5              -1
           ..  
```
- buy: Threshold to get the discount.
- get: 
  - Positive: Amount of the discount. (*# prices*)
  - Zero: Its a X%-off type discount.
  - Negative: Its a get-product type discount. The absolute value would be the product's ID.
- off: The X of the X%-off type discount.
- off_limit: The limitation of the X%-off type discount. 0 if none.
- no_clear_limit: The max sitewise use of the discount. (*# times*)
- month_clear_limit: The max sitewise use of the discount. (*# prices per month*)
### order_list
```
// Format user_id product_id_1 amount_1 product_id_2 amount_2 ...
          1       1            1        1            1
          2       1            1
          3       3            4
          4       1            2        3            4
          5       2            2        5            5

```
### user_list
```
// Format: user_id
           1
           2
           3
           ..  
```
### promduct_list
```
// Format: product_id price discount_amount discount_price
           1          100   -1              0
           2          203   -1              -1
           3          405   2               200
           4          650   4               650
           5          50    1               5
```
- Buy *discount_amount* products to get *discount_price* discount.

## Output
The user's ID, the price and the products it get.
One ouput per order.
```
Case 1: User ID 1
  List price: 200
  Discount amount: 80
  Sale price: 120
  Whole order:
    3x Product #1

Case 2: User ID 2
  List price: 100
  Discount amount: 30
  Sale price: 70
  Whole order:
    1x Product #1

Case 3: User ID 3
  List price: 1620
  Discount amount: 764
  ...
```

## Tests
In class.cpp.
```
void Test_calculator::test_calc(Promotion* promotion_list, Order* order_list, Product* product_list, User* user_list)
{
  Calculator test_calc;
  assert(test_calc.specific_calculate(&order_list[0], product_list) == 0);
  assert(test_calc.specific_calculate(&order_list[4], product_list) == 25);
  assert(test_calc.discount_calculate(promotion_list, &order_list[0], &user_list[order_list[0].user_id], 200) == 80);
  assert(test_calc.discount_calculate(promotion_list, &order_list[4], &user_list[order_list[4].user_id], 656) == 171.2);
}
```
