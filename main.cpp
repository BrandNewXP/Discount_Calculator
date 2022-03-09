#include "class.h"

using namespace std;

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
                    + calc.discount_calculate(promotion_list, &order_list[o], &user_list[order_list[o].user_id], list_price) ;

    cout << "Case " << o + 1 << ": User ID " << order_list[o].user_id
         << "\n  List price: " << list_price
         << "\n  Discount amount: " << discount_amount
         << "\n  Sale price: " << list_price - discount_amount
         << "\n  Whole order:\n";
    for(int i = 0 ; i < MAX ; i++)
      if(order_list[o].amount[i] != 0)
        cout << "    " << order_list[o].amount[i] << "x Product #" <<  i << "\n";
  }

  promotion_file.close();
  user_file.close();
  order_file.close();
  product_file.close();

  return 0;
}
