OBJDIR = ./obj
BUILDDIR = ./bin

rwildcard=$(foreach d,$(wildcard $(1:=/*)),$(call rwildcard,$d,$2) $(filter $(subst *,%,$2),$d))

CSRC = $(filter-out ./dependencies/%, $(call rwildcard,.,*.cpp))
OBJS = $(patsubst %.cpp, $(OBJDIR)/%.o, $(CSRC))

OPTIMIZE = -O3
CFLAGS += $(OPTIMIZE) -Wall -Iinclude -Ilibrary/include -std=c++11

LDFLAGS += $(OPTIMIZE)

build: deps link

link: $(OBJS)
	@echo Link $^
	@$(CC) $(LDFLAGS) $^ -o $(BUILDDIR)/boundless_fusion$(OUTPUT_EXT) $(LDLIBS)

$(OBJDIR)/%.o: %.cpp
	@echo "Compile $^ -> $@"
	@mkdir -p $(@D)
	@$(CC) $(CFLAGS) -c -o $@ $^

deps:
	@echo "Building dependencies..."
	@cd dependencies/ && make build >> /dev/null

clean:
	@rm -rf $(OBJDIR)/*
	@rm -rf $(BUILDDIR)/*
