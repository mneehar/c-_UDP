CC		= g++
CFLAGS		= -g -c -Wall 
LDFLAGS		= 
SOURCES		= main_client.cpp fuze_udpacceptor.cpp
INCLUDES	= -I.
OBJECTS		= $(SOURCES:.cpp=.o)
TARGET		= client

all: $(SOURCES) $(TARGET)

$(TARGET): $(OBJECTS) 
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.cpp.o:
	$(CC) $(CFLAGS) $(INCLUDES) $< -o $@

clean:
	rm -rf $(OBJECTS) $(TARGET)

