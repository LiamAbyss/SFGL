# Compiler
CXX 			:= g++ -std=c++17
CXXFLAGS		:= -Wall -g -O2
LDFLAGS			:= 

# Build options
BUILDDIR		= build
BUILDNAME		= game
SRCDIR			= .

# SFML
SFML_LDFLAGS 	= -lsfml-graphics
SFML_LDFLAGS += -lsfml-window
SFML_LDFLAGS += -lsfml-system
SFML_LDFLAGS += -lsfml-audio
SFML_LDFLAGS += -lsfml-network

# SFGUI
LIB_SFGUI_PATH	= /usr/local/lib/libSFGUI.so

# Third-party libraries
LIBS			= $(SRCDIR)/libs/json.hpp

# Sources
SRCS 			= $(SRCDIR)/main.cpp
SRCS += $(SRCDIR)/Camera.h $(SRCDIR)/Camera.cpp
SRCS += $(SRCDIR)/Game.h $(SRCDIR)/Game.cpp
SRCS += $(SRCDIR)/ResourceManager.h $(SRCDIR)/ResourceManager.cpp
SRCS += $(SRCDIR)/Scene.h $(SRCDIR)/Scene.cpp

OBJS 			= $(patsubst %.cpp,%.o,$(filter %.cpp, $(SRCS)))

# Recipes
.DEFAULT: all
.PHONY: all
all: $(SRCS) $(BUILDDIR) $(OBJS)
	$(CXX) $(CXXFLAGS) $(LIBS) $(OBJS) $(LIB_SFGUI_PATH) $(LDFLAGS) $(SFML_LDFLAGS) -o $(BUILDDIR)/$(BUILDNAME)

.PHONY: clean
clean:
	rm -rf $(BUILDDIR)
	rm -rf $(SRCDIR)/*.gch $(SRCDIR)/**/*.gch
	rm -rf $(SRCDIR)/*.o $(SRCDIR)/**/*.o

.PHONY: format
format:
	clang-format $(SRCS) -i --verbose

$(BUILDDIR):
	mkdir -p $(BUILDDIR)