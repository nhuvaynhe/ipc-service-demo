MONITOR_SRC := src/service/monitor/monitor.c \
               src/ipc/ipc_shm.c

$(BUILD_DIR)/monitor: $(MONITOR_SRC)
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) $^ -o $@ -pthread
