LIB      := -L./lib -lglfw3dll -lopengl32 -lglu32 -lz -lpng
INC      := -I./incl

CC        = g++
CFLAGS    = -Wall $(LIB) $(INC) -mwindows

LINKER    = g++
LFLAGS    = -Wall $(LIB) $(INC) -mwindows

TARGET    = project.exe
BINDIR    = out
SRCDIR    = src
OBJDIR    = obj
RESDIR    = res

RES      := $(wildcard $(RESDIR)/*)
RESOUT   := $(foreach item, $(RES), $(BINDIR)/$(item))
SOURCES  := $(wildcard $(SRCDIR)/*.c)
INCLUDES := $(wildcard $(SRCDIR)/*.h)
OBJECTS  := $(SOURCES:$(SRCDIR)/%.c=$(OBJDIR)/%.o)


run: all
	@./$(BINDIR)/$(TARGET)

all: $(BINDIR)/$(TARGET) $(RESOUT) 
	@cp lib/glfw3.dll $(BINDIR)
	@echo "Compiled "$@" successfully."

$(RESOUT): $(RES)
	@cp -t $(BINDIR)/$(RESDIR) $^
	@echo "Copied resources."

clean:
	@rm -rf $(BINDIR)/*
	@echo "Cleaned $(BINDIR)."
	@rm -rf $(OBJDIR)/*
	@echo "Cleaned $(OBJDIR)."
	@mkdir $(BINDIR)/$(RESDIR)

$(BINDIR)/$(TARGET): $(OBJECTS)
	@$(LINKER) $(OBJECTS) -o $@ $(LFLAGS)
	@echo "Compiled "$@" successfully."

$(OBJECTS): $(OBJDIR)/%.o : $(SRCDIR)/%.c
	@$(CC) $(CFLAGS) -c $< -o $@
	@echo "Compiled "$@" successfully."