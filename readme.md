# Overview
howell_17d_19_1 is a tool that consists of a C Web Server and a Python Client to interact with the Web Server. The Web Server accepts requests from the Client and returns a response based on one of three routes. The first route allows a client to request a HTTP website be scraped asynchronously, the second is a request for the content of a HTTP website scraped in the past, and the third is for invalid requests. The C web server can handle multiple client connections simultaneously and dispatches scraping requests to a pool of threads.

## Usage

Required tools:
```
git gcc make cmake cmocka-dev
```

If testing is not desired then only the following are required

```
gcc make
```

Running the Web Server
```
make -C c_web_server/
c_web_server/bin/web_scraper
```

Making requests to the already running webserver from the Client
```
python3 python_cli/src/client.py 2 http://books.toscrape.com/
python3 python_cli/src/client.py 1 http://books.toscrape.com/
```

## Testing
Unit tests are implemented using a third party library, CMocka. This library is used to mock syscalls and other custom functions during unit tests.

```
make -C c_web_server/ test
```

## Cleaning
Clean the directories after testing.

```
make -C c_web_server/ test
```

## More Information
See the documentation folder!