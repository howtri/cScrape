# Overview
This project has two types of testing included, unit tests and manual tests (in the form of bash scripts).
The unit tests are run in Gitlab CI and use CMocka for mocking syscalls.

## CMocka
CMocka was chosen to run the unittests as a web server/web scraper has a large amount of interactions with the OS. As you can see from the Unit tests our coverage isn't completely there. However, it sets a baseline and tests some critical aspects. The handlers, the queue, and the utils. These tests are located in c_web_server/tests. The command to run these tests can be found in the readme

## Manual Tests
I would like to have integration tests for this project, but I currently don't have the time or resources to implement them. To bridge this gap of testing the project as a whole I've created scripts within manual_tests. These tests give me confidence about running the web server with many requests in a repeatable form. These tests have only been run on an Ubunutu machine locally. Please see instructions.md in manual_tests for information on how to run them.

## Future
Please see future_improvements.md for more on future testing.