#include <string>
#include <vector>
using namespace std;

#include <iostream>
#include <sstream>
#include <fstream>
using std::string;
using std::vector;
using std::istringstream;

#include "../include/Order.h"
#include "../include/Customer.h"
#include "../include/Action.h"
#include "../include/Volunteer.h"

WareHouse::WareHouse(const string &configFilePath)
    :   isOpen(true),
        actionsLog(), 
        volunteers(),
        pendingOrders(),
        inProcessOrders(),
        completedOrders(),
        customers(),
        customerCounter(0),
        volunteerCounter(0),
        orderCounter(0),
        nullVolunteer(new CollectorVolunteer(-1, "noVolunteer", -1)),
        nullCustomer(new SoldierCustomer(-1, "noCustomer", -1, -1)),
        nullOrder(new Order(-1, -1, -1)),
        isConfigFile(true)
{
    std::ifstream configFile(configFilePath);

    if (!configFile.is_open()) {
        std::cerr << "Error opening config file: " << configFilePath << std::endl;
        return;
    }

    std::string line;
    while (std::getline(configFile, line)) {
        if (line.empty() || line[0] == '#') {
            // Skip empty lines or lines starting with #
            continue;
        }

        parse(line);
    }

    configFile.close();

}

void WareHouse::start() {
    isConfigFile = false;
    std::cout << "Warehouse is open!" << std::endl;
    string input;
    while (isOpen) {
        std::getline(std::cin, input);
        parse(input);
    }
}

//to be continued
void WareHouse::addOrder(Order* order){
    pendingOrders.push_back(order);
    orderCounter++;
}

void WareHouse::addAction(BaseAction* action) {
    actionsLog.push_back(action);
}
Customer &WareHouse::getCustomer(int customerId) const {
        for(Customer* currCustomer : customers){
            if(currCustomer->getId() == customerId){
                return *currCustomer;
            }
    }
    return *nullCustomer;
}

Volunteer &WareHouse::getVolunteer(int volunteerId) const {
    for(Volunteer* currVolunteer : volunteers){
        if(currVolunteer->getId() == volunteerId){
            return *currVolunteer;
        }
    }
    return *nullVolunteer;
}

Volunteer *WareHouse::getVolunteerByPointer(int volunteerId) const {
    for(Volunteer* currVolunteer : volunteers){
        if(currVolunteer->getId() == volunteerId){
            return currVolunteer;
        }
    }
    return nullptr;
}

Order &WareHouse::getOrder(int orderId) const {
    for (Order* order : pendingOrders) {
        if (order->getId() == orderId) {
            return *order;
        }
    }

    for (Order* order : inProcessOrders) {
        if (order->getId() == orderId) {
            return *order;
        }
    }

    for (Order* order : completedOrders) {
        if (order->getId() == orderId) {
            return *order;
        }
    }
    return *nullOrder;
}

const vector<BaseAction*> &WareHouse::getActions() const {
    return actionsLog;
}

void WareHouse::close(){
    //print all orders
    for(Order* currOrder : pendingOrders){
        std::cout << currOrder->toString() << endl;
    }
    for(Order* currOrder : inProcessOrders){
        std::cout << currOrder->toString() << endl;
    }
    for(Order* currOrder : completedOrders){
        std::cout << currOrder->toString() << endl;
    }
    isOpen = false;
}

void WareHouse::open() {
    isOpen = true;
}

//Additional
int WareHouse::getCustomerCounter() const {
    return customerCounter;
}
int WareHouse::getVolunteerCounter() const {
    return volunteerCounter;
}
int WareHouse::getOrderCounter() const {
    return orderCounter;
}

Volunteer &WareHouse::getNextFreeCollector(const Order &order) {
    for (Volunteer* volunteer : volunteers) {
        //checks that the volunteer is a collector and free to take the order
        if (volunteer->getVolunteerType() == "Collector" && volunteer->canTakeOrder(order)) {
            return *volunteer;
        }
    }
    return *nullVolunteer; 
}

Volunteer &WareHouse::getNextFreeDriver(const Order &order) {
    for (Volunteer* volunteer : volunteers) {
        //checks that the volunteer is a driver and free to take the order
        if (volunteer->getVolunteerType() == "Driver" && volunteer->canTakeOrder(order)) { 
            return *volunteer;
        }
    }
    return *nullVolunteer; 
}


void WareHouse::addCustomer(Customer* newCustomer) {
    customers.push_back(newCustomer);
    customerCounter++;
}
void WareHouse::addVolunteer(Volunteer* volunteer) {
    volunteers.push_back(volunteer);
    volunteerCounter++;
}

bool WareHouse::doesCostumerExist(int id){
    if(id < customerCounter) return true;
    return false;
}

