COMPILER=
ARCHIVER=
CP=cp
COMPILER_FLAGS =
LIB=libbigint.a

ifeq ($(notdir $(COMPILER)) , iccarm)
	EXTRA_ARCHIVE_FLAGS=--create
else
ifeq ($(notdir $(COMPILER)) , armcc)
	EXTRA_ARCHIVE_FLAGS=--create
else
	EXTRA_ARCHIVE_FLAGS=rc
endif
endif

RELEASEDIR=../../../lib
INCLUDEDIR=../../../include
INCLUDES=-I./. -I${INCLUDEDIR}

BIGINT_DIR = .
OUTS = *.o
OBJECTS =	$(addsuffix .o, $(basename $(wildcard *.c)))
BIGINT_SRCS := $(wildcard *.c)
BIGINT_OBJS = $(addprefix $(BIGINT_DIR)/, $(BIGINT_SRCS:%.c=%.o))

INCLUDEFILES=$(BIGINT_DIR)/include/bigint.h

libs: libbigint.a

libbigint.a: print_msg_bigint $(BIGINT_OBJS)
	$(ARCHIVER) $(EXTRA_ARCHIVE_FLAGS) ${RELEASEDIR}/${LIB} ${BIGINT_OBJS}

print_msg_bigint:
	@echo "Compiling BigInt Library"

.PHONY: include
include: libbigint_includes

libbigint_includes:
	${CP} ${INCLUDEFILES} ${INCLUDEDIR}

clean:
	rm -rf $(BIGINT_DIR)/${OBJECTS}
	rm -rf ${RELEASEDIR}/${LIB}

$(BIGINT_DIR)/%.o: $(BIGINT_DIR)/%.c $(INCLUDEFILES)
	$(COMPILER) $(COMPILER_FLAGS) $(EXTRA_COMPILER_FLAGS) $(INCLUDES) -c $< -o $@
