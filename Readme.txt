--MT2021030
--Bhavya Vijayvergiya
--SS Mini Project

####################################            Online Railway Ticket Booking System            ########################################

This project uses tcp connection to communicate between client and server to perform ticket Booking operations.   
 
client.c===== This file is client side connection file which represents the client operations.  
server.c===== This file is server side connection file which fetches data from client.c and perform subsequent operations on it .
  
util.h======= In this all the client side functions are stored.  
serutil.h==== In this all the server side functions are stored.. 
struc.h====== It includes the basic structures of account, train and booking.
 
db/accounts== File contains account related information of:  1.customer
								2.agent
								3.admin  
db/train===== File contains all train related information 
db/bookings== File contains all bookings related information  


NOTE :Also the admin account is protected so you have to go through pin verification  -->> PIN:"1111"

before execution of project create certain files using shell commands
 >> mkdir db
 >> touch db/train
 >> touch db/booking
 
 >> mkdir db/accounts
 >> touch db/accounts/admin
 >> touch db/accounts/customer
 >> touch db/accounts/agent

Compile the server.c and client.c files as
 >> gcc server.c -o ser
 >> gcc client.c -o cli
 

