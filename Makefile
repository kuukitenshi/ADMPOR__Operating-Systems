#-----------------------------------------------------#
# Projeto - Fase 2 - Sistemas Operativos			  #
#													  #
# Grupo 47:											  #
# Rodrigo Correia   58180							  #
# Laura Cunha       58188							  #
# Guilherme Wind    58640							  #
#-----------------------------------------------------#


# Compiler variables
CC = gcc
CFLAGS = -Wall -g

# Directory variables
SRC_DIR = src
OBJ_DIR = obj
BIN_DIR = bin
INCLUDE_DIR = include

EXEC_NAME = admpor

# Objects variables
OBJS = client.o enterprise.o intermediary.o main.o memory.o process.o utils.o synchronization.o stats.o log.o configuration.o aptime.o apsignal.o

client.o = client.h
enterprise.o = enterprise.h
intermediary.o = intermediary.h
main.o = main.h
memory.o = memory.h
process.o = process.h
utils.o = utils-private.h
synchronization.o = synchronization.h
stats.o = stats.h
log.o = log.h
configuration.o = configuration.h
aptime.o = aptime.h
apsignal.o = apsignal.h

# Object files path
vpath %.o $(OBJ_DIR)
vpath %.c $(SRC_DIR)

# Make targets
$(BIN_DIR)/$(EXEC_NAME): $(OBJS)
	$(CC) $(CFLAGS) $(addprefix $(OBJ_DIR)/,$(OBJS)) -o $@

%.o: %.c $($@)
	$(CC) -I $(INCLUDE_DIR) $(CFLAGS) -o $(OBJ_DIR)/$@ -c $<

# Create folders (PHONY)
folders:
	mkdir -p $(OBJ_DIR)
	mkdir -p $(BIN_DIR)

# Clean object files and executable (PHONY)
clean:
	rm -f $(OBJ_DIR)/*.o
	rm -f $(BIN_DIR)/$(EXEC_NAME)
