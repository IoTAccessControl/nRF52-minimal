# for windows

ROOT_DIR := $(abspath .)
BUILD_DIR  := $(ROOT_DIR)/_build

COMPILER ?= arm-none-eabi-gcc

PROJECT_SRC_DIR = $(ROOT_DIR)/src

SRC_FILES += \
	$(PROJECT_SRC_DIR)/main.c \
	$(PROJECT_SRC_DIR)/startup.c \
	$(PROJECT_SRC_DIR)/debug_monitor.c \
	$(PROJECT_SRC_DIR)/minimal_nrf52_uart.c \
	$(PROJECT_SRC_DIR)/shell_port.c \

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

TARGET ?= nrf52
LDSCRIPT = $(ROOT_DIR)/$(TARGET).ld
TARGET_ELF = $(BUILD_DIR)/$(TARGET).elf
TARGET_HEX = $(BUILD_DIR)/$(TARGET).hex

LDFLAGS += -T$(LDSCRIPT)
LDFLAGS += -Wl,-Map,$(BUILD_DIR)/$(TARGET).map

DEP_DIR = $(BUILD_DIR)/dep
DEP_CFLAGS = -MT $@ -MMD -MP -MF $(DEP_DIR)/$(*F).d

-include $(wildcard $(DEP_DIR)/*.d)

all: $(TARGET_ELF)

clean:
	@rmdir /q /s _build

$(TARGET_ELF): $(OBJ_FILES) $(LDSCRIPT)
	@echo "Linking library"
	@arm-none-eabi-gcc $(CFLAGS) $(LDFLAGS) $(OBJ_FILES) -o $@ -Wl,-lc -Wl,-lgcc
	@echo "Generated $(patsubst $(ROOT_DIR)/%,%,$@)"

$(OBJ_FILES): $(SRC_FILES) Makefile

$(BUILD_DIR):
	@echo $(BUILD_DIR)
	@mkdir _build
	@mkdir _build\src

$(DEP_DIR):
	@echo "mkdir" $(DEP_DIR)
	@mkdir _build\dep

$(BUILD_DIR)/%.o: $(ROOT_DIR)/%.c | $(BUILD_DIR) $(DEP_DIR) $(FREERTOS_PORT_ROOT)
	@echo Compiling $(patsubst $(ROOT_DIR)/%,%,$<)
	@arm-none-eabi-gcc $(DEP_CFLAGS) $(CFLAGS) $(INCLUDE_PATHS) -c -o $@ $<

flash:
	@echo Flashing: $(TARGET_HEX)
	@arm-none-eabi-objcopy -O ihex $(TARGET_ELF) $(TARGET_HEX)
	nrfjprog -f nrf52 --program $(TARGET_HEX) --sectorerase
	nrfjprog -f nrf52 --reset

# flash:
# 	@echo Flashing: $(TARGET_ELF)
# 	nrfjprog -f nrf52 --program $(TARGET_ELF) --sectorerase
# 	nrfjprog -f nrf52 --reset

debug: $(TARGET_ELF)
	arm-none-eabi-gdb-py --eval-command="target remote localhost:2331"  --ex="mon reset" --ex="load" --ex="mon reset"  --se=$(TARGET_ELF)
