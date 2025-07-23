#------------------------------------------------------
# Autor: Thiago Werlley
# Modificado por: Gustavo Salgado
#------------------------------------------------------

# Toolchain
CHAIN = arm-none-eabi
CFLAGS = -std=c99 -Wall

# Pastas
SRC = src/
INC = inc/
OBJ = obj/
BIN = bin/
CFILES = cfiles/
IPATH = -I$(INC)

# Nome do executável
NOME_EXEC = appGpio

# Endereços de rede
IP_BEAGLEBONE = 10.8.3.2
IP_TFTP = 10.8.3.1

# Arquivos fonte
LIB_CS = $(wildcard $(CFILES)*.c)
MAIN_CS = $(wildcard $(SRC)*.c)

# Arquivos objeto
LIB_OBJS = $(patsubst $(CFILES)%.c, $(OBJ)%.o, $(LIB_CS))
MAIN_OBJS = $(patsubst $(SRC)%.c, $(OBJ)%.o, $(MAIN_CS))
START_OBJ = $(OBJ)start.o

# Todos os objetos
OBJS = $(START_OBJ) $(MAIN_OBJS) $(LIB_OBJS)

# Alvo principal
all: app

# Compilação final
app: $(BIN) $(OBJ) $(OBJS)
	$(CHAIN)-ld $(OBJS) -T $(SRC)memmap.ld -o $(OBJ)main.elf
	$(CHAIN)-objcopy $(OBJ)main.elf $(BIN)$(NOME_EXEC).bin -O binary
	cp $(BIN)$(NOME_EXEC).bin /srv/tftp/$(NOME_EXEC).bin
	@echo 'setenv app "setenv autoload no; setenv ipaddr $(IP_BEAGLEBONE); setenv serverip $(IP_TFTP); tftp 0x80000000 $(NOME_EXEC).bin; echo ***Booting to BareMetal ***; go 0x80000000"'


# Compilação do start.s
$(START_OBJ): $(SRC)start.s | $(OBJ)
	$(CHAIN)-as $(IPATH) $(SRC)start.s -o $(START_OBJ)

# Compilação dos .c em src/
$(OBJ)%.o: $(SRC)%.c | $(OBJ)
	$(CHAIN)-gcc $(CFLAGS) $(IPATH) -c $< -o $@

# Compilação dos .c em cfiles/
$(OBJ)%.o: $(CFILES)%.c | $(OBJ)
	$(CHAIN)-gcc $(CFLAGS) $(IPATH) -c $< -o $@

# Diretórios bin/ e obj/
$(OBJ):
	mkdir -p $(OBJ)

$(BIN):
	mkdir -p $(BIN)

# Alvo de limpeza
clean:
	rm -rf $(OBJ)*.o
	rm -rf $(OBJ)*.elf
	rm -rf $(BIN)*.bin

# Dump para debug
dump:
	$(CHAIN)-objdump -D $(OBJ)main.elf