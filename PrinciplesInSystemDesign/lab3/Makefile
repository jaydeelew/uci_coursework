CC := gcc
SDIR := src
BDIR := bin
SRCS := $(wildcard $(SDIR)/*.c)
PROGS := $(patsubst $(SDIR)/%.c, $(BDIR)/%, $(SRCS))

all: $(PROGS)

$(BDIR)/%: $(SDIR)/%.c
	$(CC) $< -o $@ 

.phony: clean
clean: 
	rm -f $(PROGS)