# Top-level Makefile

# Compiler and flags
CC := gcc
CFLAGS := -Wall -Wextra -std=c99 -O2 \
		-Isrc \
		-Isrc/aw_wifi \
		-Isrc/ipc \
		-Isrc/log \


# Output folders
BUILD_DIR := build

# Include component makefiles
include src/service/wifi/wifi_deamon.mk
include src/service/model/mvp_model.mk
include src/service/monitor/monitor.mk

# All target
.PHONY: all
all: $(BUILD_DIR)/wifi_deamon $(BUILD_DIR)/mvp_model $(BUILD_DIR)/monitor

# Clean target
.PHONY: clean
clean:
	rm -rf $(BUILD_DIR)
