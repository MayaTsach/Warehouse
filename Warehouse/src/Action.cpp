#include <string>
#include <vector>
#include <iostream>
#include <algorithm>
using std::string;
using std::vector;
#include "../include/Action.h"
#include "../include/Volunteer.h"

using namespace std;

BaseAction::BaseAction() : errorMsg(""), status(ActionStatus::COMPLETED) {}
ActionStatus BaseAction::getStatus() const {
    return status;
}

string BaseAction::statusToString() const {
    if (status == ActionStatus::COMPLETED) {return "COMPLETED";}
    else {return "ERROR";}
}

void BaseAction::complete() {
    status = ActionStatus::COMPLETED;
}
void BaseAction::error(string errorMsg) {
    status = ActionStatus::ERROR;
    this->errorMsg = errorMsg;
    std::cout << "Error: " << this->errorMsg << endl; // Prints the error message
}
string BaseAction::getErrorMsg() const {
    return errorMsg;
}

//Simulate step
SimulateStep::SimulateStep(int _numOfSteps): numOfSteps(_numOfSteps) {}

void SimulateStep::act(WareHouse &wareHouse){
    int counter = numOfSteps;
    while(counter > 0){
        //for(Order* currOrder : wareHouse.getPendingOrders())
        vector<Order*>::iterator itPending = wareHouse.getPendingOrders().begin(); 
        while(itPending != wareHouse.getPendingOrders().end()){
            Order* currOrder = *itPending;
            //from pending to collector
            if(currOrder->getStatus() == OrderStatus::PENDING){
                Volunteer& assignedVolunteer = wareHouse.getNextFreeCollector(*currOrder);
                //check that there is a free volunteer, if there wasn't- the null volunteer will be gotten
                if(assignedVolunteer.getId() != -1){  
                    //the volunteer takes the order
                    assignedVolunteer.acceptOrder(*currOrder);
                    //the order status is changed
                    currOrder->setStatus(OrderStatus::COLLECTING);
                    //the order collector is changed
                    currOrder->setCollectorId(assignedVolunteer.getId());

                    //the order moves to inProcess vector
                    (wareHouse.getInProcessOrders()).push_back(currOrder);
                    //the order is deleted from Pending vector
                    // Use std::remove_if along with erase to remove the specified Order pointer
                    wareHouse.getPendingOrders().erase(std::remove_if(
                        wareHouse.getPendingOrders().begin(),
                        wareHouse.getPendingOrders().end(),
                        [currOrder](Order* order) { return order == currOrder; }
                    ), wareHouse.getPendingOrders().end());              
                }
                else{
                    //only if we didn't delete the order from the vector we will continue to the next order
                    //because the iterator stays in the same place the deleted order was
                    itPending++; 
                }
            }

            //from collector to driver
            else if(currOrder->getStatus() == OrderStatus::COLLECTING){
                Volunteer& assignedVolunteer = wareHouse.getNextFreeDriver(*currOrder);
                //check that there is a freee volunteer, if there wasn't- the null volunteer will be gotten
                if(assignedVolunteer.getId() != -1){  
                    //the volunteer takes the order
                    assignedVolunteer.acceptOrder(*currOrder);
                    //the order status is changed
                    currOrder->setStatus(OrderStatus::DELIVERING);
                    //the order driver is changed
                    currOrder->setDriverId(assignedVolunteer.getId());

                    //the order moves to inProcess vector
                    wareHouse.getInProcessOrders().push_back(currOrder);
                    //the order is deleted from Pending vector
                    // Use std::remove_if along with erase to remove the specified Order pointer
                    wareHouse.getPendingOrders().erase(std::remove_if(
                        wareHouse.getPendingOrders().begin(),
                        wareHouse.getPendingOrders().end(),
                        [currOrder](Order* order) { return order == currOrder; }
                    ), wareHouse.getPendingOrders().end());               
                }
                else{
                    //only if we didn't delete the order from the vector we will continue to the next order
                    //because the iterator stays in the same place the deleted order was
                    itPending++; 
                }
            }
        }
        //step on all volunteers
        for(Volunteer* volunteer : wareHouse.getVolunteers()){
            volunteer->step();
        }

        //move orders to relevant vectors if they are done
        //for(Order* currOrder : wareHouse.getInProcessOrders())
        vector<Order*>::iterator itInPro = wareHouse.getInProcessOrders().begin(); 
        while(itInPro != wareHouse.getInProcessOrders().end()){
            Order* currOrder = *itInPro;
            Volunteer *currVolunteer = nullptr;
            if (currOrder->getStatus() == OrderStatus::COLLECTING) {
                currVolunteer = wareHouse.getVolunteerByPointer(currOrder->getCollectorId());
                //if the assigned collector isn't busy- it means he is done with the order
                //if so move to the relevant vector
                if(currVolunteer->getId() != -1 && (!(currVolunteer->isBusy())) ){  
                    //move to pending orders to get a driver assigned
                    wareHouse.getPendingOrders().push_back(currOrder);
                    // Use std::remove_if along with erase to remove the specified Order pointer
                    wareHouse.getInProcessOrders().erase(std::remove_if(
                        wareHouse.getInProcessOrders().begin(),
                        wareHouse.getInProcessOrders().end(),
                        [currOrder](Order* order) { return order == currOrder; }
                    ), wareHouse.getInProcessOrders().end());
                    
                    //check if the volunteer has reached his max orders
                    if (!(currVolunteer->hasOrdersLeft())){
                        wareHouse.getVolunteers().erase(std::remove_if(
                        wareHouse.getVolunteers().begin(),
                        wareHouse.getVolunteers().end(),
                        [currVolunteer](Volunteer* volunteer) { return volunteer == currVolunteer; }
                    ), wareHouse.getVolunteers().end());
                        if (currVolunteer) { //Deletion of the volunteer if reached maxOrders.
                            delete currVolunteer;
                            currVolunteer = nullptr;
                        }
                    }                   
                }
                else{
                    //only if we didn't delete the order from a vector we will continue to the next order
                    //because the iterator stays in the same place the deleted order was
                    itInPro++; 
                }
            }
            
            //if the assigned driver isn't busy- it means he is done with the order
            //move to completed orders 
            else if (currOrder->getStatus() == OrderStatus::DELIVERING) {
                currVolunteer = wareHouse.getVolunteerByPointer(currOrder->getDriverId());
                if (currVolunteer->getId() != -1 && (!(currVolunteer->isBusy()))){
                    wareHouse.getCompletedOrders().push_back(currOrder);
                    // Use std::remove_if along with erase to remove the specified Order pointer
                    wareHouse.getInProcessOrders().erase(std::remove_if(
                        wareHouse.getInProcessOrders().begin(),
                        wareHouse.getInProcessOrders().end(),
                        [currOrder](Order* order) { return order == currOrder; }
                    ), wareHouse.getInProcessOrders().end());

                    //change the order status to completed
                    currOrder->setStatus(OrderStatus::COMPLETED);

                    //check if the volunteer has reached his max orders
                    if (!(currVolunteer->hasOrdersLeft())){
                            wareHouse.getVolunteers().erase(std::remove_if(
                            wareHouse.getVolunteers().begin(),
                            wareHouse.getVolunteers().end(),
                            [currVolunteer](Volunteer* volunteer) { return volunteer == currVolunteer; }
                        ), wareHouse.getVolunteers().end());
                    }
                }
                else{
                    //only if we didn't delete the order from a vector we will continue to the next order
                    //because the iterator stays in the same place the deleted order was
                    itInPro++; 
                }
            }
        }         
        counter --;
    } 
}


