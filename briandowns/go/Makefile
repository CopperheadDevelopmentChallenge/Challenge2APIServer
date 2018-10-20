GO ?= go

BINDIR := bin
BINARY := comments

$(BINDIR)/$(BINARY): clean
	if [ ! -d $(BINDIR) ]; then mkdir $(BINDIR); fi
	$(GO) build -o $(BINDIR)/$(BINARY)

.PHONY: clean
clean:
	$(GO) clean
	rm -rf $(BINDIR)/$(BINARY)
