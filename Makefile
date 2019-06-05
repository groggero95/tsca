CC 			= gcc
CFLAGS 		= -Ofast -g -std=gnu99 -W -Wall -Wno-comment
CFLAGS_ATT	= -Ofast -std=gnu99 -W -Wall -Wno-comment
INCLUDES 	= -I./include/
LDFLAGS 	= -L./libraries -lm
BUILD_DIR 	= ./build
SOURCE_DIR 	= ./source
INCLUDE_DIR	= ./include
BIN			= ./main
TIME		= ./timing
ATTACK		= ./panda4x4

OBJS = $(patsubst $(SOURCE_DIR)/%.c, $(BUILD_DIR)/%.o, $(wildcard $(SOURCE_DIR)/*.c))
HEAD = $(wildcard $(INCLUDE_DIR)/*.h)

all: main attack

pres:
	 $(MAKE) -C ./presentation/ all


timing: dir $(filter-out ./build/main.o ./build/panda4x4.o, $(OBJS)) ${HEAD}
	$(CC) $(CFLAGS) $(INCLUDES) $(filter-out ./build/main.o ./build/panda4x4.o, $(OBJS)) $(LDFLAGS) -o $(TIME)

attack: dir $(filter-out ./build/timing.o ./build/main.o, $(OBJS)) ${HEAD}
	$(CC) $(CFLAGS_ATT) $(INCLUDES) $(filter-out ./build/main.o ./build/timing.o, $(OBJS)) $(LDFLAGS) -o $(ATTACK)

deb:
	@echo ${OBJS}
	@echo ${HEAD}

main: dir $(filter-out ./build/timing.o ./build/panda4x4.o, $(OBJS)) ${HEAD}
	$(CC) $(CFLAGS) $(INCLUDES) $(filter-out ./build/timing.o ./build/panda4x4.o, $(OBJS)) $(LDFLAGS) -o $(BIN)

dir:
	mkdir -p $(BUILD_DIR)

$(BUILD_DIR)/%.o: $(SOURCE_DIR)/%.c ${HEAD}
	$(CC) -c $(SOURCE_DIR)/$*.c $(INCLUDES) -o $(BUILD_DIR)/$*.o

clean:
	@echo "Cleaning up this shit"
	-rm -f $(BUILD_DIR)/*.o
	-rm -f ./main
	-rm -f ./timing
	-rm -f ./panda4x4

