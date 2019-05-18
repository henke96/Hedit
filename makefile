SOURCES = \
src/window/window.c \
src/fenwickTree/fenwickTreeI32.c \

HEADERS = \
include/window/window.h \
src/window/win/window_impl.h \
src/window/linux/window_impl.h \
include/fenwickTree/fenwickTreeI32.h \
include/fenwickTree/_fenwickTree.h \

TEST_HEADERS = \
test/fenwickTree/test_fenwickTreeI32.h \

WIN_DEBUG = build/debug/hedit.exe
WIN_RELEASE = build/release/hedit.exe
WIN_TEST = build/test/hedit.exe

LINUX_DEBUG = build/debug/hedit
LINUX_RELEASE = build/release/hedit
LINUX_TEST = build/test/hedit

CC = gcc
FLAGS = -Wall -Wextra -Wpedantic -Wwrite-strings -Iinclude -std=c11
RELEASE_FLAGS = -O3 -DNDEBUG -s
DEBUG_FLAGS = -g3 -Wno-unused-parameter
TEST_FLAGS = 

WIN_FLAGS =
WIN_RELEASE_FLAGS = -Wl,-subsystem,windows
WIN_LIBS = -lgdi32

LINUX_FLAGS = -no-pie -fno-pie # Make gcc generate elf.
LINUX_LIBS = -lX11

win_debug: $(WIN_DEBUG)
win_release: $(WIN_RELEASE)
win_test: $(WIN_TEST)

linux_debug: $(LINUX_DEBUG)
linux_release: $(LINUX_RELEASE)
linux_test: $(LINUX_TEST)

$(WIN_DEBUG): $(HEADERS) src/main.c $(SOURCES)
	$(CC) $(FLAGS) $(WIN_FLAGS) $(DEBUG_FLAGS) -o $(WIN_DEBUG) src/main.c $(SOURCES) $(WIN_LIBS)

$(WIN_RELEASE): $(HEADERS) src/main.c $(SOURCES)
	$(CC) $(FLAGS) $(WIN_RELEASE_FLAGS) $(WIN_FLAGS) $(RELEASE_FLAGS) -o $(WIN_RELEASE) src/main.c $(SOURCES) $(WIN_LIBS)

$(WIN_TEST): $(HEADERS) test/test.c $(SOURCES) $(TEST_HEADERS)
	$(CC) $(FLAGS) $(WIN_FLAGS) $(TEST_FLAGS) -o $(WIN_TEST) test/test.c $(SOURCES) $(WIN_LIBS)

$(LINUX_DEBUG): $(HEADERS) src/main.c $(SOURCES)
	$(CC) $(FLAGS) $(LINUX_FLAGS) $(DEBUG_FLAGS) -o $(LINUX_DEBUG) src/main.c $(SOURCES) $(LINUX_LIBS)

$(LINUX_RELEASE): $(HEADERS) src/main.c $(SOURCES)
	$(CC) $(FLAGS) $(LINUX_FLAGS) $(RELEASE_FLAGS) -o $(LINUX_RELEASE) src/main.c $(SOURCES) $(LINUX_LIBS)

$(LINUX_TEST): $(HEADERS) test/test.c $(SOURCES) $(TEST_HEADERS)
	$(CC) $(FLAGS) $(LINUX_FLAGS) $(TEST_FLAGS) -o $(LINUX_TEST) test/test.c $(SOURCES) $(LINUX_LIBS)

clean:
	rm -f build/debug/*
	rm -f build/release/*
	rm -f build/test/*
