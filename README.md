# InterfaceProject
An Automated Payment System for Public Transport

## ServerSide
A PHP/Laravel application to handle the server side requests made by the system.
Is a simple RESTful service that handles:
* Authentication
* Validation
* Transactions
* Database management

## ArduinoClient
The Arduino code, represents the client side of the system.
Handles:
* Creating an encrypted secure tunnel with the server side.
* Sending credintials to authenticate with the server.
* Sending transactional requests in a REST format using HTTP requests, mainly `HTTP/GET`.

## Schemas
The schemes of the system, how the physical modules connect to each other. *__Hardware side__*

## Datasheets
The datasheets of the components used, like ... duh.
