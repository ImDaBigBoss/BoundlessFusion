OBJDIR = ./obj
BUILDDIR = ./bin

rwildcard=$(foreach d,$(wildcard $(1:=/*)),$(call rwildcard,$d,$2) $(filter $(subst *,%,$2),$d))

CSRC = $(filter-out ./dependencies/%, $(call rwildcard,.,*.cpp))
OBJS = $(patsubst %.cpp, $(OBJDIR)/%.o, $(CSRC))

OPTIMIZE = -O3
CFLAGS = $(OPTIMIZE) -Wall -Iinclude -Ilibrary/include -Idependencies/emsdk/upstream/emscripten/cache/sysroot/include

LDFLAGS = $(OPTIMIZE)

EMXX = ./dependencies/emsdk/upstream/emscripten/em++

build: deps link html

link: $(OBJS)
	@echo Link $^
	@$(EMXX) $(LDFLAGS) $^ -o $(BUILDDIR)/boundless_fusion.js

	@echo "Compiled!"

$(OBJDIR)/%.o: %.cpp
	@echo "Compile $^ -> $@"
	@mkdir -p $(@D)
	@$(EMXX) $(CFLAGS) -c -o $@ $^

deps:
	@echo "Building dependencies..."
	@cd dependencies/ && make build >> /dev/null

html:
	@echo "Building html..."
	@cp base_html.html $(BUILDDIR)/index.html

clean:
	@rm -rf $(OBJDIR)/*
	@rm -rf $(BUILDDIR)/*
	@cd dependencies/ && make clean >> /dev/null
