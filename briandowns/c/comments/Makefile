CC ?= gcc

VERSION := 0.1.0
BINDIR := bin
BINARY := comments
CFLAGS := -lulfius -ljansson -lyder -lorcania -ljson-c

$(BINDIR)/$(BINARY): clean
	if [ ! -d $(BINDIR) ]; then mkdir $(BINDIR); fi
	$(CC) -Dgit_sha="$(git rev-parse HEAD)" main.c api.c log.c store.c $(CFLAGS) -o $(BINDIR)/$(BINARY) 

.PHONY: clean
clean:
	rm -f $(BINDIR)/$(BINARY)

.PHONY: deps
deps:
	./deps.sh 
