#------------------------------------------------------------------------------
# Make environment configuration.
#------------------------------------------------------------------------------
# Remove make built-in variables and implicit rules. It improves performance
# and debugging.
MAKEFLAGS += -Rr
MAKEFLAGS += --warn-undefined-variables

# Default target.
.DEFAULT_GOAL := all

# Use a single shell instance for better performance and exit as soon as any
# error happens.
#SHELL := /bin/bash
.ONESHELL:
#.SHELLFLAGS += -eu -o pipefail -c

# Delete the target file when the recipe fails.
.DELETE_ON_ERROR:

#------------------------------------------------------------------------------
# Build type and platform.
#------------------------------------------------------------------------------ 
TARGET_NAME := a.out

#------------------------------------------------------------------------------
# Folders & target.
#------------------------------------------------------------------------------ 
LIBS_DIR := $(CURDIR)/libs
BUILD_DIR := $(CURDIR)/build
BIN_DIR := $(BUILD_DIR)/bin
OBJS_DIR := $(BUILD_DIR)/objs
DEPS_DIR := $(BUILD_DIR)/deps

TARGET := $(BIN_DIR)/$(TARGET_NAME)

#------------------------------------------------------------------------------
# Sub-directories and files listing.
#------------------------------------------------------------------------------
# Warning: files *MUST* have unique filename across the whole codebase.

SOURCES := \
	src/e131.cpp \
	src/main.cpp \
	src/cube.cpp \
	src/remote.cpp \
	src/episodes.cpp \

INCLUDE_DIRS := \
	include

LIBS := -lpthread

#------------------------------------------------------------------------------
# Tools selections
#------------------------------------------------------------------------------
CC := g++

#------------------------------------------------------------------------------
# Compiler options.
#------------------------------------------------------------------------------
CFLAGS := -std=c++11 -c -Wall -Wextra -Wpedantic
CPPFLAGS := $(addprefix -I,$(INCLUDE_DIRS))
DEPFLAGS = -MMD -MT $@ -MF $(DEPS_DIR)/$(@F:.o=.d) 

EXTRA_FLAGS := -O0 -g

#------------------------------------------------------------------------------
# Linker options.
#------------------------------------------------------------------------------
LDFLAGS := -L$(LIBS_DIR)/$(TARGET_PLATFORM)
LDLIBS := $(LIBS)

#------------------------------------------------------------------------------
# Create object and dependency files lists.
#------------------------------------------------------------------------------
ALL_FILES := $(notdir $(SOURCES))
ALL_FOLDERS := $(dir $(SOURCES))

OBJECTS := $(ALL_FILES:%.cpp=$(OBJS_DIR)/%.o)
DEPS := $(ALL_FILES:%.cpp=$(DEPS_DIR)/%.d)

# Because filenames *MUST* be unique we can add source files directories to vpath as well as the
# objects directory where all the objects are gathered.
VPATH := $(OBJS_DIR) $(ALL_FOLDERS)

#------------------------------------------------------------------------------
# Rules.
#------------------------------------------------------------------------------
.PHONY: all
all: $(TARGET)

.PHONY: clean
clean:
	rm $(OBJECTS)

$(OBJS_DIR) $(DEPS_DIR) $(BIN_DIR):
	@mkdir -p $@

$(TARGET): $(OBJECTS) | $(BIN_DIR)
	$(CC) $(addprefix $(OBJS_DIR)/,$(notdir $^)) $(LDFLAGS) $(LDLIBS) -o $@

$(OBJS_DIR)/%.o: %.cpp | $(OBJS_DIR) $(DEPS_DIR)
	$(CC) $(CFLAGS) $(EXTRA_FLAGS) $(CPPFLAGS) $(DEPFLAGS) $< -o $@

-include $(DEPS)
