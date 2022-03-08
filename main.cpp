#include <iostream>
#include <algorithm>
#include <fstream>
#include <string>
#include <sstream>

#define MAX 100
#define STRING_LEN 20
#define INF 2147483647

using namespace std;

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
  double specific_calculate(Product* product_list, Order* order)
  {
    double discount_sum = 0;
    for(int i = 0 ; i < MAX ; i++)
    {
      if(product_list[i].discount_amount > 0 && order->amount[i] > product_list[i].discount_amount)
        discount_sum += product_list[i].discount_price * (order->amount[i] / product_list[i].discount_amount);
    }
    return discount_sum;
  }

  double discount_calculate(Promotion* promotion_list, Order* order, double list_price)
  {
    double discount_sum = 0;
    for(int i = 0 ; i < MAX ; i++)
    {
      // Format: promotion_id buy get  off off_limit no_clear_limit month_clear_limit
      double discount = 0;
      if(promotion_list[i].promotion_id != 0 && list_price >= promotion_list[i].buy)
      {
        if(promotion_list[i].get > 0) // Direct discount
        {
          discount += promotion_list[i].get;
        }
        else if(promotion_list[i].get == 0) // %off
        {
          double org_discount = list_price * promotion_list[i].off / 100.0;
          if(promotion_list[i].off_limit != 0 && org_discount > promotion_list[i].off_limit)
            discount += promotion_list[i].off_limit;
          else
            discount += org_discount;
        }
        else // Obtain product
        {
          order->amount[-int(promotion_list[i].get)]++;
        }
      }
      discount_sum += discount;
    }
    return discount_sum; //discount_sum ;
  }
};

bool comp(Promotion l, Promotion r)
{
  return l.get > r.get;
}

int main(void)
{
  Promotion promotion_list[MAX];
  User user_list[MAX];
  Order order_list[MAX];
  Product product_list[MAX];

  fstream promotion_file, user_file, order_file, product_file;
  promotion_file.open("promotion_list");
  user_file.open("user_list");
  order_file.open("order_list");
  product_file.open("product_list");

  string s;
  int order_amount = 0;

  while(getline(promotion_file, s))
  {
    if(s[0] == '/')
      continue;

    istringstream iss(s);
    // Format: promotion_id buy get  off off_limit no_clear_limit month_clear_limit
    int p_id = 0;
    iss >> p_id;
    promotion_list[p_id].promotion_id = p_id;
    iss >> promotion_list[p_id].buy
        >> promotion_list[p_id].get
        >> promotion_list[p_id].off
        >> promotion_list[p_id].off_limit
        >> promotion_list[p_id].no_clear_limit
        >> promotion_list[p_id].month_clear_limit ;
  }
  sort(promotion_list, promotion_list + MAX, comp);

  int itr = 0;
  while(getline(order_file, s))
  {
    if(s[0] == '/' || s.length() == 0)
      continue;

    order_amount++ ;

    istringstream iss(s);
    // Format: product_id price discount_amount discount_price

    int p_id, p_amount;
    iss >> order_list[itr].user_id;
    while(iss >> p_id >> p_amount)
    {
      //cout << "THERE " << p_id << " " << p_amount << "\n" ;
      order_list[itr].amount[p_id] += p_amount;
    }

    itr++;
  }

  while(getline(product_file, s))
  {
    if(s[0] == '/' || s.length() == 0)
      continue;

    istringstream iss(s);
    // Format: product_id price discount_amount discount_price
    int p_id;
    iss >> p_id;
    product_list[p_id].product_id = p_id;
    iss >> product_list[p_id].price
        >> product_list[p_id].discount_amount
        >> product_list[p_id].discount_price ;
  }

  Calculator calc;

  for(int o = 0 ; o < order_amount ; o++)
  {
    double list_price = 0, discount_amount = 0;
    for(int i = 0 ; i < MAX ; i++)
    {
      if(order_list[o].amount[i] != 0)
      {
        list_price += product_list[i].price * order_list[o].amount[i];
      }
    }
    discount_amount = calc.specific_calculate(product_list, &order_list[o])
                    + calc.discount_calculate(promotion_list, &order_list[o], list_price) ;

    cout << "Case " << o + 1 << ":\n  List price: " << list_price
         << "\n  Discount amount: " << discount_amount
         << "\n  Sale price: " << list_price - discount_amount
         << "\n  Whole order:\n";
    for(int i = 0 ; i < MAX ; i++)
      if(order_list[o].amount[i] != 0)
        cout << "    " << order_list[o].amount[i] << " Product #" <<  i << "\n";
  }
  //cout << calc.discount_calculate(order_list[0], 0) << "\n" ;



  //

  promotion_file.close();
  user_file.close();
  order_file.close();
  product_file.close();

  return 0;
}
