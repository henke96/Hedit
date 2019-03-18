SOURCES = \
src/main.c \
src/window/window.c \

HEADERS = \
include/window/window.h \
src/window/win/window_impl.h \
src/window/linux/window_impl.h \

WIN_DEBUG = build/debug/hedit.exe
WIN_RELEASE = build/release/hedit.exe

LINUX_DEBUG = build/debug/hedit
LINUX_RELEASE = build/release/hedit

CC = gcc
FLAGS = -Wall -Wextra -Wpedantic -Wwrite-strings -Iinclude -std=c11
RELEASE_FLAGS = -O3 -DNDEBUG -s
DEBUG_FLAGS = -g3 -Wno-unused-parameter

WIN_FLAGS =
WIN_RELEASE_FLAGS = -Wl,-subsystem,windows
WIN_LIBS = -lgdi32

LINUX_FLAGS = -no-pie -fno-pie # Make gcc generate elf.
LINUX_LIBS = -lX11

win_debug: $(WIN_DEBUG)
win_release: $(WIN_RELEASE)
linux_debug: $(LINUX_DEBUG)
linux_release: $(LINUX_RELEASE)

$(WIN_DEBUG): $(HEADERS) $(SOURCES)
	$(CC) $(FLAGS) $(WIN_FLAGS) $(DEBUG_FLAGS) -o $(WIN_DEBUG) $(SOURCES) $(WIN_LIBS)

$(WIN_RELEASE): $(HEADERS) $(SOURCES)
	$(CC) $(FLAGS) $(WIN_RELEASE_FLAGS) $(WIN_FLAGS) $(RELEASE_FLAGS) -o $(WIN_RELEASE) $(SOURCES) $(WIN_LIBS)

$(LINUX_DEBUG): $(HEADERS) $(SOURCES)
	$(CC) $(FLAGS) $(LINUX_FLAGS) $(DEBUG_FLAGS) -o $(LINUX_DEBUG) $(SOURCES) $(LINUX_LIBS)

$(LINUX_RELEASE): $(HEADERS) $(SOURCES)
	$(CC) $(FLAGS) $(LINUX_FLAGS) $(RELEASE_FLAGS) -o $(LINUX_RELEASE) $(SOURCES) $(LINUX_LIBS)

clean:
	rm -f build/debug/*
	rm -f build/release/*
