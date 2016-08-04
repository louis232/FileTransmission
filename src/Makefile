#顶层路径
#ROOT = $(firstword $(subst mxml, mxml ,$(PWD)))/mxml

PROJECT = FileTransmission
ROOT = $(firstword $(subst $(PROJECT), $(PROJECT) ,$(PWD)))/$(PROJECT)

#包含编译规则
include $(ROOT)/Rules.make

CSOURCES = $(wildcard *.c)

LDFLAGS += -lmxml

targets = $(CSOURCES:.c=)

all:$(targets)

%:%.c
	$(CC) $(CFLAGS) $@.c -o $@ $(LDFLAGS)

clean:
	rm -f $(targets) $(TEMPFILES) *.o

