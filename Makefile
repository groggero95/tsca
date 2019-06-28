#
# Copyright (C) EURECOM, Telecom Paris
#
# Prof. Renaud PACALET          <renaud.pacalet@telecom-paristech.fr>
# Alberto ANSELMO               <Alberto.Anselmo@eurecom.fr>
# Simone Alessandro CHIABERTO	<Simone-Alessandro.Chiaberto@eurecom.fr>
# Fausto CHIATANTE              <Fausto.Chiatante@eurecom.fr>
# Giulio ROGGERO                <Giulio.Roggero@eurecom.fr>
#
# This file must be used under the terms of the CeCILL. This source
# file is licensed as described in the file COPYING, which you should
# have received as part of this distribution. The terms are also
# available at:
# http://www.cecill.info/licences/Licence_CeCILL_V1.1-US.txt
#

CC 			= gcc
CFLAGS 		= -Ofast -g -std=gnu99 -W -Wall -Wno-comment
CFLAGS_ATT	= -Ofast -std=gnu99 -W -Wall -Wno-comment
INCLUDES 	= -I./include/
LDFLAGS 	= -lm
BUILD_DIR 	= ./build
SOURCE_DIR 	= ./source
INCLUDE_DIR	= ./include
BIN			= ./main
TEST_BIN	= ./test_main
TIME		= ./timing
ATTACK		= ./panda4x4

SOURCE_DOCS		= 	$(wildcard *.md)
EXPORTED_DOCS 	=  	$(SOURCE_DOCS:.md=.pdf)

OBJS = $(patsubst $(SOURCE_DIR)/%.c, $(BUILD_DIR)/%.o, $(wildcard $(SOURCE_DIR)/*.c))
HEAD = $(wildcard $(INCLUDE_DIR)/*.h)

.PHONY: all clean pres time attack top test rep

all: dir top attack time test rep

pres:
	 $(MAKE) -C ./presentation/ all

time: dir $(TIME)

attack: dir $(ATTACK)

top: dir $(BIN)

test: dir $(TEST_BIN)

dir: $(BUILD_DIR)

rep: $(EXPORTED_DOCS)

clean:
	@echo "Cleaning up this shit"
	-rm -f $(BUILD_DIR)/*.o
	-rm -rf $(BUILD_DIR)
	-rm -f ./main
	-rm -f ./timing
	-rm -f ./panda4x4
	-rm -f ./test_main
	-rm -f $(EXPORTED_DOCS)


$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(BUILD_DIR)/%.o: $(SOURCE_DIR)/%.c ${HEAD}
	$(CC) -c $(SOURCE_DIR)/$*.c $(INCLUDES) -o $(BUILD_DIR)/$*.o

$(TIME): $(filter-out ./build/main.o ./build/panda4x4.o, $(OBJS))
	$(CC) $(CFLAGS) $(INCLUDES) $(filter-out ./build/main.o ./build/panda4x4.o, $(OBJS)) $(LDFLAGS) -o $(TIME)

$(ATTACK): $(filter-out ./build/main.o ./build/timing.o, $(OBJS))
	$(CC) $(CFLAGS_ATT) $(INCLUDES) $(filter-out ./build/main.o ./build/timing.o, $(OBJS)) $(LDFLAGS) -o $(ATTACK)

$(BIN): $(filter-out ./build/timing.o ./build/cipher.o ./build/panda4x4.o, $(OBJS))
	$(CC) $(CFLAGS) $(INCLUDES) $(filter-out ./build/timing.o ./build/cipher.o ./build/panda4x4.o, $(OBJS)) $(LDFLAGS) -o $(BIN)

$(TEST_BIN): $(filter-out ./build/timing.o ./build/panda4x4.o ./build/cipher.o ./build/pcc.o ./build/utils.o, $(OBJS))
	$(CC) $(CFLAGS) $(INCLUDES) $(filter-out ./build/timing.o ./build/panda4x4.o ./build/cipher.o ./build/pcc.o ./build/utils.o, $(OBJS)) $(LDFLAGS) -o $(TEST_BIN)

%.pdf: %.md
	pandoc -o $@ $<
