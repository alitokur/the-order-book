#pragma once

#include <cstdint>
#include <vector>
#include <random>
#include <cmath>
#include "types.h"
struct TestOrder{
    uint64_t order_id;
    Side side;
    uint64_t price;
    uint32_t quantity;
    uint32_t event_timestamp;
    uint32_t entry_timestamp;
};

#define ORDER_NUM 1000000

// S(t+Δt) = S(t) * exp( (μ - 0.5σ²)*Δt + σ * sqrt(Δt) * Z )
inline double gbm(double S, double mu, double sigma, double dt)
{
    static thread_local std::mt19937_64 gen(123);
    static thread_local std::normal_distribution<> N(0.0, 1.0);
    double z = N(gen);
    return S * std::exp((mu - 0.5 * sigma * sigma) * dt +
                        sigma * std::sqrt(dt) * z);
}

inline uint64_t gbm_price_uint(){
    double p = gbm(0.04, 0.0, 0.01, 1.0/60.0);
    return static_cast<uint64_t>(std::llround(p * 1e6));

}

inline std::vector<TestOrder> get_orders(){
    std::vector<uint64_t> prices;
    prices.reserve(ORDER_NUM);
    double S = 0.04;
    for(int i = 0; i<ORDER_NUM; i++){
        S = gbm_price_uint();
        prices.push_back(S);
    }


    std::vector<TestOrder> orders;
    orders.reserve(ORDER_NUM);
    uint64_t id = 1;
    for(double p : prices){
        TestOrder o;
        o.order_id = id;
        o.side = (rand()%2 == 0) ? Side::Buy : Side::Sell;
        o.price = p + ((rand() % 2001 - 1000 ) / 1e6 );
        o.quantity = 1 + rand() % 1000;
        id++;
        orders.push_back(o);
    }
    return orders;
};

 
