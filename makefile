CC =			gcc
CFLAGS =	-std=c99 -O0 -Wall -Wextra -Wpedantic
LDFLAGS = 

SRC_DIR =	src 
SRCS 		=	$(wildcard $(SRC_DIR)/.c)

TARGET 	= argb

$(TARGET): $(SRCS)
	$(CC) $(CFLAGS) -o $(TARGET) $(LDFLAGS)
