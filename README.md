posix-file-sharing
==================

Simple File Sharing implemented in C++/Posix

Socket_Attivo and Socket_Passivo:
---------------------------------
A Passive Socket is created by a Server which listens for Client's requests. 
Calling the accetta() method on this object a reference to an Active Socket object is returned. 
The exchange of information will take place on this object. 
The class also provide methods like getIP() and getPort(). These methods return the IP and the PORT on which the passive socket is created. 
In the Active Socket class, methods to allow the exchange of files and messages have been implemented.

Interpreter:
------------
The Interpreter is a set of utility methods that allow you to recognize the various parts of a message extracted from a socket.
It creates an enumeration to distinguish the various types of commands implemented in the system. 
To split the string extracted from the Socket, the interpreter uses a reentrant string tokenizer (strtok_r) that returns the meaningful parts of the msg.

Repository:
-----------
The repository has been implemented as a multilevel structure. 
In particular, it contains a list of File_Location. Each File_Location contains a list of Location.
A File_Location associates a file name to a list of locations where the user can download it.

A Location is an IP:PORT object which identifies the FS (File Server) where the file is located. 
In addition, each Location has a semaphore (maxClient) which is used to limit file simultaneous downloads from a FS.
In practice, when a file request is made to the Repository, a client must acquire the semaphore maxClient. 
When the client completes the download, it will release the maxClient semaphore.

Reservation list:
-----------------
When a File is not found a Client can ask a Reservation.
A Reservation List maintains a list of File_Condition. 
It provides two methods: addReservation(), which suspends the thread on a File_Condition and serverReservation(),
which awakens all the threads blocked on the File_Condition of the particular file that now is ready to be downloaded.
A File_Condition is an object that associates the name of the file to a semaphore on which a Thread will be suspended until the file is available.

DFR (Direction Forward Routing):
--------------------------------
The DFR class contains:
  - A Repository which will save the File-locations (File Server FS IP:PORT)
  - A Reservation list in order to manage reservation requests for not-found-file
  - Two Passive_Socket in order to listen Client Requests and FS (File Server) Requests.

Two threads will cyclically listen on these passive sockets.
For each contact received from a client or from a FS the DFR creates a new thread and a new Active_Socket completely dedicated to the new contact.

Client:
-------
A Client contains two Active_Sockets in order to communicate with the DFR and to download fromthe FS.
The download is automatically created as soon as the client receives the location of the researched file.
Client provides also a method to request a reservation in case the the requested file is not found.

FS (File Server):
-----------------
A File Server contains a Passive_Socket on which it will listen for client requests
It also contains an Active_Socket in order to connect to the DFR.
A FS is able to register and unregister a file on the DFR repository
A Thread will cyclically listen for client download requests. 
Once a request is accepted, the FS creates a new thread and a new Active_Socket completely dedicated to the new contact and it will proceed with sending the requested file.
