ROOT_DIR := $(abspath .)
BUILD_DIR  := $(ROOT_DIR)/_build

ifeq ($(V), 1)
Q =
else
Q = @
endif

FREERTOS_ROOT_DIR := $(ROOT_DIR)/freertos_kernel

COMPILER ?= arm-none-eabi-

CC = $(COMPILER)gcc
DUMP = $(COMPILER)objdump
OBJ_COPY = $(COMPILER)objcopy

PROJECT_SRC_DIR = $(ROOT_DIR)/src

SRC_FILES += \
  $(PROJECT_SRC_DIR)/debug_monitor_exception.c \
  $(PROJECT_SRC_DIR)/dummy_functions.c \
  $(PROJECT_SRC_DIR)/fpb.c \
  $(PROJECT_SRC_DIR)/log_port.c \
  $(PROJECT_SRC_DIR)/main.c \
  $(PROJECT_SRC_DIR)/minimal_nrf52_uart.c \
  $(PROJECT_SRC_DIR)/shell.c \
  $(PROJECT_SRC_DIR)/shell_commands.c \
  $(PROJECT_SRC_DIR)/shell_port.c \
  $(PROJECT_SRC_DIR)/startup.c \


OBJ_FILES := $(patsubst $(ROOT_DIR)/%.c,$(BUILD_DIR)/%.o,$(SRC_FILES))

INCLUDE_PATHS += \
  -I$(ROOT_DIR)/include

CFLAGS += \
  -mcpu=cortex-m4 \
  -mfloat-abi=hard -mfpu=fpv4-sp-d16 \
  -mthumb \
  -g3 \
  -nostdlib \
  -ffunction-sections -fdata-sections \
  -Wall \
  -Werror \
  -fdebug-prefix-map=$(ROOT_DIR)=. \
  -Os
  # --specs=nano.specs

TARGET ?= nrf52
LDSCRIPT = $(ROOT_DIR)/$(TARGET).ld
TARGET_ELF = $(BUILD_DIR)/$(TARGET).elf

LDFLAGS += -T$(LDSCRIPT) --specs=nano.specs
LDFLAGS += -Wl,-Map,$(BUILD_DIR)/$(TARGET).map

DEP_DIR = $(BUILD_DIR)/dep
DEP_CFLAGS = -MT $@ -MMD -MP -MF $(DEP_DIR)/$(*F).d

all: $(TARGET_ELF)

flash: $(TARGET_ELF)
	@echo Flashing: $(BUILD_DIR)/$(TARGET).hex
	nrfjprog -f nrf52 --program $(BUILD_DIR)/$(TARGET).hex --sectorerase
	nrfjprog -f nrf52 --reset

debug: $(TARGET_ELF)
	arm-none-eabi-gdb-py --eval-command="target remote localhost:2331"  --ex="mon reset" --ex="load" --ex="mon reset"  --se=$(TARGET_ELF)

-include $(wildcard $(DEP_DIR)/*.d)

clean:
	rm -rf $(BUILD_DIR)

$(TARGET_ELF): $(OBJ_FILES) $(LDSCRIPT)
	@echo "Linking library"
	$(Q) $(CC)  $(CFLAGS) $(LDFLAGS) $(OBJ_FILES) -o $@ -Wl,-lc -Wl,-lgcc
	@echo "Generated $(patsubst $(ROOT_DIR)/%,%,$@)"
	$(Q) $(DUMP) -S $@ > $(BUILD_DIR)/$(TARGET).asm
	@echo "Generated $(BUILD_DIR)/$(TARGET).asm"
	$(Q) $(OBJ_COPY) -O ihex $@ $(BUILD_DIR)/$(TARGET).hex
	@echo "Generated $(BUILD_DIR)/$(TARGET).hex"
	$(Q) $(OBJ_COPY) -O binary $@ $(BUILD_DIR)/$(TARGET).bin
	@echo "Generated $(BUILD_DIR)/$(TARGET).bin"


$(BUILD_DIR):
	@mkdir -p $(BUILD_DIR)

$(DEP_DIR):
	@mkdir -p $(DEP_DIR)

$(OBJ_FILES): $(SRC_FILES) Makefile

$(BUILD_DIR)/%.o: $(ROOT_DIR)/%.c | $(BUILD_DIR) $(DEP_DIR) $(FREERTOS_PORT_ROOT)
	@echo "Compiling $(patsubst $(ROOT_DIR)/%,%,$<)"
	@mkdir -p $(dir $@)
	$(Q) $(CC) $(DEP_CFLAGS) $(CFLAGS) $(INCLUDE_PATHS) -c -o $@ $<
