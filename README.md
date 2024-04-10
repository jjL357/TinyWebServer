# TinyWebServer 
A network backend server built in C++
##  Project Overview
- Implement a multi-threaded Reactor high-concurrency model using I/O multiplexing technology epoll and thread pool.

- Parse HTTP request messages using regular expressions and state machines, handle requests for static resources, and send response messages. 

- Encapsulate char using standard library containers to implement automatically expanding buffers.
  
- Implement a timer based on a min heap to close inactive connections that have timed out. 

- Implement an asynchronous logging system using the singleton pattern and blocking queues to record the server's operational status.

- Implement a database connection pool using the RAII mechanism to reduce the overhead of establishing and closing database connections, while also implementing user registration and login functionality. 