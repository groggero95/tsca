CC 			= gcc
CFLAGS 		= -O3 -g -std=gnu99 -W -Wall -Wno-comment
INCLUDES 	= -I./include/
LDFLAGS 	= -L./libraries
BUILD_DIR 	= ./build
SOURCE_DIR 	= ./source
INCLUDE_DIR	= ./include
BIN			= ./main
TIME		= ./timing

OBJS = $(patsubst $(SOURCE_DIR)/%.c, $(BUILD_DIR)/%.o, $(wildcard $(SOURCE_DIR)/*.c))
HEAD = $(wildcard $(INCLUDE_DIR)/*.h)

all: main

test:
	 $(MAKE) -C ./test/ all

cleant:
	 $(MAKE) -C ./test/ clean

timing: dir $(filter-out ./build/main.o, $(OBJS)) ${HEAD}
	$(CC) $(CFLAGS) $(INCLUDES) $(filter-out ./build/main.o, $(OBJS)) $(LDFLAGS) -o $(TIME)	

deb:
	@echo ${OBJS}
	@echo ${HEAD}

main: dir $(filter-out ./build/timing.o, $(OBJS)) ${HEAD}
	$(CC) $(CFLAGS) $(INCLUDES) $(filter-out ./build/timing.o, $(OBJS)) $(LDFLAGS) -o $(BIN)

dir:
	mkdir -p $(BUILD_DIR)

$(BUILD_DIR)/%.o: $(SOURCE_DIR)/%.c ${HEAD}
	$(CC) -c $(SOURCE_DIR)/$*.c $(INCLUDES) -o $(BUILD_DIR)/$*.o

clean:
	@echo "Cleaning up this shit"
	-rm -f $(BUILD_DIR)/*.o
	-rm -f ./main
	-rm -f ./timing

