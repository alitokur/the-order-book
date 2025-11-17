
#include "../include/order_book.h"
#include "order_generator.h"

int main (int argc, char *argv[]) {

    OrderBook book;
    ///create orders with GBM
    
    std::vector orders = get_orders();

   
    for(const auto& order:orders){
        book.add_order(order.order_id, order.side, order.quantity, order.price,0,0);
    }
    book.display_orders();

    return 0;
}
