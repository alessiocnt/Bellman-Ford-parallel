CC = gcc
VCC = nvcc
CFLAGS = -std=c99 -Wall -Wextra -Wpedantic -fopenmp

TARGET_BASE = omp_base.exe
TARGET_FRONTIER = omp_frontier.exe
TARGET_CUDA = cuda.exe

# Directories
OMP_BASE = openmp_base
OMP_FRONTIER = openmp_frontier
CUDA = cuda
SRCDIR = src
LIBDIR = libs
OBJDIR = obj
# Directories OpenMP-Base
SRCS_BASE = $(wildcard $(OMP_BASE)/$(SRCDIR)/*.c)
LIB_SRCS_BASE = $(wildcard $(OMP_BASE)/$(LIBDIR)/*.c)
OBJS_BASE = $(patsubst $(OMP_BASE)/$(SRCDIR)/%.c, $(OMP_BASE)/$(OBJDIR)/%.o, $(SRCS_BASE))
LIB_OBJS_BASE = $(patsubst $(OMP_BASE)/$(LIBDIR)/%.c, $(OMP_BASE)/$(OBJDIR)/%.o, $(LIB_SRCS_BASE))
DEPS_BASE = $(wildcard $(OMP_BASE)/$(LIBDIR)/*.h)
# Directories OpenMP-Frontier
SRCS_FRONTIER = $(wildcard $(OMP_FRONTIER)/$(SRCDIR)/*.c)
LIB_SRCS_FRONTIER = $(wildcard $(OMP_FRONTIER)/$(LIBDIR)/*.c)
OBJS_FRONTIER = $(patsubst $(OMP_FRONTIER)/$(SRCDIR)/%.c, $(OMP_FRONTIER)/$(OBJDIR)/%.o, $(SRCS_FRONTIER))
LIB_OBJS_FRONTIER = $(patsubst $(OMP_FRONTIER)/$(LIBDIR)/%.c, $(OMP_FRONTIER)/$(OBJDIR)/%.o, $(LIB_SRCS_FRONTIER))
DEPS_FRONTIER = $(wildcard $(OMP_FRONTIER)/$(LIBDIR)/*.h)
# Directories CUDA
SRCS_CUDA = $(wildcard $(CUDA)/$(SRCDIR)/no_cyc_loop.cu)

.PHONY: all clean

all: $(TARGET_BASE) $(TARGET_FRONTIER)

# Rules to compile target
$(TARGET_BASE): $(OBJS_BASE) $(LIB_OBJS_BASE)
	$(CC) $(CFLAGS) $^ -o $@

$(TARGET_FRONTIER): $(OBJS_FRONTIER) $(LIB_OBJS_FRONTIER)
	$(CC) $(CFLAGS) $^ -o $@

# Rules to compile source files
$(OMP_BASE)/$(OBJDIR)/%.o: $(OMP_BASE)/$(SRCDIR)/%.c $(DEPS_BASE)
	mkdir -p $(OMP_BASE)/$(OBJDIR)
	$(CC) $(CFLAGS) -I$(OMP_BASE)/$(LIBDIR) -c $< -o $@

$(OMP_FRONTIER)/$(OBJDIR)/%.o: $(OMP_FRONTIER)/$(SRCDIR)/%.c $(DEPS_FRONTIER)
	mkdir -p $(OMP_FRONTIER)/$(OBJDIR)
	$(CC) $(CFLAGS) -I$(OMP_FRONTIER)/$(LIBDIR) -c $< -o $@

# Rules to compile library files
$(OMP_BASE)/$(OBJDIR)/%.o: $(OMP_BASE)/$(LIBDIR)/%.c $(DEPS_BASE)
	mkdir -p $(OMP_BASE)/$(OBJDIR)
	$(CC) $(CFLAGS) -I$(OMP_BASE)/$(LIBDIR) -c $< -o $@

$(OMP_FRONTIER)/$(OBJDIR)/%.o: $(OMP_FRONTIER)/$(LIBDIR)/%.c $(DEPS_FRONTIER)
	mkdir -p $(OMP_FRONTIER)/$(OBJDIR)
	$(CC) $(CFLAGS) -I$(OMP_FRONTIER)/$(LIBDIR) -c $< -o $@

clean:
	rm -rf $(OMP_BASE)/$(OBJDIR) $(TARGET_BASE) $(OMP_FRONTIER)/$(OBJDIR) $(TARGET_FRONTIER) $(TARGET_CUDA)

main:
	$(OMP_BASE)/$(OBJDIR)/main.o $(LIB_SRCS_BASE)
	$(CC) $(CFLAGS) $^ -o $(TARGET_BASE)

	$(OMP_FRONTIER)/$(OBJDIR)/main.o $(LIB_SRCS_FRONTIER)
	$(CC) $(CFLAGS) $^ -o $(TARGET_FRONTIER)

	# $(VCC) $(SRCS_CUDA) -o $(TARGET_CUDA)
