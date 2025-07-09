CC ?= wclang
RM := rm -f
SRCS := Parser/BinToHex.c Executor/ExecMain.c \
		Parser/HeaderLoader.c Parser/SectionLoader.c \
		Parser/Helpers.c
		
RDMC_SRCS := Parser/BinToHex.c Parser/HeaderLoader.c Parser/SectionLoader.c Parser/Helpers.c 

AS_SRCS := $(wildcard Assembler/*.c)

OBJS := $(patsubst %.c,%.o,$(SRCS))
RDMC_OBJS := $(patsubst %.c,%.o,$(RDMC_SRCS))
AS_OBJS := $(patsubst %.c,%.o,$(AS_SRCS))

OUT := macCOFF
RDMC := readMC
AS := asMC

CFLAGS= -O2
LDFLAGS=

main:
	$(MAKE) all
	$(MAKE) all  BUILD=RDMC

ifeq ($(BUILD),RDMC)
CFLAGS += -DRDMC
all: ptclean $(RDMC)
else
all: ptclean  $(OUT) $(AS)
endif

$(OUT): $(OBJS)
	$(CC) $(LDFLAGS) -o $@ $^
	
$(AS): $(AS_OBJS)
	$(CC) $(LDFLAGS) -o $@ $^

$(RDMC): $(RDMC_OBJS)
	$(CC) $(LDFLAGS) -o $@ $^

%.o: %.c 
	$(CC) $(CFLAGS) -o $@ -c $<

clean:
	$(RM) $(OBJS) $(OUT) $(RDMC) $(RDMC_OBJS) $(AS) $(AS_OBJS)
	
ptclean:
	$(RM) Parser/SectionLoader.o