bool WareHouse::doesVolunteerExist(int id){
    //runs on the volunteers vector and searches for the volunteer id given
    for(Volunteer* currVolunteer : volunteers){
        if(currVolunteer->getId() == id)
            return true;
    }
    return false;
}
bool WareHouse::isOrderExist(int id) {
    for (const Order* order : pendingOrders) {
        if (order->getId() == id) {
            return true;  // Order exists
        }
    }

    for (const Order* order : inProcessOrders) {
        if (order->getId() == id) {
            return true;  // Order exists
        }
    }

    for (const Order* order : completedOrders) {
        if (order->getId() == id) {
            return true;  // Order exists
        }
    }

    return false;
}    

void WareHouse::parse(const string &input) {
    istringstream iss(input);
    string action;
    
    // Read the first word to determine the action
    iss >> action;

    // Use the remaining part of the string for further processing
    string remainingData;
    getline(iss, remainingData);

    // Perform actions based on the first word
    
    if (action == "step") {
        parse_simulateStep(remainingData);
    }
    else if (action == "order") {
        parse_addOrder(remainingData);
    }
    else if (action == "customer") {
        parse_addCustomer(remainingData);
    }
    else if (action == "orderStatus") {
        parse_printOrderStatus(remainingData);
    }
    else if (action == "customerStatus") {
        parse_printCustomerStatus(remainingData);
    }
    else if (action == "volunteerStatus") {
        parse_printVolunteerStatus(remainingData);
    }
    else if (action == "log") {
        parse_printActionLog();
    }
    else if (action == "close") {
        parse_Close();
    }
    else if (action == "backup") {
        parse_backupWareHouse();
    }
    else if (action == "restore") {
        parse_restoreWareHouse();
    }
    else if (action == "volunteer") {
        parse_addVolunteer(remainingData);
    }
}


//vectors getters

vector<Order*> &WareHouse::getPendingOrders(){
    return pendingOrders;
}
vector<Order*> &WareHouse::getInProcessOrders(){
    return inProcessOrders;
}
vector<Order*> &WareHouse::getCompletedOrders(){
    return completedOrders;
}
vector<Customer*> &WareHouse::getCustomers(){
    return customers;
}
vector<Volunteer*> &WareHouse::getVolunteers(){
    return volunteers;
}



// Rule of 5
WareHouse::~WareHouse() {
    // Destruct each vector:
    // ActionsLog:
    for (BaseAction* action : actionsLog) {
        if (action) {
            delete action;
            action = nullptr;
        }
    }

    // volunteers
    for (Volunteer* volunteer : volunteers) {
        if (volunteer) {
            delete volunteer;
            volunteer = nullptr;
        }
    }

    // pendingOrders
    for (Order* pending : pendingOrders) {
        if (pending) {
            delete pending;
            pending = nullptr;
        }
    }
    //inProcessOrders
    for (Order* inProcess : inProcessOrders) {
        if (inProcess) {
            delete inProcess;
            inProcess = nullptr;
        }
    }

    // completedOrders:
    for (Order* completed : completedOrders) {
        if (completed) {
            delete completed;
            completed = nullptr;
        }
    }

    // customers
    for (Customer* customer : customers) {
        if (customer) {
            delete customer;
            customer = nullptr;
        }
    }

    //null members
    delete nullVolunteer;
    delete nullCustomer;
    delete nullOrder;
}



// Copy constructor
WareHouse::WareHouse(const WareHouse& other) 
    :   isOpen(other.isOpen),
        actionsLog(),
        volunteers(),
        pendingOrders(),
        inProcessOrders(),
        completedOrders(),
        customers(),
        customerCounter(other.customerCounter),
        volunteerCounter(other.volunteerCounter),
        orderCounter(other.orderCounter),
        nullVolunteer(new CollectorVolunteer(-1, "noVolunteer", -1)),
        nullCustomer(new SoldierCustomer(-1, "noCustomer", -1, -1)),
        nullOrder(new Order(-1, -1, -1)),
        isConfigFile(other.isConfigFile)
{
    // ActionsLog:
    for (BaseAction* action : other.actionsLog) {
        actionsLog.push_back(action->clone());
    }

    // volunteers
    for (Volunteer* volunteer : other.volunteers) {
        volunteers.push_back(volunteer->clone());
    }

    // pendingOrders
    for (Order* pending : other.pendingOrders) {
        pendingOrders.push_back(pending->clone());
    }
    //inProcessOrders
    for (Order* inProcess : other.inProcessOrders) {
        inProcessOrders.push_back(inProcess->clone());
    }

    // completedOrders:
    for (Order* completed : other.completedOrders) {
        completedOrders.push_back(completed->clone());
    }

    // customers
    for (Customer* customer : other.customers) {
        customers.push_back(customer->clone());
    }
}

