WIFI_SRC := src/service/wifi/wifi_deamon.c \
            src/ipc/ipc_shm.c \
            src/aw_wifi/aw_wifi.c

$(BUILD_DIR)/wifi_deamon: $(WIFI_SRC)
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) $^ -o $@ -pthread