std::string SimulateStep::toString() const {
    return "simulateStep " + to_string(numOfSteps) + " " + statusToString();
}

SimulateStep *SimulateStep::clone() const {
    return new SimulateStep(*this);
}


//AddOrder
AddOrder::AddOrder(int id):BaseAction(), customerId (id){}

void AddOrder::act(WareHouse &wareHouse){
    if(wareHouse.doesCostumerExist(customerId) == false)
        this->error("Cannot place this order");
    else{  //only if the costumer exists
        Customer &currCustomer = wareHouse.getCustomer(customerId);
        int orderId = currCustomer.addOrder(wareHouse.getOrderCounter());
        if(orderId == -1) // the costumer didn't add the order
            this->error("Cannot place this order");
        else{
            Order *newOrder = new Order(orderId, customerId, currCustomer.getCustomerDistance());
            wareHouse.addOrder(newOrder);
        }
    }
}

string AddOrder::toString() const { //for PrintActionsLog
    return "order " + to_string(customerId) + " " + statusToString();
}

AddOrder *AddOrder::clone() const{
    return new AddOrder(*this);
}


//AddCustomer
AddCustomer::AddCustomer(string _customerName, string _customerType, int _distance, int _maxOrders) 
    : BaseAction(),
    customerName(_customerName), customerType(AddCustomer::convertStringToEnum(_customerType)), distance(_distance), maxOrders(_maxOrders) {
}
void AddCustomer::act(WareHouse &wareHouse) {
    Customer *newCustomer = nullptr;
    if (static_cast<int>(customerType) == 0) { //Soldier
        newCustomer = new SoldierCustomer(wareHouse.getCustomerCounter(), customerName, distance, maxOrders);
    }
    else { //Civilian
        newCustomer = new CivilianCustomer(wareHouse.getCustomerCounter(), customerName, distance, maxOrders);
    }
    wareHouse.addCustomer(newCustomer);
}
AddCustomer *AddCustomer::clone() const {
    return new AddCustomer(*this);
}
string AddCustomer::toString() const { //for ActionLog
    string result = "customer " + customerName + " " + typeToString()
                    + " " + to_string(distance) + " " + to_string(maxOrders) + " " + statusToString();
    return result;
}
CustomerType AddCustomer::convertStringToEnum(const string& str) {
    if (str == "soldier") {
        return CustomerType::Soldier;
    } else {
        return CustomerType::Civilian;
    }
}
string AddCustomer::typeToString() const{
    if (customerType == CustomerType::Soldier) {return "soldier";}
    else {return "civilian";}
}



