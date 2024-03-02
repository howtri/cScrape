# Overview
Manual tests allows us to run the server locally and simulate many requests from the client. This ensures the server can hold up to multiple requests and multiple QPS. It also gives us an easy way to throw many requests at the server and ensure it doesnt crash or other unexpected behavior occurs.

## Server.sh
Server.sh starts the web server and uses the client.sh file to send many requests to the server. This results in real requests going out to http://books.toscrape.com/. This should be a minimal amount given that the web server only scrapes a website once and will skip future requests for the same website + path combo. But still be careful. Also some sites don't allow webscrapers so please ensure the site you are scraping allows web scrapers if you change client.sh.

Run:
```
./server.sh
```

## Valgrind.sh

Run:
```
./valgrind.sh
```