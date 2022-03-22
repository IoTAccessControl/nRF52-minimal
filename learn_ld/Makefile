ROOT_DIR := $(abspath .)
BUILD_DIR := $(ROOT_DIR)/build
PROJECT := minimal

TOOL := arm-none-eabi-
CC := $(TOOL)gcc
OBJDUMP := $(TOOL)objdump

CFLAGS := -mcpu=cortex-m4 \
	-mfloat-abi=hard -mfpu=fpv4-sp-d16 \
	-mthumb \
	-ffreestanding \
	-g3 \
	-nostdlib \
	-ffunction-sections -fdata-sections \
	-Wall \
	-Werror \
	-Os

LD_FILE := main.ld

LDFLAGS := --specs=nano.specs \
	-Wl,--gc-sections \
	-Wl,--print-memory-usage \
	-Wl,-Map=$(BUILD_DIR)/$(PROJECT).map \
	 -T$(LD_FILE) -Wl,-lc -Wl,-lgcc  

SRC_FILES += $(ROOT_DIR)/minimal.c \
	$(ROOT_DIR)/startup.c


TARGET := $(BUILD_DIR)/$(PROJECT).elf

OBJ_FILES := $(patsubst $(ROOT_DIR)/%.c,$(BUILD_DIR)/%.o,$(SRC_FILES))

all: $(TARGET)

$(TARGET): $(OBJ_FILES)
	@echo "Link: " $(TARGET)
	$(CC) $(LDFLAGS) $(CFLAGS) $^ -o $@
	$(OBJDUMP) -S $(TARGET) > $(BUILD_DIR)/$(PROJECT).asm

$(BUILD_DIR):
	@mkdir -p $(BUILD_DIR)

# if makefile is modified, then rebuild
$(OBJ_FILES): $(SRC_FILES) Makefile

$(BUILD_DIR)/%.o: $(ROOT_DIR)/%.c | $(BUILD_DIR)
	@echo "Compile: " $<
	$(CC) $(CFLAGS) -c -o $@ $< 

.PHONY: clean

clean:
	rm -rf $(BUILD_DIR)