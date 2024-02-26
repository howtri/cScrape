# Overview
C web server that scrapes requested web pages and stores the contents locally on disk (to a certain amount). Python CLI to make a request to the web server.

## C Web Server
Features: multi-threaded

Accepts:
Request to scrape a webpage
Request for a web pages content

## Python CLI
Use sockets to make requests to the C webserver.

## Push from CLI
git -c http.sslVerify=false clone https://tristan.howell:<PAT>@code.levelup.cce.af.mil/tristan.howell/howell_17d_19_1.git
