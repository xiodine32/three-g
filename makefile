LIB      := -L./lib -lglfw3dll -lopengl32 -lglu32 -lz -lpng
INC      := -I./incl

CC        = g++
CFLAGS    = -Wall $(LIB) $(INC) $(ARCH) $(DEBUG)

LINKER    = g++
LFLAGS    = -Wall $(LIB) $(INC) $(ARCH) $(DEBUG)

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


run: ARCH = -mwindows
run: DEBUG = -O2
run: clean debug

debug: all
	@cpplint --extensions=h,c --filter=-build/include,-readability/casting,-legal/copyright $(SOURCES) $(INCLUDES)
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
	@echo "Created $(BINDIR)/$(RESDIR)."

$(BINDIR)/$(TARGET): $(OBJECTS)
	@$(LINKER) $(OBJECTS) -o $@ $(LFLAGS)
	@echo "Compiled "$@" successfully."

$(OBJECTS): $(OBJDIR)/%.o : $(SRCDIR)/%.c
	@$(CC) $(CFLAGS) -c $< -o $@
	@echo "Compiled "$@" successfully."
