#include "class.h"

void Promotion::new_month()
{
  month_used_limit = 0;
}

double Calculator::specific_calculate(Order* order, Product* product_list)
{
  double discount_sum = 0;
  for(int i = 0 ; i < MAX ; i++)
  {
    if(product_list[i].discount_amount > 0 && order->amount[i] > product_list[i].discount_amount)
      discount_sum += product_list[i].discount_price * (order->amount[i] / product_list[i].discount_amount);
  }
  return discount_sum;
}

double Calculator::discount_calculate(Promotion* promotion_list, Order* order, User* user, double list_price)
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
        //No clear discount (# times)
        if(promotion_list[i].no_clear_limit > 0)
        {
          promotion_list[i].no_clear_limit-- ;
          discount += promotion_list[i].get;
        }
        //Monthly clear discount (# prices)
        if(promotion_list[i].month_used_limit < promotion_list[i].month_clear_limit)
        {
          promotion_list[i].month_used_limit += promotion_list[i].get ;
          discount += promotion_list[i].get;
        }
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

void Test_calculator::test_calc(Promotion* promotion_list, Order* order_list, Product* product_list, User* user_list)
{
  Calculator test_calc;
  assert(test_calc.specific_calculate(&order_list[0], product_list) == 0);
  assert(test_calc.specific_calculate(&order_list[4], product_list) == 25);
  assert(test_calc.discount_calculate(promotion_list, &order_list[0], &user_list[order_list[0].user_id], 200) == 80);
  assert(test_calc.discount_calculate(promotion_list, &order_list[4], &user_list[order_list[4].user_id], 656) == 171.2);
}

bool comp(Promotion l, Promotion r)
{
  return l.get > r.get;
}
