LIB      := -L./lib -lglfw3dll -lopengl32 -lglu32
INC      := -I./incl

CC        = g++
CFLAGS    = -Wall $(LIB) $(INC) -mwindows

LINKER    = g++
LFLAGS    = -Wall $(LIB) $(INC) -mwindows

TARGET    = project.exe
BINDIR    = out
SRCDIR    = src
OBJDIR    = obj

SOURCES  := $(wildcard $(SRCDIR)/*.c)
INCLUDES := $(wildcard $(SRCDIR)/*.h)
OBJECTS  := $(SOURCES:$(SRCDIR)/%.c=$(OBJDIR)/%.o)


run: all
	@./$(BINDIR)/$(TARGET)

all: $(BINDIR)/$(TARGET)
	@cp lib/glfw3.dll $(BINDIR)
	@echo "Compiled "$@" successfully."

clean:
	@rm -rf $(BINDIR)/*
	@echo "Cleaned $(BINDIR)."
	@rm -rf $(OBJDIR)/*
	@echo "Cleaned $(OBJDIR)."

$(BINDIR)/$(TARGET): $(OBJECTS)
	@$(LINKER) $(OBJECTS) -o $@ $(LFLAGS)
	@echo "Compiled "$@" successfully."

$(OBJECTS): $(OBJDIR)/%.o : $(SRCDIR)/%.c
	@$(CC) $(CFLAGS) -c $< -o $@
	@echo "Compiled "$@" successfully."