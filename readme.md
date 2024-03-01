# Overview
C web server that scrapes requested web pages and stores the contents locally on disk (to a certain amount). Python CLI to make a request to the web server.

## C Web Server
Features: multi-threaded

Accepts:
Request to scrape a webpage
Request for a web pages content

### Architecture
Diagram

Reason for Monolith
- ease of initial development
- not multiple teams/developers for splitting work
- expected low number of qps

## Python CLI
Use sockets to make requests to the C webserver.

## Push from CLI
git -c http.sslVerify=false clone https://<user.name>>:<PAT>@code.levelup.cce.af.mil/tristan.howell/howell_17d_19_1.git

git -c http.sslVerify=false push origin main

## Assumptions
Pages will not be re-scraped upon submission
Pages scraped before can be added to the queue but no changes will take place

## Pages that we can scrape
Not all websites allow web scraping, also we only allow HTTP (not HTTPS).

## Application only works on Linux due to file path processing

##
Operates from Current working dir: /mnt/c/Users/15714/Documents/howell_17d_19_1/howell_17d_19_1/c_web_server
meaning we can use the data dir within it

## Future improvements
Use a hashmap for files that have already been scraped instead of checking if the file exists
if this was much bigger we would likely want to use object storage somewhere else or a volume mounted
for multiple machines so we are able to scrape and return from multiple machines (currnently our app is stateful)

Make filepaths work for for Windows and Linux