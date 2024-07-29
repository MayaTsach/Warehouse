# Food Warehouse Management System

## Overview
This project is a simulation of a Food Warehouse Management System in C++. The system is designed to help manage a warehouse that distributes food packages to citizens. The warehouse is operated by volunteers, and the system handles orders, processes them, and simulates various actions within the warehouse.

## Project Structure
The project is structured as follows:
- **Warehouse**: Manages volunteers, customers, and orders.
- **Customer**: Represents different types of customers with unique IDs.
- **Volunteer**: Abstract class for different volunteer roles (Collector and Driver).
- **Order**: Represents an order with various statuses.
- **BaseAction**: Abstract class for different actions that can be performed in the warehouse.

## Classes and Their Responsibilities
### Warehouse
- Manages lists of volunteers, customers, and orders.
- Handles different types of orders: Pending, In Process, and Completed.
- Provides unique IDs for customers and volunteers.

### Customer
- Represents customers with unique IDs.
- Two types: Soldier and Civilian.

### Volunteer
- Abstract class for different volunteer roles.
- Roles include Collector and Driver, each with specific responsibilities and limitations.

### Order
- Represents an order with a unique ID and status.
- Order statuses include Pending, Collecting, Delivering, and Completed.

### BaseAction
- Abstract class for actions performed in the warehouse.
- Actions include creating orders, customers, and performing simulation steps.

## Program Flow
1. The program receives the path of the config file as the first command-line argument.
2. Initializes the warehouse according to the config file.
3. Starts the simulation and waits for user actions.
4. Handles actions such as placing orders, creating customers, checking order status, and performing simulation steps.

## Actions
### SimulateStep
- Moves the simulation one unit of time forward.
- Updates the status of orders and volunteers.

### AddOrder
- Creates a new order for a customer.

### AddCustomer
- Creates a new customer and stores them in the warehouse.

### PrintOrderStatus
- Prints the status of a specific order.

### PrintCustomerStatus
- Prints the status of a specific customer.

### PrintVolunteerStatus
- Prints the status of a specific volunteer.

### PrintActionsLog
- Prints a log of all actions performed.

### Close
- Prints all orders with their status and closes the warehouse.

### BackupWarehouse
- Saves the current state of the warehouse.

### RestoreWarehouse
- Restores the warehouse to a previously saved state.


## Input File Format
The input file describes the initial state of the warehouse, including customers and volunteers. Each line follows a specific pattern:
- **Customer**: `customer <customer_name> <customer_type> <customer_distance> <max_orders>`
- **Volunteer**: `volunteer <volunteer_name> <volunteer_role> <volunteer_coolDown>/<volunteer_maxDistance> <distance_per_step>(for drivers only) <volunteer_maxOrders>(optional)`
