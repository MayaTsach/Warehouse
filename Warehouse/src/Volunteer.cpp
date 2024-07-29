#include "../include/Volunteer.h"
#include <iostream>

        Volunteer::Volunteer(int _id, const string &_name): completedOrderId(NO_ORDER), activeOrderId(NO_ORDER), id(_id), name(_name) {}

        int Volunteer::getId() const{
            return id;
        }

        const string &Volunteer::getName() const{
            return name;
        }

        int Volunteer::getActiveOrderId() const{
            return activeOrderId;
        }

        int Volunteer::getCompletedOrderId() const{
            return completedOrderId;
        }

        bool Volunteer::isBusy() const{  // Signal whether the volunteer is currently processing an order    
            if (activeOrderId == NO_ORDER) // if there's no active order then he's not busy
                return false;
            return true;
        }

        string Volunteer::toStringIsBusy() const {
            if (isBusy()) {return "True";}
            else {return "False";}
        }




        CollectorVolunteer::CollectorVolunteer(int _id, string _name, int _coolDown):  Volunteer(_id, _name), coolDown(_coolDown), timeLeft(0){}

        CollectorVolunteer * CollectorVolunteer::clone() const{//Return a copy of the volunteer
            return new CollectorVolunteer(*this);
        }   

        void CollectorVolunteer::step() {  //Simulate volunteer step,if the volunteer finished the order, transfer activeOrderId to completedOrderId
            if(activeOrderId != -1) {   
                bool isDone = this->decreaseCoolDown();
                if(isDone){
                    completedOrderId = activeOrderId;
                    activeOrderId = NO_ORDER;
                }
            }
        }

        int CollectorVolunteer::getCoolDown() const{
            return coolDown;
        }

        int CollectorVolunteer::getTimeLeft() const{
            return timeLeft;
        }

        bool CollectorVolunteer::decreaseCoolDown(){  //Decrease timeLeft by 1,return true if timeLeft=0,false otherwise
            timeLeft = timeLeft - 1;
            if (timeLeft == 0) 
                return true;
            return false;
        }

        bool CollectorVolunteer::hasOrdersLeft() const {  // Signal whether the volunteer didn't reach orders limit,Always true for CollectorVolunteer and DriverVolunteer
            return true;
        }

        bool CollectorVolunteer::canTakeOrder(const Order &order) const {  // Signal if the volunteer can take the order.  
            if (timeLeft == 0)
                return true;
            return false;         
        }  

        void CollectorVolunteer::acceptOrder(const Order &order) {   // Prepare for new order(Reset activeOrderId,TimeLeft,DistanceLeft,OrdersLeft depends on the volunteer type)
            if(canTakeOrder(order)){
                activeOrderId = order.getId();
                timeLeft = coolDown;  //the time that will take him to proccess the order 
                activeOrderId = order.getId();
            }          
        }

        string CollectorVolunteer::getVolunteerType() const{
            return "Collector";
        }

        string CollectorVolunteer::toString() const {   //toString fot PrintVolunteerStatus
            string theString = "\n VolunteerID: " + std::to_string(getId()) + "\n isBusy: " + toStringIsBusy() + "\n OrderID: ";
            if(isBusy())
                theString = theString + std::to_string(getActiveOrderId());
            else theString = theString + "None";
            theString = theString + "\n TimeLeft: " ;
            if(getTimeLeft() == 0) 
                theString = theString + "None";
            else theString = theString + std::to_string(getTimeLeft());
            theString = theString + "\n OrdersLeft: No Limit";
            return theString + "\n";
        }
    




        LimitedCollectorVolunteer::LimitedCollectorVolunteer(int _id, string _name, int _coolDown ,int _maxOrders): CollectorVolunteer(_id, _name, _coolDown), maxOrders(_maxOrders), ordersLeft(_maxOrders) {}

        LimitedCollectorVolunteer *LimitedCollectorVolunteer::clone() const {
            return new LimitedCollectorVolunteer(*this);
        }

        bool LimitedCollectorVolunteer::hasOrdersLeft() const {   // Signal whether the volunteer didn't reach orders limit,Always true for CollectorVolunteer and DriverVolunteer
            if (ordersLeft == 0) return false;
            return true;
        }

        bool LimitedCollectorVolunteer::canTakeOrder(const Order &order) const {   // Signal if the volunteer can take the order.
            if (CollectorVolunteer::canTakeOrder(order) && hasOrdersLeft()) return true;
            return false;          
        }

        void LimitedCollectorVolunteer::acceptOrder(const Order &order) {  // Prepare for new order(Reset activeOrderId,TimeLeft,DistanceLeft,OrdersLeft depends on the volunteer type)
            if(canTakeOrder(order)){
                CollectorVolunteer::acceptOrder(order);
                ordersLeft = ordersLeft - 1;
            }
        }  

        string LimitedCollectorVolunteer::getVolunteerType() const{
            return "Collector";
        } 

        int LimitedCollectorVolunteer::getMaxOrders() const{
            return maxOrders;
        }

        int LimitedCollectorVolunteer::getNumOrdersLeft() const{
            return ordersLeft;
        }

        string LimitedCollectorVolunteer::toString() const {  //toString fot PrintVolunteerStatus
            string theString = "\n VolunteerID: " + std::to_string(getId()) + "\n isBusy: " + toStringIsBusy() + "\n OrderID: ";
            if(isBusy())
                theString = theString + std::to_string(getActiveOrderId());
            else theString = theString + "None";
            theString = theString + "\n TimeLeft: " ;
            if(getTimeLeft() == 0) 
                theString = theString + "None";
            else theString = theString + std::to_string(getTimeLeft());
            theString = theString + "\n OrdersLeft: " + std::to_string(ordersLeft);
            return theString + "\n";
        }
    





        DriverVolunteer::DriverVolunteer(int _id, string _name, int _maxDistance, int _distancePerStep): Volunteer(_id, _name), maxDistance(_maxDistance), distancePerStep(_distancePerStep), distanceLeft(0){}

        DriverVolunteer *DriverVolunteer::clone() const {
            return new DriverVolunteer(*this);
        }

        int DriverVolunteer::getDistanceLeft() const{
            return distanceLeft;
        }

        int DriverVolunteer::getMaxDistance() const{
            return maxDistance;
        }

        int DriverVolunteer::getDistancePerStep() const{
            return distancePerStep;
        }  

        bool DriverVolunteer::decreaseDistanceLeft(){ //Decrease distanceLeft by distancePerStep,return true if distanceLeft<=0,false otherwise
            distanceLeft = distanceLeft - distancePerStep;
            if (distanceLeft <= 0){
                distanceLeft = 0;  //if the order is completed then the distance left is 0
                return true;  
            }
            return false;
        } 
        
        bool DriverVolunteer::hasOrdersLeft() const {  // Signal whether the volunteer didn't reach orders limit,Always true for CollectorVolunteer and DriverVolunteer
            return true;
        }

        bool DriverVolunteer::canTakeOrder(const Order &order) const {  // Signal if the volunteer is not busy and the order is within the maxDistance
            if ((distanceLeft > 0) || (order.getDistance() > maxDistance))
                return false;
            return true;
        } 
        
        // Prepare for new order(Reset activeOrderId,TimeLeft,DistanceLeft,OrdersLeft depends on the volunteer type)
        // Assign distanceLeft to order's distance 
        void DriverVolunteer::acceptOrder(const Order &order) {  
            if(canTakeOrder(order)){
                distanceLeft = order.getDistance();
                activeOrderId = order.getId();
            }
        }

        string DriverVolunteer::getVolunteerType() const{
            return "Driver";
        } 

        // Decrease distanceLeft by distancePerStep
        //Simulate volunteer step,if the volunteer finished the order, transfer activeOrderId to completedOrderId
        void DriverVolunteer::step() {  
            if(activeOrderId != -1) {
                bool isDone = decreaseDistanceLeft();
                if (isDone){
                    completedOrderId = activeOrderId;
                    activeOrderId = NO_ORDER;
                }
            }
        } 

        string DriverVolunteer::toString() const {  //toString fot PrintVolunteerStatus
            string theString = "\n VolunteerID: " + std::to_string(getId()) + "\n isBusy: " + toStringIsBusy() + "\n OrderID: ";
            if(isBusy())
                theString = theString + std::to_string(getActiveOrderId());
            else theString = theString + "None";
            theString = theString + "\n TimeLeft: " ;
            if(getDistanceLeft() == 0) 
                theString = theString + "None";
            else theString = theString + std::to_string(getDistanceLeft());
            theString = theString + "\n OrdersLeft: No Limit";
            return theString + "\n";
        }






        LimitedDriverVolunteer::LimitedDriverVolunteer(int _id, const string &_name, int _maxDistance, int _distancePerStep,int _maxOrders): DriverVolunteer( _id, _name, _maxDistance, _distancePerStep), maxOrders(_maxOrders), ordersLeft(_maxOrders){}
                
        LimitedDriverVolunteer *LimitedDriverVolunteer::clone() const {
            return new LimitedDriverVolunteer (*this);
        }

        int LimitedDriverVolunteer::getMaxOrders() const{
            return maxOrders;
        }

        int LimitedDriverVolunteer::getNumOrdersLeft() const{
            return ordersLeft;
        }

        bool LimitedDriverVolunteer::hasOrdersLeft() const { // Signal whether the volunteer didn't reach orders limit,Always true for CollectorVolunteer and DriverVolunteer
            if (ordersLeft > 0)
                return true;
            return false;
        }

        bool LimitedDriverVolunteer::canTakeOrder(const Order &order) const { // Signal if the volunteer is not busy, the order is within the maxDistance.
            if (DriverVolunteer::canTakeOrder(order) && hasOrdersLeft()) return true;
            return false;          
        }

        // Prepare for new order(Reset activeOrderId,TimeLeft,DistanceLeft,OrdersLeft depends on the volunteer type)
        // Assign distanceLeft to order's distance and decrease ordersLeft
        void LimitedDriverVolunteer::acceptOrder(const Order &order) { 
             if(canTakeOrder(order)){
                DriverVolunteer::acceptOrder(order);
                ordersLeft = ordersLeft - 1;
                activeOrderId = order.getId();
            }
        }

        string LimitedDriverVolunteer::getVolunteerType() const{
            return "Driver";
        } 

        string LimitedDriverVolunteer::toString() const {   //toString fot PrintVolunteerStatus
            string theString = "\n VolunteerID: " + std::to_string(getId()) + "\n isBusy: " + toStringIsBusy() + "\n OrderID: ";
            if(isBusy())
                theString = theString + std::to_string(getActiveOrderId());
            else theString = theString + "None";
            theString = theString + "\n TimeLeft: " ;
            if(getDistanceLeft() == 0) 
                theString = theString + "None";
            else theString = theString + std::to_string(getDistanceLeft());
            theString = theString + "\n OrdersLeft: " + std::to_string(ordersLeft);
            return theString + "\n";
        }
