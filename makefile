LIB      := -L./lib -lglfw3dll -lopengl32 -lglu32 -lz -lpng
INC      := -I./incl

CC        = g++
CFLAGS    = -Wall $(LIB) $(INC) -O2 $(ARCH) $(DEBUG)

LINKER    = g++
LFLAGS    = -Wall $(LIB) $(INC) -O2 $(ARCH) $(DEBUG)

TARGET    = project.exe
DEBUG     = -DDEBUG=1
ARCH      = # -mwindows
BINDIR    = out
SRCDIR    = src
OBJDIR    = obj
RESDIR    = res

RES      := $(wildcard $(RESDIR)/*)
RESOUT   := $(foreach item, $(RES), $(BINDIR)/$(item))
SOURCES  := $(wildcard $(SRCDIR)/*.c)
INCLUDES := $(wildcard $(SRCDIR)/*.h)
OBJECTS  := $(SOURCES:$(SRCDIR)/%.c=$(OBJDIR)/%.o)


debug: all
	@./$(BINDIR)/$(TARGET)

run: ARCH = -mwindows
run: DEBUG = 
run: clean debug

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