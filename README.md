Assignment Definition:
A C++ program that simulates a “Food Warehouse management system”. The program opens the warehouse, assigns volunteers and customers,
assigns orders, handles them, performs steps in the simulation, and more actions.

The Program Flow:
The program receives the path of the config file as the first command-line argument:
string configurationFile=argv[1]
The config file describes the initial state of the warehouse before we start the simulation. It contains info of the initial customers and volunteers.
Once the program starts, it initializes the warehouse according to a given config file, and then
starts the simulation by calling the start() method of Warehouse, followed by printing to the console: “Warehouse is open!“ .
Then, the program waits for the user to enter an action to execute. After each executed action,
the program waits for the next action in a loop.
Generally, the warehouse order handling policy is as follows:

Step 1: Customer places an order. 
Step 2: Collector Volunteer answers the phone and collects the food.
Step 3: Driver Volunteer delivers the food to the customer.
Step 4: Happy customer

Running example:
• Warehouse is open!
• order 17 // There is no customer with customerID=17.
Error: Cannot place this order
• order 0 // Moshe places an order
• orderStatus 0 // Prints the status of orderID = 0
OrderId: 0
OrderStatus: Pending
CustomerID: 0
Collector: None
Driver: None
• order 0
• step 1 // Perform 1 step in the simulation
• orderStatus 0
OrderId: 0
OrderStatus: Collecting
CustomerID: 0
Collector: 0
Driver: None
• orderStatus 1 // Prints the status of orderID = 1
OrderId: 1
OrderStatus: Collecting
CustomerID: 0
Collector: 1
Driver: None
• volunteerStatus 0 // Prints Tamar’s details
VolunteerID: 0
isBusy: True
OrderId: 0
TimeLeft: 1
OrdersLeft: No Limit
• volunteerStatus 1 // Prints Ron’s details
VolunteerID: 1
isBusy: True
OrderId: 1
TimeLeft: 2
OrdersLeft: 1
• step 1 // Perform 1 step in the simulation
• orderStatus 0
OrderId: 0
OrderStatus: Collecting // No driver has received the order yet, only in the next
simulation step!
CustomerID: 0
Collector: 0
Driver: None
• step 1 // Tal(Driver) processed the order and completed it in one step because his
distPerStep > customer distance.
• orderStatus 0
OrderId: 0
OrderStatus: Completed
CustomerID: 0
Collector: 0
Driver: 2
• orderStatus 1
OrderId: 1
OrderStatus: Collecting // No driver has received the order yet, only in the next
simulation step!
CustomerID: 0
Collector: 1
Driver: None
• customerStatus 0 // Prints Moshe’s details
CustomerID 0
OrderId: 0
OrderStatus: Completed
OrderId: 1
OrderStatus: Collecting
numOrdersLeft: 0
• order 1
• order 1
Error: Cannot place this order // Ron has maxOrders=1;
• log // Prints all actions and their ActionStatus ,excluding ‘log’ itself
order 17 ERROR
order 0 COMPLETED
orderStatus 0 COMPLETED
order 0 COMPLETED
simulateStep 1 COMPLETED
orderStatus 0 COMPLETED
orderStatus 1 COMPLETED
volunteerStatus 0 COMPLETED
volunteerStatus 1 COMPLETED
simulateStep 1 COMPLETED
orderStatus 0 COMPLETED
simulateStep 1 COMPLETED
orderStatus 0 COMPLETED
orderStatus 1 COMPLETED
customerStatus 0 COMPLETED
order 1 COMPLETED
order 1 ERROR
• close //Closes the warehouse and prints all orders status
OrderID: 1, CustomerID: 0, Status: Collecting
OrderID: 2, CustomerID: 1, Status: Pending
OrderID: 0, CustomerID: 0, Status: Completed
