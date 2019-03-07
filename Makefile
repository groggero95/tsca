CC 			= gcc
CFLAGS 		= -O0 -g -std=gnu99 -W -Wall -Wno-comment
INCLUDES 	= -I./include/
LDFLAGS 	= -L./libraries -lrt -lm -lgmp
BUILD_DIR 	= ./build
SOURCE_DIR 	= ./source
BIN			= ./main


OBJS = \
	   $(BUILD_DIR)/mm.o \
	   $(BUILD_DIR)/me.o

all: main

main: ${OBJS}
	$(CC) $(CFLAGS) $(INCLUDES) $(OBJS) $(LDFLAGS) -o $(BIN)

$(BUILD_DIR):
	mkdir $(BUILD_DIR)

$(BUILD_DIR)/%.o: $(SOURCE_DIR)/%.c
	$(CC) -c $(SOURCE_DIR)/$*.c $(INCLUDES) -o $(BUILD_DIR)/$*.o

clean:
	rm ./main
	rm -f $(BUILD_DIR)/*.o
