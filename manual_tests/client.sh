#!/bin/bash

sleep 5
# Execute the python client multiple times to simulate multiple requests
python3 ../python_cli/src/client.py 2 http://books.toscrape.com/ &
python3 ../python_cli/src/client.py 1 http://books.toscrape.com/ &
sleep 1
python3 ../python_cli/src/client.py 1 http://books.toscrape.com/catalogue/a-light-in-the-attic_1000/index.html &
sleep 1
python3 ../python_cli/src/client.py 1 http://books.toscrape.com/ &
python3 ../python_cli/src/client.py 3 http://books.toscrape.com/ &
python3 ../python_cli/src/client.py 3 a &
python3 ../python_cli/src/client.py 3 &
sleep 1
python3 ../python_cli/src/client.py 1 http://tttexxttbooks.toscrape.com/ &
python3 ../python_cli/src/client.py 2 http://books.toscrape.com/catalogue/tipping-the-velvet_999/index.html &
python3 ../python_cli/src/client.py 1 http://books.toscrape.com/ &
sleep 1
python3 ../python_cli/src/client.py 1 http://books.toscrape.com/catalogue/a-light-in-the-attic_1000/index.html &
sleep 1
python3 ../python_cli/src/client.py 1 http://books.toscrape.com/catalogue/tipping-the-velvet_999/index.html &
python3 ../python_cli/src/client.py 1 http://books.toscrape.com/catalogue/tipping-the-velvet_999/index.html &
python3 ../python_cli/src/client.py 3 http://books.toscrape.com/catalogue/tipping-the-velvet_999/index.html &
python3 ../python_cli/src/client.py 3 a &
python3 ../python_cli/src/client.py 3 &
sleep 1
python3 ../python_cli/src/client.py 1 http://tttexxttbooks.toscrape.com/ &
python3 ../python_cli/src/client.py 2 http://books.toscrape.com/ &
python3 ../python_cli/src/client.py 1 http://books.toscrape.com/ &
sleep 1
python3 ../python_cli/src/client.py 1 http://books.toscrape.com/catalogue/a-light-in-the-attic_1000/index.html &
sleep 1
python3 ../python_cli/src/client.py 1 http://books.toscrape.com/ &
python3 ../python_cli/src/client.py 3 http://books.toscrape.com/ &
python3 ../python_cli/src/client.py 3 a &
python3 ../python_cli/src/client.py 3 &
sleep 1
python3 ../python_cli/src/client.py 1 http://tttexxttbooks.toscrape.com/ &

echo "Client Requests Complete."