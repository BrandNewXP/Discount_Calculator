#include <iostream>
#include <algorithm>
#include <fstream>
#include <string>
#include <sstream>

#define MAX 100
#define STRING_LEN 20
#define INF 2147483647

class Promotion
{
public:
  int promotion_id = 0;
  /* Buy $buy get $get discount,
  or buy $buy get $off % discount.
     Max to $limit .              */
  double buy;
  double get; // 0: %off, -t: Get product_id t
  double off; // %

  /* Limit per order. */
  double off_limit; // #price


  int no_clear_limit; // #times
  double month_clear_limit; // #price
};

class Order
{
public:
  int user_id;
  int amount[MAX] = {0};
};

class Product
{
public:
  int product_id = 0;
  double price;

  /* Buy $discount_amount products in order
     to get $discount_price discount. */
  int discount_amount; // 0: No discount
  double discount_price;
};

class User
{
public:
  int user_id = 0;

  /* Record discount limits. */
  int no_clear[MAX] = {0} ; // #times
  double month_clear[MAX] = {0} ; // #price

  /* Initial discount limits. */
  void new_month()
  {
    for(int i = 0 ; i < MAX ; i++)
      month_clear[i] = 0;
  }
};

class Calculator
{
public:
  double specific_calculate(Product* product_list, Order* order);
  double discount_calculate(Promotion* promotion_list, Order* order, User* user, double list_price);
};
