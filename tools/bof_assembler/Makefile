OBJDIR = ./obj
BUILDDIR = ./bin

rwildcard=$(foreach d,$(wildcard $(1:=/*)),$(call rwildcard,$d,$2) $(filter $(subst *,%,$2),$d))

CSRC = $(call rwildcard,.,*.c)
OBJS = $(patsubst %.c, $(OBJDIR)/%.o, $(CSRC))

OPTIMIZE = -O3
CFLAGS = $(OPTIMIZE) -Wall -Iinclude
LDFLAGS = $(OPTIMIZE)

build: $(OBJS)
	@echo Link $^
	@$(CC) $(LDFLAGS) $^ -o $(BUILDDIR)/bof_assembler

$(OBJDIR)/%.o: %.c
	@echo "Compile $^ -> $@"
	@mkdir -p $(@D)
	@$(CC) $(CFLAGS) -c -o $@ $^

clean:
	@rm -rf $(OBJDIR)/*
	@rm -rf $(BUILDDIR)/*
