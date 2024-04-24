CC = nvcc
CFLAGS = -std=c++11
TARGET = cuda_bellman_ford.exe
SRCDIR = src
LIBDIR = libs
OBJDIR = obj

SRCS = $(wildcard $(SRCDIR)/*.cu)
LIB_SRCS = $(wildcard $(LIBDIR)/*.cu)
OBJS = $(patsubst $(SRCDIR)/%.cu, $(OBJDIR)/%.o, $(SRCS))
LIB_OBJS = $(patsubst $(LIBDIR)/%.cu, $(OBJDIR)/%.o, $(LIB_SRCS))
DEPS = $(wildcard $(LIBDIR)/*.h)

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJS) $(LIB_OBJS)
	$(CC) $(CFLAGS) $^ -o $@

$(OBJDIR)/%.o: $(SRCDIR)/%.cu $(DEPS)
    mkdir -p $(OBJDIR)
    $(CC) $(CFLAGS) -I$(LIBDIR) -c $< -o $@

$(OBJDIR)/%.o: $(LIBDIR)/%.cu $(DEPS)
    mkdir -p $(OBJDIR)
    $(CC) $(CFLAGS) -I$(LIBDIR) -c $< -o $@

clean:
	rm -rf $(OBJDIR) $(TARGET)
# clean:
# 	rmdir /S /Q $(OBJDIR) 2> nul || cmd /c exit 0
# 	del /Q $(TARGET) $(TARGET).exe 2> nul || cmd /c exit 0

# Target to compile a specific source file
# graph_generator: $(OBJDIR)/graph_generator.o $(LIB_OBJS)
# 	$(CC) $(CFLAGS) $^ -o $(TARGET)

main: $(OBJDIR)/main.o $(LIB_OBJS)
	$(CC) $(CFLAGS) $^ -o $(TARGET)
