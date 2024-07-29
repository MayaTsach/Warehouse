#pragma once
#include <string>
#include <vector>
using namespace std;

using std::string;
using std::vector;


#include "Order.h"
#include "Customer.h"
#include "Volunteer.h"

class BaseAction;
class Volunteer;
class Order;
class Customer;

// Warehouse responsible for Volunteers, Customers Actions, and Orders.


class WareHouse {

    public:

        
        WareHouse(const string &configFilePath);
        void start();
        void addOrder(Order* order);
        void addAction(BaseAction* action);
        Customer &getCustomer(int customerId) const;
        Volunteer &getVolunteer(int volunteerId) const;
        Volunteer *getVolunteerByPointer(int volunteerId) const;
        Order &getOrder(int orderId) const;
        const vector<BaseAction*> &getActions() const;
        void close();
        void open();

        //Additional
        int getCustomerCounter() const;
        int getVolunteerCounter() const;
        int getOrderCounter() const;

        Volunteer &getNextFreeCollector(const Order &order) ; //gets the next available to take an order collector
        Volunteer &getNextFreeDriver(const Order &order) ;  //gets the next available to take an order driver

        void addCustomer(Customer* customer);
        void addVolunteer(Volunteer* volunteer);
        bool doesCostumerExist(int id);
        bool doesVolunteerExist(int id);
		bool isOrderExist(int id);
        void parse(const string &input);

        vector<Order*>& getPendingOrders();
        vector<Order*>& getInProcessOrders();
        vector<Order*>& getCompletedOrders();
        vector<Customer*>& getCustomers();
        vector<Volunteer*>& getVolunteers();


        // Rule of 5
        virtual ~WareHouse();
        WareHouse(const WareHouse& other);
        WareHouse& operator=(const WareHouse& other);
        WareHouse(WareHouse&& ohter);
        WareHouse& operator=(WareHouse&& other);


    private:
        bool isOpen;
        vector<BaseAction*> actionsLog;
        vector<Volunteer*> volunteers;
        vector<Order*> pendingOrders;
        vector<Order*> inProcessOrders;
        vector<Order*> completedOrders;
        vector<Customer*> customers;
        int customerCounter; //For assigning unique customer IDs
        int volunteerCounter; //For assigning unique volunteer IDs
        int orderCounter; //For assigning unique order IDs

        //will be used with id -1 for cases there is no member as we searched
        Volunteer *nullVolunteer;
        Customer *nullCustomer;
        Order *nullOrder;

        bool isConfigFile;

        //Additional
        void parse_simulateStep(const string &remainingData);
        void parse_addOrder(const string &remainingData);
        void parse_addCustomer(const string &remainingData);
        void parse_printOrderStatus(const string &remainingData);
        void parse_printCustomerStatus(const string &remainingData);
        void parse_printVolunteerStatus(const string &remainingData);
        void parse_printActionLog();
        void parse_Close();
        void parse_backupWareHouse();
        void parse_restoreWareHouse();

        void parse_addVolunteer(const string &remainingData);

        // Rule of 5:
        void clean();
        void toNullPtr();

};