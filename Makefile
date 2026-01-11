LIBS = -lsqlite3
CFLAGS = -Wall -g 
SRCS = src/application.c src/database.c src/main.c
OBJS = $(SRCS:.c=.o)
CC = gcc 
TARGET = library-app

all:$(TARGET)

$(TARGET):  $(OBJS)
	$(CC) $^ -o $@ $(LIBS)

%.o:%.c
	$(CC) $(CFLAGS) -c $< -o $@

run: 
	./library-app

clean:
	rm -f $(OBJS) $(TARGET)


