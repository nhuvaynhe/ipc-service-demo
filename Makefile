# Platform selection: default is host
PLATFORM ?= host

# Toolchain configuration
TINA_FOLDER := ~/innova/github-tina/t113-x-tina
CROSS_COMPILE :=  $(TINA_FOLDER)/out/toolchain/gcc-linaro-5.3.1-2016.05-x86_64_arm-linux-gnueabi/bin/arm-linux-gnueabi-

# Host tools
HOST_CC := gcc
HOST_CXX := g++

# Toolchain selection based on platform
ifeq ($(PLATFORM),cross)
    CC := $(CROSS_COMPILE)gcc
    CXX := $(CROSS_COMPILE)g++
else
    CC := $(HOST_CC)
    CXX := $(HOST_CXX)
endif

CFLAGS := -Wall -Wextra -std=c99 -O2 \
		-Isrc \
		-Isrc/aw_wifi \
		-Isrc/ipc/libshm \
		-Isrc/ipc/libsock \
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
