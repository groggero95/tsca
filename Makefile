CC 			= gcc
CFLAGS 		= -O0 -g -std=gnu99 -W -Wall -Wno-comment
INCLUDES 	= -I./include/
LDFLAGS 	= -L./libraries -lrt -lm
BUILD_DIR 	= ./build
SOURCE_DIR 	= ./source
INCLUDE_DIR	= ./include
BIN			= ./main

OBJS = $(patsubst $(SOURCE_DIR)/%.c, $(BUILD_DIR)/%.o, $(wildcard $(SOURCE_DIR)/*.c))
HEAD = $(wildcard $(INCLUDE_DIR)/*.h)

all: main

deb:
	@echo ${OBJS}
	@echo ${HEAD}

main: ${OBJS} ${HEAD}
	$(CC) $(CFLAGS) $(INCLUDES) $(OBJS) $(LDFLAGS) -o $(BIN)



$(BUILD_DIR)/%.o: $(SOURCE_DIR)/%.c ${HEAD}
	$(CC) -c $(SOURCE_DIR)/$*.c $(INCLUDES) -o $(BUILD_DIR)/$*.o

clean:
	@echo "Cleaning up this shit"
	-rm -f $(BUILD_DIR)/*.o
	-rm -f ./main

