g++ *.cpp -lboost_system -lpthread -o smtp_server

telnet 127.0.0.1 25000
HELO localhost

MAIL FROM:<a@test.com>

RCPT TO:<b@test.com>

DATA

Hello world

.
