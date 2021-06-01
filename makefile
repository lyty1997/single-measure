#编写有错误
# VPATH = User User/led User/SPI_PCAP02 User/usart User/systick
# CFLAGS = -I User User/led User/SPI_PCAP02
# TARGET = test
# CC = arm-none-eabi-gcc
# OBJCOPY = arm-none-eabi-objcopy
# RM = rm -rf
# CORE = 4
# CPUFLAGS = -mthumb -mcpu=cortex-m$(CORE)
# LDFLAGS = -O2 -T ../STM32F417IG_FLASH.ld -specs=nosys.specs -static -Wl,-cref,-u,Reset_Handler -Wl,-Map=$(TARGET).map -Wl,--gc-sections -Wl,--defsym=malloc_getpagesize_P=0x80 -Wl,--start-group -lc -lm -Wl,--end-group
# # CFLAGS = -g -o
# OBJCT = startup_stm32f40xx.o system_stm32f4xx.o stm32f4xx_gpio.o stm32f4xx_rcc.o stm32f4xx_spi.o stm32f4xx_usart.o bsp_debug_usart.o bsp_led.o SPI_PCAP02.o main.o
# $(TARGET):$(OBJCT)
# 	$(CC) -o $(TARGET).elf $(OBJCT) $(CPUFLAGS) $(LDFLAGS)
# main.o:main.c
# 	$(CC) -c $(CPUFLAGS) -g -Wa,--warn -o $@ $^
# SPI_PCAP02.o:SPI_PCAP02.c
# 	$(CC) -c $(CPUFLAGS) -g -Wa,--warn -o $@ $^
# bsp_led.o:bsp_led.c
# 	$(CC) -c $(CPUFLAGS) -g -Wa,--warn -o $@ $^
# bsp_debug_usart.o:bsp_debug_usart.c
# 	$(CC) -c $(CPUFLAGS) -g -Wa,--warn -o $@ $^
# stm32f4xx_usart.o:stm32f4xx_usart.c
# 	$(CC) -c $(CPUFLAGS) -g -Wa,--warn -o $@ $^
# stm32f4xx_spi.o:stm32f4xx_spi.c
# 	$(CC) -c $(CPUFLAGS) -g -Wa,--warn -o $@ $^
# stm32f4xx_rcc.o:stm32f4xx_rcc.c
# 	$(CC) -c $(CPUFLAGS) -g -Wa,--warn -o $@ $^
# stm32f4xx_gpio.o:stm32f4xx_gpio.c
# 	$(CC) -c $(CPUFLAGS) -g -Wa,--warn -o $@ $^
# system_stm32f4xx.o:system_stm32f4xx.s
# 	$(CC) -c $(CPUFLAGS) -g -Wa,--warn -o $@ $^
# ./Output/startup_stm32f40xx.o:startup_stm32f40xx.s
# 	$(CC) -c $(CPUFLAGS) -g -Wa,--warn -o $@ $^
# bin:
# 	$(OBJCOPY) $(TARGET).elf $(TARGET).bin
# hex:
# 	$(OBJCOPY) $(TARGET).elf -Oihex $(TARGET).hex
# .PHONY: clean
# clean:
# 	$(RM) *.o $(TARGET).*


# 添加包含路径
vpath %.S stlib
vpath %.c stlib source
vpath %.h stlib stlib/cminc include
DEFS += -DUSE_STDPERIPH_DRIVER
INCS += -Istlib -Istlib/cminc -Iinclude
# 使用其他外设在这里添加
OBJS += main.o
OBJS += stlib/startup_stm32f40xx.o
OBJS += stlib/system_stm32f4xx.o
OBJS += source/bsp_debug_usart.o source/bsp_led.o source/SPI_PCAP02.o
OBJS += source/stm32f4xx_gpio.o source/stm32f4xx_rcc.o source/stm32f4xx_spi.o source/stm32f4xx_usart.o source/SysTick.o source/misc.o
# 使用了编译优化和硬件浮点数
CFLAGS += -mcpu=cortex-m4 -mthumb -Wall
CFLAGS += -mfloat-abi=hard -mfpu=fpv4-sp-d16
CFLAGS += -O3
CFLAGS += -ffunction-sections -fdata-sections
LFLAGS += -mthumb -mcpu=cortex-m4
LFLAGS += -mfloat-abi=hard -mfpu=fpv4-sp-d16
LFLAGS += -Wl,--gc-sections
LFLAGS += -Wl,-cref,-u,Reset_Handler
LFLAGS += -Wl,-Map=test.map
LFLAGS += -Wl,--defsym=malloc_getpagesize_P=0x80
LFLAGS += -Wl,--start-group
LFLAGS += -Wl,--end-group
LFLAGS += -O2
LFLAGS += -specs=nosys.specs
LFLAGS += -static
LFLAGS += -lc
LFLAGS += -lm
# 最后生成的bin文件
all:test.bin test.hex
.PHONY:clean
clean:
	rm -f $(OBJS) $(OBJ_FLASH) $(OBJ_SRAM)
	rm -f test.bin test.elf test.hex
test.bin:test.elf
	arm-none-eabi-objcopy -O binary -S $< $@
test.hex:test.elf
	arm-none-eabi-objcopy $< -Oihex $@
test.elf:$(OBJS) $(OBJ_FLASH)
	arm-none-eabi-gcc $(LFLAGS) $^ -T stlib/STM32F405RGT6_FLASH.ld -o $@
	arm-none-eabi-size $@
burn:test.bin
	st-flash write $< 0x8000000
%.o:%.S
	echo cc: $<
	arm-none-eabi-gcc $(CFLAGS) -c $< -o $@
%.o:%.c
	echo cc: $<
	arm-none-eabi-gcc $(CFLAGS) $(DEFS) $(INCS) -c $< -o $@