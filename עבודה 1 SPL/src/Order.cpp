#include "../include/Order.h"
#include <iostream>

        Order::Order(int _id, int _customerId, int _distance): id(_id), customerId(_customerId), distance(_distance), status(OrderStatus::PENDING), collectorId(NO_VOLUNTEER), driverId(NO_VOLUNTEER) {}; 

        int Order::getId() const {
            return id;
        }

        int Order::getCustomerId() const {
            return customerId;
        }
      
        void Order::setStatus(OrderStatus status) {
            this->status = status;
        }

        void Order::setCollectorId(int collectorId) {
            this->collectorId = collectorId;
        }

        void Order::setDriverId(int driverId) {
            this->driverId = driverId;
        }

        int Order::getCollectorId() const {
            return collectorId;
        }
        
        int Order::getDriverId() const {
            return driverId;
        }

        OrderStatus Order::getStatus() const {
            return status;
        }

        int Order::getDistance() const {
            return distance;
        }

        string Order::EnumToString(OrderStatus status) const{  //returns a string of the enum
            if (status == OrderStatus::PENDING) return "PENDING";
            if (status == OrderStatus::COLLECTING) return "COLLECTING";
            if (status == OrderStatus::DELIVERING) return "DELIVERING"; 
            if (status == OrderStatus::COMPLETED) return "COMPLETED"; 
            return NULL;  // only for the compiler to not write a warning
        }

        const string Order::toString() const {  //toString for the Close action
            return {"OrderID: " + std::to_string(id) + ", CostumerID: " + std::to_string(customerId) + ", OrderStatus: " + EnumToString(status)};  //need to see if EnumToString works fine
        }

        const string Order::toString_PrintOrder() const { //toString for the PrintOrderStatus action
            std::string result = ("\n OrderId: " + std::to_string(id) + "\n OrderStatus: " + EnumToString(status) + "\n CostumerID: " + std::to_string(customerId));
            
            if (collectorId == NO_VOLUNTEER) {result += "\n Collector: NONE";}
            else {result += "\n Collector: " + std::to_string(collectorId);}
            
            if (driverId == NO_VOLUNTEER) {result += "\n Driver: NONE";}
            else {result += "\n Driver: " + std::to_string(driverId);}
            
            return result;  //need to see if EnumToString works fine
        }

        const string Order::toString_PrintCostumer() const {  //toString for the PrintCostumerStatus action
            return {"OrderId: " + std::to_string(id) + "\nOrderStatus: " + EnumToString(status)};  //need to see if EnumToString works fine
        }

        Order *Order::clone() const {
            return new Order(*this);
        }
