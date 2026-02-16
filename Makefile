CC       = gcc
CFLAGS   = -Wall -Wextra
SRCS     = allocator.c
INCLUDES = allocator.h
OBJS     = allocator.o

CL_FORM       = clang-format
CL_FORM_FLAGS = --sort-includes -Werror -i

TEST_SRC = test.c
TEST_OBJ = test.o
TEST_EXE = ./test

$(OBJS): $(SRCS) $(INCLUDES)

$(TEST_OBJ): $(TEST_SRC) $(INCLUDES)

test: $(OBJS) $(TEST_OBJ)
	$(CC) $(CFLAGS) -o $(TEST_EXE) $(TEST_OBJ) $(OBJS)

format: $(SOURCES) $(INCLUDES)
	$(CL_FORM) $(CL_FORM_FLAGS) $(SOURCES) $(INCLUDES)
	
.PHONY = clean

clean:
	rm -f *.o $(EXE)
