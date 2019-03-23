CC 				= gcc
#CC				= arm-linux-gnueabi-gcc
CFLAGS 			= -O3 -g -std=gnu99 -W -Wall -Wno-comment
INCLUDES 		= -I./include/
LDFLAGS 		= -L./libraries -lrt -lm
BUILD_DIR 		= ./build
SOURCE_DIR 		= ./../source
INCLUDE_DIR		= ./../include
BIN				= ./main

OBJS = $(BUILD_DIR)/bigint.o

all: main

main: ${OBJS}
	$(CC) $(INCLUDES) $(CFLAGS) $(OBJS) $(LDFLAGS) -o main

$(OBJS): $(BUILD_DIR)

$(BUILD_DIR):
	mkdir $(BUILD_DIR)

$(BUILD_DIR)/%.o: $(SOURCE_DIR)/%.c
	$(CC) -c $(SOURCE_DIR)/$*.c $(INCLUDES) -o $(BUILD_DIR)/$*.o

clean:
	rm -f $(BUILD_DIR)/*.o
	rm ./main
