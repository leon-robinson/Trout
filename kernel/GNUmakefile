override KERNEL := bin/kernel.elf

override CC := x86_64-elf-gcc

CFLAGS ?= -O2 -g -Wall -Wextra -Wpedantic -pipe

LDFLAGS ?= 

override INTERNALCFLAGS := \
    -I.                    \
	-Iinclude/             \
    -std=c11               \
    -ffreestanding         \
    -fno-stack-protector   \
    -fno-stack-check       \
    -fno-pie               \
    -fno-pic               \
    -mabi=sysv             \
    -mno-80387             \
    -mno-mmx               \
    -mno-3dnow             \
    -mno-sse               \
    -mno-sse2              \
    -mno-red-zone          \
    -mcmodel=kernel        \
    -MMD

override INTERNALLDFLAGS :=     \
    -nostdlib                   \
    -static                     \
    -Wl,-z,max-page-size=0x1000 \
    -Wl,-T,linker.ld

override CFILES := $(shell find ./src -type f -name '*.c')
override OBJ := $(patsubst ./src/%.c,bin/%.c.o,$(CFILES))
override HEADER_DEPS := $(CFILES:.c=.d)

.PHONY: all
all: $(KERNEL)

$(KERNEL): $(OBJ)
	$(CC) $(OBJ) $(LDFLAGS) $(INTERNALLDFLAGS) -o $@

-include $(HEADER_DEPS)
bin/%.c.o: src/%.c
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) $(INTERNALCFLAGS) -c $< -o $@

.PHONY: clean
clean:
	@rm -rf bin