SRCDIR = ./src
BINDIR = ./bin

LDFLAGS = 

run:
	@go run $(SRCDIR)

build:
	@rm -rf $(BINDIR)
	@mkdir $(BINDIR)

	@echo "Building... version: $(BUILD_NUMBER)"
	@GOOS=linux GOARCH=amd64 go build -ldflags="$(LDFLAGS)" -o $(BINDIR)/boundless_fusion-linux-amd64 $(SRCDIR)
	@GOOS=linux GOARCH=arm64 go build -ldflags="$(LDFLAGS)" -o $(BINDIR)/boundless_fusion-linux-arm64 $(SRCDIR)
	@GOOS=darwin GOARCH=arm64 go build -ldflags="$(LDFLAGS)" -o $(BINDIR)/boundless_fusion-darwin-arm64 $(SRCDIR)
	@GOOS=darwin GOARCH=amd64 go build -ldflags="$(LDFLAGS)" -o $(BINDIR)/boundless_fusion-darwin-amd64 $(SRCDIR)
	@echo "Done."

.PHONY: run
