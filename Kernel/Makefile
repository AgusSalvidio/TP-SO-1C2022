CC = gcc
CFLAGS = -g -Wall -Wextra
LIBS = -lcommons -pthread -lutils

SOURCE_DIR = src
INCLUDE_DIR = include
TARGET_DIR = target

SHARED_DIR = ../Utils
USER_SHARED_LIBS_DIR = $(SHARED_DIR)/$(TARGET_DIR)
USER_SHARED_LIBS_INCLUDE_DIR = $(SHARED_DIR)/$(INCLUDE_DIR)

APPLICATIONS = kernel

OBJECTS = $(patsubst $(SOURCE_DIR)/%.c, $(TARGET_DIR)/%.o, $(wildcard $(SOURCE_DIR)/*.c))
HEADERS = $(wildcard $(INCLUDE_DIR)/*.h)

.PHONY : buildDirectoryCreation all clean

all : buildDirectoryCreation $(APPLICATIONS)

buildDirectoryCreation:
	@echo "About to create $(TARGET_DIR)/ (if it doesn't exist) ..."
	mkdir -p $(TARGET_DIR)

$(TARGET_DIR)/%.o: $(SOURCE_DIR)/%.c $(HEADERS)
	@echo "About to compile $< ..."
	$(CC) $(CFLAGS) -I$(USER_SHARED_LIBS_INCLUDE_DIR) -I$(INCLUDE_DIR) -c $< -o $@
	@echo "Created output of $< named $@"

$(APPLICATIONS): $(OBJECTS)
	@echo "About to link $(OBJECTS) with $(LIBS) and create $(APPLICATIONS) ..."
	$(CC) $(CFLAGS) $(OBJECTS) $(LIBS) -L$(USER_SHARED_LIBS_DIR) -o $(TARGET_DIR)/$@

clean:
	@echo "About to delete objects and applications in $(TARGET_DIR)/ ..."
	rm -f $(TARGET_DIR)/*.o
	rm -f $(TARGET_DIR)/$(APPLICATIONS)