CC = g++

OPT = -O3
DEFINES = -pg
FLAGS = -std=c++23 -Wall -Wextra -pedantic $(DEFINES)
LIBS = -lpigpio -lpthread -lrt

TARGET = core-module
SOURCEDIR = source
BUILDDIR = build

SOURCES = $(wildcard $(SOURCEDIR)/*.cpp)
OBJECTS = $(patsubst $(SOURCEDIR)/%.cpp, $(BUILDDIR)/%.o, $(SOURCES))

MAKEFLAGS := --jobs=$(shell nproc)

.PHONY: all clean depend run

all: $(TARGET)

$(BUILDDIR):
	@mkdir -p $(BUILDDIR)

$(TARGET): $(OBJECTS)
	$(CC) $(FLAGS) $(OPT) $^ -o $@ $(LIBS)

$(BUILDDIR)/%.o: $(SOURCEDIR)/%.cpp $(SOURCEDIR)/%.hpp | $(BUILDDIR)
	$(CC) $(FLAGS) $(OPT) -c $< -o $@

depend: .depend

.depend: $(SOURCES)
	rm -rf $(BUILDDIR)/.depend
	$(CC) $(FLAGS) -MM $^ -MF $(BUILDDIR)/.depend

clean:
	rm -rf $(BUILDDIR)
	rm -f $(TARGET)

-include $(BUILDDIR)/.depend
