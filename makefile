###############################################################################
# Configurazione base progetto
###############################################################################
TARGET := quadr_ino_wing
SRC_DIR := src
BUILD_DIR := build
TEST_DIR := tests

###############################################################################
# Configurazione microcontrollore e toolchain
###############################################################################
MCU := atmega328p
F_CPU := 16000000UL
PROGRAMMER := arduino
PORT := /dev/ttyACM0        # Cambia in base alla tua porta (es. COM3 su Windows)
BAUD := 115200

###############################################################################
# Compilatore AVR e linker
###############################################################################
CXX := avr-g++
OBJCOPY := avr-objcopy
CXXFLAGS := -std=c++17 -Wall -Wextra -Os -DF_CPU=$(F_CPU) -mmcu=$(MCU)
LDFLAGS := -mmcu=$(MCU)

###############################################################################
# Individuazione automatica dei sorgenti e delle cartelle include
###############################################################################
SRCS := $(shell find $(SRC_DIR) -type f -name '*.cpp')
OBJS := $(SRCS:$(SRC_DIR)/%.cpp=$(BUILD_DIR)/%.o)
INCLUDES := $(shell find $(SRC_DIR) -type d)
INC_FLAGS := $(addprefix -I, $(INCLUDES))


TEST_INCLUDES := $(shell find $(TEST_DIR) -type d)
TEST_INC_FLAGS := $(addprefix -I, $(TEST_INCLUDES))


# Trova tutti i file sorgenti
SRC_CPP := $(shell find $(SRC_DIR) -name "*.cpp")
TEST_CPP := $(shell find $(TEST_DIR) -name "*.cpp")

# Escludi il main di produzione e includi i test
TEST_SRC := $(filter-out $(SRC_DIR)/$(TARGET).cpp, $(SRC_CPP)) $(TEST_CPP)

# Oggetti corrispondenti (mantiene la struttura di cartelle)
OBJS := $(patsubst $(SRC_DIR)/%.cpp, $(BUILD_DIR)/%.o, $(SRC_CPP))
TEST_OBJS := $(patsubst $(SRC_DIR)/%.cpp, $(BUILD_DIR)/%.o, $(filter-out $(SRC_DIR)/$(TARGET).cpp, $(SRC_CPP))) \
             $(patsubst $(TEST_DIR)/%.cpp, $(BUILD_DIR)/%.o, $(TEST_CPP))



all: $(BUILD_DIR)/$(TARGET).hex graph

test: $(BUILD_DIR)/test_$(TARGET).hex graph


$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(dir $@)
	@echo " Compiling $<"
	$(CXX) $(CXXFLAGS) $(INC_FLAGS) -c $< -o $@

$(BUILD_DIR)/$(TARGET).elf: $(OBJS)
	@echo " Linking..."
	$(CXX) $(LDFLAGS) $^ -o $@

$(BUILD_DIR)/$(TARGET).hex: $(BUILD_DIR)/$(TARGET).elf
	@echo " Generating HEX..."
	$(OBJCOPY) -O ihex -R .eeprom $< $@

# ============================================================
# COMPILAZIONE TEST
# ============================================================

# Compila i .cpp nella cartella tests (stesso stile)
$(BUILD_DIR)/%.o: $(TEST_DIR)/%.cpp
	@mkdir -p $(dir $@)
	@echo " Compiling test $<"
	$(CXX) $(CXXFLAGS) $(INC_FLAGS) $(TEST_INC_FLAGS) -c $< -o $@

# Linka tutti gli oggetti in un ELF di test
$(BUILD_DIR)/test_$(TARGET).elf: $(TEST_OBJS)
	@echo " Linking test executable..."
	$(CXX) $(LDFLAGS) $^ -o $@

$(BUILD_DIR)/test_$(TARGET).hex: $(BUILD_DIR)/test_$(TARGET).elf
	@echo " Generating HEX..."
	$(OBJCOPY) -O ihex -R .eeprom $< $@


# ============================================================
# TARGETS PRINCIPALI
# ============================================================



###############################################################################
# Flash del microcontrollore
###############################################################################
#upload: $(BUILD_DIR)/$(TARGET).hex
#	@echo " Uploading to Arduino Uno..."
#	avrdude -V -F -c $(PROGRAMMER) -p $(MCU) -P $(PORT) -b $(BAUD) -U flash:w:$<
#
###############################################################################
# Pulizia
###############################################################################
clean:
	@echo " Cleaning build directory..."
	rm -vrf $(BUILD_DIR)
	rm -v ./tools/*.dot

###############################################################################
# Ricostruzione completa
###############################################################################
rebuild: clean all

graph:
	@echo "Drawing Graphs "	
	./tools/makeGraph.sh



