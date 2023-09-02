OUTPUT_EXT = ""

CC = g++
CFLAGS = -Idependencies/glfw/include

LDFLAGS = -lstdc++ dependencies/glfw/src/libglfw3.a
ifeq ($(shell uname), Darwin)
	LDFLAGS += -framework Cocoa -framework OpenGL -framework IOKit
endif

include ../platform.mak
