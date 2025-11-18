
#include "../include/order_book.h"
#include "order_generator.h"
#include <chrono>

int main (int argc, char *argv[]) {

    OrderBook book;
    std::vector orders = get_orders();
    

    auto start = std::chrono::high_resolution_clock::now();
    for(const auto& order:orders){
        book.add_order(order.order_id, order.side, order.quantity, order.price,0,0);
    }
    auto end = std::chrono::high_resolution_clock::now();
    auto diff = end - start; 
    std::cout << "Added " << orders.size() << " orders in "
              << std::chrono::duration<double, std::milli>(diff).count() << " ms"
              << std::endl;

    std::cout << "Time for each order: "
              << std::chrono::duration<double, std::micro>(diff).count() / orders.size()
              << " µs" << std::endl;
    
    book.display_orders();

    return 0;
}