//move constructor
WareHouse::WareHouse(WareHouse&& other) 
    :   isOpen(other.isOpen),
        actionsLog(),
        volunteers(),
        pendingOrders(),
        inProcessOrders(),
        completedOrders(),
        customers(),
        customerCounter(other.customerCounter),
        volunteerCounter(other.volunteerCounter),
        orderCounter(other.orderCounter),
        nullVolunteer(new CollectorVolunteer(-1, "noVolunteer", -1)),
        nullCustomer(new SoldierCustomer(-1, "noCustomer", -1, -1)),
        nullOrder(new Order(-1, -1, -1)),
        isConfigFile(other.isConfigFile)
{
    if(this!= &other){

        //for each vector swap the vector with the pointers values
        actionsLog.swap(other.actionsLog);
        volunteers.swap(other.volunteers);
        pendingOrders.swap(other.pendingOrders);
        inProcessOrders.swap(other.inProcessOrders);
        completedOrders.swap(other.completedOrders);
        customers.swap(other.customers);

        //reset to nullptr the pointers inside the vector of other, so they won't be deleted in it's destructor
        other.toNullPtr();
    }
}

//copy assignment operator
WareHouse &WareHouse::operator=(const WareHouse& other){
    if(this != &other){
        isOpen = other.isOpen;
        customerCounter = other.customerCounter;
        volunteerCounter = other.volunteerCounter;
        orderCounter = other.orderCounter;
        isConfigFile = other.isConfigFile;

        //clear data of the vectors
        this->clean();

        //copy all the vector's values (except the null members which are the same)
        for(BaseAction* curr : other.actionsLog){
            actionsLog.push_back(curr->clone());
        }
        for(Volunteer* curr : other.volunteers){
            volunteers.push_back(curr->clone());
        }
        for(Order* curr : other.pendingOrders){
            pendingOrders.push_back(curr->clone());
        }
        for(Order* curr : other.inProcessOrders){
            inProcessOrders.push_back(curr->clone());
        }  
        for(Order* curr : other.completedOrders){
            completedOrders.push_back(curr->clone());
        }
        for(Customer* curr : other.customers){
            customers.push_back(curr->clone());
        }  
    }
    return *this;
}

// Move Assignment operator
WareHouse &WareHouse::operator=(WareHouse&& other) {
    if (this != &other) {
        clean();
        
        isOpen = other.isOpen;
        // Swap the content of vectors
        actionsLog.swap(other.actionsLog);
        volunteers.swap(other.volunteers);
        pendingOrders.swap(other.pendingOrders);
        inProcessOrders.swap(other.inProcessOrders);
        completedOrders.swap(other.completedOrders);
        customers.swap(other.customers);

        customerCounter = other.customerCounter;
        volunteerCounter = other.volunteerCounter;
        isConfigFile = other.isConfigFile;
        
        other.toNullPtr();
    }
    return *this;
}
        
//Additional

void WareHouse::parse_simulateStep(const string &remainingData) {
        std::istringstream iss(remainingData);
    int steps;

    iss >> steps;

    //make sure the that has to be allocated dynamically - and whether should have a destructor in each class

    // Dynamically allocate a SimulateStep object on the heap
    SimulateStep *step = new SimulateStep(steps);

    // Call the act method
    step->act(*this);

    actionsLog.push_back(step); // Addition to ActionLog
}
void WareHouse::parse_addOrder(const string &remainingData) {
    std::istringstream iss(remainingData);
    int id;

    iss >> id;

    /*
    AddOrder add = AddOrder(id);
    add.act(*this);
    */
    
    // Dynamically allocate a SimulateStep object on the heap
    AddOrder *newOrder = new AddOrder(id);

    // Call the act method
    newOrder->act(*this);

    actionsLog.push_back(newOrder); // Addition to ActionLog

}
void WareHouse::parse_addCustomer(const string &remainingData) {
    std::istringstream iss(remainingData);
    std::string name, type;
    int distance, maxOrders; 

    // Parse the remaining data for a customer
    iss >> name >> type >> distance >> maxOrders;  

    if (!isConfigFile) { //addition is called by user
        AddCustomer *newCustomer = new AddCustomer(name, type, distance, maxOrders);
        newCustomer->act(*this);
        actionsLog.push_back(newCustomer); //Adding the action to actions log
    }
    else { //addition is called by config file
        Customer* newCustomer = nullptr;
        if (type == "soldier") {
            newCustomer = new SoldierCustomer(getCustomerCounter(), name, distance, maxOrders);
        }
        else { // type is civilian
            newCustomer = new CivilianCustomer(getCustomerCounter(), name, distance, maxOrders);
        }
        addCustomer(newCustomer);
    }                                       
}

