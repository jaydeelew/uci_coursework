CC:=gcc
BDIR:=bin
SDIR:=src
MAIN:=$(SDIR)/hw0.c 
TEST:=$(SDIR)/test_hw0.c 
INCLUDE:=include

all: hw0.c hw0_test.c
	mkdir -p $(BDIR)
	$(CC) -o $(BDIR)/hw0 $(MAIN) -I $(INCLUDE)
	$(CC) -o $(BDIR)/test_hw0 $(TEST) -I $(INCLUDE) -lcunit
hw0.c:
	mkdir -p $(BDIR)
	$(CC) -o $(BDIR)/hw0 $(MAIN) -I $(INCLUDE)
hw0_test.c:
	mkdir -p $(BDIR)
	$(CC) -o $(BDIR)/test_hw0 $(TEST) -I $(INCLUDE) -lcunit
.phony:clean
clean:
	rm -rf $(BDIR)