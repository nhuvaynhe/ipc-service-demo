MODEL_SRC := src/service/model/mvp_model.c \
             src/ipc/ipc_shm.c

$(BUILD_DIR)/mvp_model: $(MODEL_SRC)
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) $^ -o $@ -pthread