void WareHouse::parse_printOrderStatus(const string &remainingData) {
    std::istringstream iss(remainingData);
    int orderId;
    iss >> orderId;
    PrintOrderStatus *POS = new PrintOrderStatus(orderId);
    POS->act(*this);
    actionsLog.push_back(POS);
}
void WareHouse::parse_printCustomerStatus(const string &remainingData) {
    std::istringstream iss(remainingData);
    int customerId;
    iss >> customerId;
    PrintCustomerStatus *PCS = new PrintCustomerStatus(customerId);
    PCS->act(*this);
    actionsLog.push_back(PCS);
}
void WareHouse::parse_printVolunteerStatus(const string &remainingData) {
    std::istringstream iss(remainingData);
    int volunteerId;
    iss >> volunteerId;
    PrintVolunteerStatus *PVS = new PrintVolunteerStatus(volunteerId);
    PVS->act(*this);
    actionsLog.push_back(PVS);
}
void WareHouse::parse_printActionLog() {
    PrintActionsLog *PAL = new PrintActionsLog();
    PAL->act(*this);
    actionsLog.push_back(PAL);
}
void WareHouse::parse_Close() {
    Close *clo = new Close();
    clo->act(*this);
    actionsLog.push_back(clo);
}
void WareHouse::parse_backupWareHouse() {
    BackupWareHouse *BUWH = new BackupWareHouse();
    BUWH->act(*this);
    actionsLog.push_back(BUWH);
}
void WareHouse::parse_restoreWareHouse() {
    RestoreWareHouse *RWH = new RestoreWareHouse();
    RWH->act(*this);
    actionsLog.push_back(RWH);
}
void WareHouse::parse_addVolunteer(const string &remainingData) {
    std::istringstream iss(remainingData);
    std::string name, role;

    // Parse the remaining data for a volunteer up to the role
    iss >> name >> role;

    // Stop parsing here and condition based on the role
    //BaseAction *action = nullptr;
    Volunteer* newVolunteer;

    if (role == "collector") {
        int coolDown;
        iss >> coolDown;
        
        newVolunteer = new CollectorVolunteer(getVolunteerCounter(), name, coolDown);
        
        //action = new AddCollector(name, coolDown);
    }
    else if (role == "limited_collector") {
        int coolDown, maxOrders;
        iss >> coolDown >> maxOrders;
        
        newVolunteer = new LimitedCollectorVolunteer(getVolunteerCounter(), name, coolDown, maxOrders);
        
        //action = new AddLimitedCollector(name, coolDown, maxOrders);
    }
    else if (role == "driver") {
        int maxDistance, distance_per_step;
        iss >> maxDistance >> distance_per_step;
        
        newVolunteer = new DriverVolunteer(getVolunteerCounter(), name, maxDistance, distance_per_step);

        //action = new AddDriver(name, maxDistance, distance_per_step);
    }
    else if (role == "limited_driver") {
        int maxDistance, distance_per_step, maxOrders;
        iss >> maxDistance >> distance_per_step >> maxOrders;

        newVolunteer = new LimitedDriverVolunteer(getVolunteerCounter(), name, maxDistance, distance_per_step, maxOrders);

    //action = new AddLimitedDriver(name, maxDistance, distance_per_step, maxOrders);
    }

    addVolunteer(newVolunteer);

/*action->act(*this);
delete action; */            
}

void WareHouse::clean() {
    // ActionsLog:
    for (BaseAction* action : actionsLog) {
        delete action;
    }
    actionsLog.clear();

    // volunteers
    for (Volunteer* volunteer : volunteers) {
        delete volunteer;
    }
    volunteers.clear();

    // pendingOrders
    for (Order* pending : pendingOrders) {
        delete pending;
    }
    pendingOrders.clear();

    //inProcessOrders
    for (Order* inProcess : inProcessOrders) {
        delete inProcess;
    }
    inProcessOrders.clear();

    // completedOrders:
    for (Order* completed : completedOrders) {
        delete completed;
    }
    completedOrders.clear();

    // customers
    for (Customer* customer : customers) {
        delete customer;
    }
    customers.clear();

}

void WareHouse::toNullPtr() {
    // ActionsLog:
    for (BaseAction*& action : actionsLog) {
        action = nullptr ;  
    }
    //actionsLog.clear();

    // volunteers
    for (Volunteer*& volunteer : volunteers) {
        volunteer = nullptr;
    }

    // pendingOrders
    for (Order*& pending : pendingOrders) {
        pending = nullptr;
    }
    //inProcessOrders
    for (Order*& inProcess : inProcessOrders) {
        inProcess = nullptr;
    }

    // completedOrders:
    for (Order*& completed : completedOrders) {
        completed = nullptr;
    }

    // customers
    for (Customer*& customer : customers) {
        customer = nullptr;
    }
}
