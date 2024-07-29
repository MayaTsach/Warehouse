#include <string>
#include <vector>
#include <iostream>
using std::string;
using std::vector;
#include "../include/Customer.h"




//Customer
Customer::Customer(int _id, const string &_name, int _locationDistance, int _maxOrders) : 
    id(_id), name(_name), locationDistance(_locationDistance), maxOrders(_maxOrders), ordersId() {}
const string &Customer::getName() const {
    return name;
}
int Customer::getId() const {
    return id;
}
int Customer::getCustomerDistance() const {
    return locationDistance;
}
int Customer::getMaxOrders() const { //Returns maxOrders
    return maxOrders;
} 
int Customer::getNumOrders() const { //Returns num of orders the customer has made so far
    return ordersId.size();
} 

bool Customer::canMakeOrder() const { //Returns true if the customer didn't reach max orders
    return ordersId.size() < static_cast<std::vector<int>::size_type>(maxOrders);
} 
const vector<int> &Customer::getOrdersIds() const {
    return ordersId;
}
int Customer::addOrder(int orderId) { //return OrderId if order was added successfully, -1 otherwise
    if (canMakeOrder()) {
        ordersId.push_back(orderId);
        return orderId;
    }
    return -1;
}


//SoldierCustomer
SoldierCustomer::SoldierCustomer(int _id, string _name, int _locationDistance, int _maxOrders) : 
    Customer(_id, _name, _locationDistance, _maxOrders) {}

SoldierCustomer *SoldierCustomer::clone() const {
    return new SoldierCustomer(*this);
}


//CivilianCustomer
CivilianCustomer::CivilianCustomer(int _id, string _name, int _locationDistance, int _maxOrders) :
    Customer(_id, _name, _locationDistance, _maxOrders) {}

CivilianCustomer *CivilianCustomer::clone() const {
    return new CivilianCustomer(*this);
}
