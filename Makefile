CC=gcc
CFLAGS=-Wall -g -Iinclude

SRCDIR=src
TESTDIR=$(SRCDIR)/tests
INCDIR=include
OBJDIR=obj
BINDIR=bin
TESTOBJDIR=$(OBJDIR)/tests

ifeq ($(OS),Windows_NT)
	EXE_EXT := .exe
else
	EXE_EXT :=
endif

TARGET := $(BINDIR)/main$(EXE_EXT)
HEAP_TEST_TARGET := $(BINDIR)/heap_test$(EXE_EXT)

all: prepare build

prepare:
ifeq ($(OS),Windows_NT)
	@if not exist $(BINDIR) mkdir $(BINDIR)
	@if not exist $(OBJDIR) mkdir $(OBJDIR)
	@if not exist $(TESTOBJDIR) mkdir $(TESTOBJDIR)
else
	@mkdir -p $(BINDIR) $(OBJDIR) $(TESTOBJDIR)
endif

build: prepare
ifeq ($(OS),Windows_NT)
	$(MAKE) build_windows
else
	$(MAKE) build_unix
endif

build_windows: $(TARGET)
build_unix: $(TARGET)

# Test targets - add additional test binaries here as needed
TEST_TARGETS = $(HEAP_TEST_TARGET)

test: prepare test_all

test_all: $(TEST_TARGETS)
	@echo Running all unit tests...
	$(HEAP_TEST_TARGET)

$(TARGET): $(OBJDIR)/main.o $(OBJDIR)/heap.o
	$(CC) $(CFLAGS) -o $@ $^

$(HEAP_TEST_TARGET): $(TESTOBJDIR)/heap_test.o $(OBJDIR)/heap.o
	$(CC) $(CFLAGS) -o $@ $^

$(OBJDIR)/main.o: $(SRCDIR)/main.c $(INCDIR)/ds/heap.h
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJDIR)/heap.o: $(SRCDIR)/heap.c $(INCDIR)/ds/heap.h
	$(CC) $(CFLAGS) -c $< -o $@

$(TESTOBJDIR)/heap_test.o: $(TESTDIR)/heap_test.c $(INCDIR)/ds/heap.h
	$(CC) $(CFLAGS) -c $< -o $@

# Clean rules
clean:
ifeq ($(OS),Windows_NT)
	if exist $(BINDIR)\main$(EXE_EXT) del $(BINDIR)\main$(EXE_EXT)
	if exist $(BINDIR)\heap_test$(EXE_EXT) del $(BINDIR)\heap_test$(EXE_EXT)
	if exist $(OBJDIR)\*.o del $(OBJDIR)\*.o
	if exist $(TESTOBJDIR)\*.o del $(TESTOBJDIR)\*.o
else
	rm -f $(BINDIR)/main$(EXE_EXT)
	rm -f $(BINDIR)/heap_test$(EXE_EXT)
	rm -f $(OBJDIR)/*.o
	rm -f $(TESTOBJDIR)/*.o
endif

run:
	$(BINDIR)/main$(EXE_EXT)

run_test:
	$(HEAP_TEST_TARGET)

.PHONY: all prepare build build_windows build_unix test test_all clean run run_test
