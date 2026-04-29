CC 		=	gcc
CFLAGS 		= 	-std=c99 -O0 -Wall -Wextra -Wpedantic
LDFLAGS 	=

SRC_DIR		=	src
SRCS 		=	$(wildcard $(SRC_DIR)/*.c)

TARGET		=	bvm

$(TARGET): $(SRCS)
	$(CC) $(CFLAGS) $(SRCS) -o $(TARGET) $(LDFLAGS)
