OUTPUT_EXT = ".js"

CC = ./dependencies/emsdk/upstream/emscripten/em++

CFLAGS = -Idependencies/emsdk/upstream/emscripten/cache/sysroot/include
LDFLAGS = -sFETCH -sASYNCIFY -sALLOW_MEMORY_GROWTH

include ../platform.mak

build: html

html:
	@cp base_html.html $(BUILDDIR)/index.html
