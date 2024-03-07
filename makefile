CC = gcc
CFLAGS = -Wall -Wextra -Wpedantic
TARGET = my_program
SRCDIR = src
LIBDIR = libs
OBJDIR = obj

SRCS = $(wildcard $(SRCDIR)/*.c)
LIB_SRCS = $(wildcard $(LIBDIR)/*.c)
OBJS = $(patsubst $(SRCDIR)/%.c, $(OBJDIR)/%.o, $(SRCS))
LIB_OBJS = $(patsubst $(LIBDIR)/%.c, $(OBJDIR)/%.o, $(LIB_SRCS))
DEPS = $(wildcard $(LIBDIR)/*.h)

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJS) $(LIB_OBJS)
	$(CC) $(CFLAGS) $^ -o $@

# $(OBJDIR)/%.o: $(SRCDIR)/%.c $(DEPS)
# 	@mkdir -p $(OBJDIR)
# 	$(CC) $(CFLAGS) -c $< -o $@

# $(OBJDIR)/%.o: $(LIBDIR)/%.c $(DEPS)
# 	$(CC) $(CFLAGS) -c $< -o $@

# $(OBJDIR)/%.o: $(SRCDIR)/%.c $(DEPS)
# 	@mkdir -p $(OBJDIR)
# 	$(CC) $(CFLAGS) -I$(LIBDIR) -c $< -o $@

# $(OBJDIR)/%.o: $(LIBDIR)/%.c $(DEPS)
# 	@mkdir -p $(OBJDIR)
# 	$(CC) $(CFLAGS) -I$(LIBDIR) -c $< -o $@

$(OBJDIR)/%.o: $(SRCDIR)/%.c $(DEPS)
	if not exist $(OBJDIR) mkdir $(OBJDIR)
	$(CC) $(CFLAGS) -I$(LIBDIR) -c $< -o $@

$(OBJDIR)/%.o: $(LIBDIR)/%.c $(DEPS)
	if not exist $(OBJDIR) mkdir $(OBJDIR)
	$(CC) $(CFLAGS) -I$(LIBDIR) -c $< -o $@

# clean:
# 	rm -rf $(OBJDIR) $(TARGET)
clean:
	rmdir /S /Q $(OBJDIR) 2> nul || cmd /c exit 0
	del /Q $(TARGET) $(TARGET).exe 2> nul || cmd /c exit 0

# Target to compile a specific source file
graph_generator: $(OBJDIR)/graph_generator.o $(LIB_OBJS)
	$(CC) $(CFLAGS) $^ -o $(TARGET)
