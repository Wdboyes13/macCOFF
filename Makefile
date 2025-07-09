CC = wclang
RM := rm -f
SRCS := Parser/BinToHex.c Executor/ExecMain.c \
		Parser/HeaderLoader.c Parser/SectionLoader.c \
		Parser/Helpers.c 

OBJS := $(patsubst %.c,%.o,$(SRCS))

OUT := macCOFF

CFLAGS=
LDFLAGS=

$(OUT): $(OBJS)
	$(CC) $(LDFLAGS) -o $@ $^

%.o: %.c 
	$(CC) $(CFLAGS) -o $@ -c $<

clean:
	$(RM) $(OBJS) $(OUT)