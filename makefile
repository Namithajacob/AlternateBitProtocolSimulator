$(shell mkdir -p bin)
$(shell mkdir -p build)

CC=g++
CFLAGS=-std=c++17

INCLUDECADMIUM=-I ../../lib/cadmium

all:build/receiver_text.o build/sender_text.o build/subnet_text.o build/main_text.o build/message.o
	$(CC) -g -o bin/RECEIVER build/receiver_text.o build/message.o
	$(CC) -g -o bin/SENDER build/sender_text.o build/message.o
	$(CC) -g -o bin/SUBNET build/subnet_text.o build/message.o
	$(CC) -g -o bin/main_pg build/main_text.o build/message.o



build/receiver_text.o : test/src/receiver/main.cpp
	$(CC) -g -c $(CFLAGS) $(INCLUDECADMIUM) test/src/receiver/main.cpp -o build/receiver_text.o

build/sender_text.o : test/src/sender/main.cpp
	$(CC) -g -c $(CFLAGS) $(INCLUDECADMIUM) test/src/sender/main.cpp -o build/sender_text.o

build/subnet_text.o : test/src/subnet/main.cpp
	$(CC) -g -c $(CFLAGS) $(INCLUDECADMIUM) test/src/subnet/main.cpp -o build/subnet_text.o

build/main_text.o : src/top_model/main.cpp
	$(CC) -g -c $(CFLAGS) $(INCLUDECADMIUM) src/top_model/main.cpp -o build/main_text.o

build/message.o :
	$(CC) -g -c $(CFLAGS) $(INCLUDECADMIUM) src/message/message.cpp -o build/message.o
	

clean:
	rm -rf build *.o*~
	rm -rf bin *.o*~