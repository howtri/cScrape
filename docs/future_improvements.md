# Future Improvements and Issues

### Allow HTTPS
Use openSSL or some other library to allow scraping of HTTPS.

### Rescrape Submissions in some cases
Currently once a website has been scraped and is stored on disk we will not scrape it again. This is likely not always optimal.

### Application only works on Linux
Due to pthread requirements and other libraries this is likely not going to change. However, in the event it does a more robust path solution is required.

### Write path is fragile
We rely on the applications current working directory to ensure we are able to write and read from the disk directory.

### Evaluate limits on max connections and max tasks
We can likely increase both limits.

### Security Considerations
Do extra research to ensure path traversal is not possible. I don't think it is as we only pass the url to the send syscall but its worth further investigation.

### Limit to URL queue
There is a scenario where the URL queue is flooded with requests and we are signaled to shutdown. In this case the application would continue processing requests for a while, which may not be desirable.

### Logging
Currently information is only sent to stderr and stdout. Implement more sophisticated logging.

### Thread Statistics
Currently threads don't return any information on join. Consider returning statistics or some other data.

### Network Communication
Relying on the full buffer being sent to end communication is fragile. Consider sending the amount of bytes that are being sent as part of the message.

### Add more thread tasks
Currently, only web scraping is done by the threads. Consider passing each handler function to a thread and having the thread take ownership of the socket.

### Hashmap for already scraped websites
We currently check if a file exists to determine if a website has been scraped and there is content. Checking disk takes significantly longer than checking if the the file is available in a hashmap O(1) access. This disregards OS tricks that may make checking very fast.

### Regex
Parse URLs with Regex to improve on the simple check starts with http://
