# Web Server
The web server is the core of this project. It is currently designed as a monolithic application to service requests to scrape HTTP urls and requests to return pre-scraped content.

![Design Diagram](diagram_howell_17d_19_1.png "Design Diagram")

## Component Breakdown

### Web Scraper Main
Entry point for the webserver. Responsible initializing our global variables, initializing the server, and starting the handle requests loop. Responsible for creating and cleaning up the thread pool.

### Web Scraper Server
Uses poll to handle multiple client requests simultaneously. The primary thread of the application runs in the handle loop for the majority of the lifetime of the program. The loop establishes connections and then waits for a connection to be ready. When a connection transmits data the data is dispateched to a handler and the connection is closed. The web scraper server shuts down upon SIGINT.

### Web Scraper Handler
Three handlers
- Queue a URL to be scraped asynchronously and add a task for the thread pool
- Retrieve content from disk for a prescraped URL
- Respond to invalid requests

The queueing handler interacts with the url queue which is interacted with from multiple threads. The url queue is protected with a global mutex from web_scraper_globals.

Here we also perform simple validation to ensure we only queue HTTP urls. HTTPS is not supported.

### Web Scraper Threads
This file is responsible for creating a thread pool and using locks to appropriately add tasks to be dispatched to a thread. The file also contains scrape_url_task which is a task that is dispatched to the thread pool. This task repeatedly processes URLs until there aren't any more URLs to process and then the task ends. The reason for this is to reduce complexity of multiple conditions for waking up throughout the program. We can just wait on the threadpool to be given a task and a thread will be woken up to begin processing. The reason URLs are processed in a loop is for efficiency of not needing to be started for every new url and  so that if the max tasks are reached we can still continue to accept and process URLs, without having to deny client requests.

### Web Scraper Queue
Simple queue data structure that allows O(1) enqueue and dequeue. I chose to keep the queue simple and leave the mutexes to the operations that enqueue and dequeue but it would also be viable to add the mutexes to the queue structure. Key to note is that when a URL is dequeued it is dynamic memory that is returned, the caller must free the URL that is returned.

### Web Scraper Globals
I chose to use a global thread pool and queue mutex for simplicity. I preferred it over needing to plump these items through to the thread functions and the handlers.

### Web Scraper Scraping
This function uses getaddrinfo to perform DNS resolution and simply picks the first IPv4 address that is returned. IPv6 is not supported.
A simple HTTP request is sent over TCP.
Content returned from scraped sites is written to disk on the local machine.

### Web Scraper Utils
A hashing algorithm is used to translate a domain and path into a 20 character + .txt filename. This was chosen as we accept URLs that are ~700 characters long.
Among other things, like using POSIX libraries, web scraper utils makes this application only suitable for Linux-like machines. The path is hardcoded to follow a Linux FS convention.