//PrintCustomerStatus
PrintCustomerStatus::PrintCustomerStatus(int _customerId): BaseAction(), customerId(_customerId){}

void PrintCustomerStatus::act(WareHouse &wareHouse) {
    if(wareHouse.doesCostumerExist(customerId) == false){ //if the customer doesn't exist
        this->error("Customer doesn't exist"); }
    else{
        Customer &currCustomer = wareHouse.getCustomer(customerId);
        std::cout << "CustomerID: " + to_string(customerId) << endl;
        vector<int> customerOrders = currCustomer.getOrdersIds();
        //prints all the orders of the customer
        for(int currOrder : customerOrders){
            std::cout << (wareHouse.getOrder(currOrder)).toString_PrintCostumer() << endl;
        }
        std::cout << "numOrdersLeft: " + to_string(currCustomer.getMaxOrders() - currCustomer.getNumOrders()) << endl;
    }
}

PrintCustomerStatus *PrintCustomerStatus::clone() const{
    return new PrintCustomerStatus(*this);
}

string PrintCustomerStatus::toString() const {  //for ActionLog
    return "customerStatus " + to_string(customerId) + " " + statusToString();
}

//PrintOrderStatus
PrintOrderStatus::PrintOrderStatus(int id) : BaseAction(), orderId(id) {}
void PrintOrderStatus::act(WareHouse &wareHouse) {
    //Check if id exists
    Order &currOrder = wareHouse.getOrder(orderId);
    if (currOrder.getId() == -1){
        string error = "Order doesn't exist";
        this->error(error);
    }
    /*
    if (!wareHouse.WareHouse::isOrderExist(orderId)) {
        string error = "Order doesn't exist";
        this->error(error);
    } */
    else {
        //Order &order = wareHouse.getOrder(orderId);
        std::cout << currOrder.toString_PrintOrder() << endl;
    }
}
PrintOrderStatus *PrintOrderStatus::clone() const {
    return new PrintOrderStatus(*this);
}
string PrintOrderStatus::toString() const { // For ActionLog
    return "orderStatus " + to_string(orderId) + " " + statusToString();
}



//PrintVolunteerStatus
PrintVolunteerStatus::PrintVolunteerStatus(int id): BaseAction(), VolunteerId(id){}

void PrintVolunteerStatus::act(WareHouse &wareHouse) {
    Volunteer &currVolunteer = wareHouse.getVolunteer(VolunteerId);
    if (currVolunteer.getId() == -1) {
        this->error("Volunteer doesn't exist");
    }
    /*
    if(wareHouse.doesVolunteerExist(VolunteerId) == false)
        this->error("\n Volunteer doesn't exist");
    */
    else{
        //Volunteer &currVolunteer = wareHouse.getVolunteer(VolunteerId);
        std::cout << currVolunteer.toString()<< endl;
    }
}

PrintVolunteerStatus *PrintVolunteerStatus::clone() const {
    return new PrintVolunteerStatus(*this);
}

string PrintVolunteerStatus::toString() const { //for ActionLog
    return "volunteerStatus " + to_string(VolunteerId) + " " + statusToString();
}



// PrintActionsLog
PrintActionsLog::PrintActionsLog() : BaseAction() {}
void PrintActionsLog::act(WareHouse &wareHouse) {
    for (const BaseAction* action : wareHouse.getActions()) {
        std::cout << action->toString() << endl;
    }
}
PrintActionsLog *PrintActionsLog::clone() const {
    return new PrintActionsLog(*this);
}
string PrintActionsLog::toString() const {
    return "log " + statusToString();
}




//Close
Close::Close(): BaseAction(){}  

void Close::act(WareHouse &wareHouse) {
    wareHouse.close();
}

Close *Close::clone() const {
    return new Close(*this);
}

string Close::toString() const {
    return "close " + statusToString();
}


//BackupWareHouse
BackupWareHouse::BackupWareHouse(): BaseAction(){}

void BackupWareHouse::act(WareHouse &wareHouse) {
    if (backup) {
        delete backup;
        backup = nullptr;
    }
    backup = new WareHouse(wareHouse);
}

BackupWareHouse *BackupWareHouse::clone() const {
    return new BackupWareHouse(*this);
}

string BackupWareHouse::toString() const {
    return "backup " + statusToString();
}




// RestoreWareHouse
RestoreWareHouse::RestoreWareHouse() : BaseAction() {}
void RestoreWareHouse::act(WareHouse &wareHouse) {
    if (backup == 0) {
        error("No backup available");
    }
    else {
        wareHouse = *backup;
    }
}
RestoreWareHouse *RestoreWareHouse::clone() const {
    return new RestoreWareHouse(*this);
}
string RestoreWareHouse::toString() const {
    return "restore " + statusToString();
}
 
