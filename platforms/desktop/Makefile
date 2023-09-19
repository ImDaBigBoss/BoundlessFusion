OUTPUT_EXT = ""

CC = g++
CFLAGS = -Idependencies/glfw/include

LDLIBS = -lstdc++ dependencies/glfw/src/libglfw3.a
ifeq ($(shell uname), Darwin)
	LDFLAGS += -framework Cocoa -framework OpenGL -framework IOKit
else
	LDLIBS += -lGL
endif

include ../platform.mak
