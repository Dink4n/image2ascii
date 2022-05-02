.POSIX:

# includes and libs
CFLAGS = $(INCS)
LDFLAGS = -lm -lncurses

# paths
DEST_DIR = /usr/local

BIN = image2ascii
SRC = ascii.c
OBJ = ascii.o

all: options $(BIN)

options:
	@echo $(BIN) build options:
	@echo "CFLAGS  = $(CFLAGS)"
	@echo "LDFLAGS = $(LDFLAGS)"
	@echo "CC      = $(CC)"

$(BIN): $(OBJ)
	$(CC) $(LDFLAGS) $< -o $@

$(OBJ): $(SRC)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	$(RM) -rf $(OBJ) $(BIN)

install: $(BIN)
	mkdir -p $(DEST_DIR)/bin
	cp -f $(BIN) $(DEST_DIR)/bin
	chmod 755 $(DEST_DIR)/bin/$(BIN)

uninstall:
	rm -f $(DEST_DIR)/bin/$(BIN)

.PHONY: all options clean install uninstall
