CC:=gcc
CARGS:=-Wall
BNAME:=53secret
SDIR:=src
INCL:=include
BDIR:=bin

53secret: $(SDIR)/hw1.c $(SDIR)/helpers1.c
	mkdir -p $(BDIR)
	$(CC) $(CARGS) $(SDIR)/hw1.c $(SDIR)/helpers1.c -o $(BDIR)/$(BNAME) -I $(INCL)
.PHONY: clean
clean:
	rm -rf $(BDIR)
