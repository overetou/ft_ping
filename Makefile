#Work title
TARGET=ft_ping

#Directories
OBJ_DIR=obj
SRC_DIR=src
INC_DIRS=includes
LIB_DIRS=

#Compilation related indications
CC=gcc
CFLAGS=-Wall -Wextra -Werror
LIBS=

#From there we use the info to compile everything.
SRC_FILE_NAMES = main.c arguments_processing.c errors.c network_part.c signal_handling.c stats.c str_manip.c time.c
SOURCES=$(addprefix $(SRC_DIR)/, $(SRC_FILE_NAMES))
OBJS:=$(SOURCES:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)
INC_FLAGS=$(addprefix -I,$(INC_DIRS))
LIBIDR_FLAGS=$(addprefix -L,$(LIB_DIRS))
CFLAGS+=$(INC_FLAGS)
LIBS_FLAGS=$(addprefix -l,$(LIBS))
VPATH=$(SRC_DIR)

.phony: $(TARGET) clean re

all: $(TARGET)

$(TARGET): $(OBJS) $(INC_DIRS)/ft_ping.h
	@$(CC) $(CFLAGS) $(OBJS) -o $(TARGET) $(LIBIDR_FLAGS) $(LIBS_FLAGS)
	@echo "Compilation succeeded."

$(OBJS): $(OBJ_DIR)/%.o : $(SRC_DIR)/%.c |$(OBJ_DIR)
	@$(COMPILE.c) $(OUTPUT_OPTIONS) -o $@ $<

$(OBJ_DIR):
	mkdir $(OBJ_DIR)

clean:
	rm -rf $(OBJS) $(TARGET)

re: clean $(TARGET)