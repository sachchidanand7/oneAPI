#ifndef _MARKETDATA_H
#define _MARKETDATA_H
#include<string>
#include<iostream>
#include<vector>
#include<unordered_map>
#include<memory>


using namespace std;

namespace common {

#pragma pack(1)
struct MarketData {
char symbol[128];
int64_t productId;
double price;
double yield;
double spread;
double benchMarkPrice;
long   benchMarkId;
long  timestamp;

};

#pragma pack()

}
#endif
