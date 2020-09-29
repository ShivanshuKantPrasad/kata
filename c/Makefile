CC := clang
LDLIBS += -lcriterion
CFLAGS += -Wall -Wextra -pedantic

TARGET = $(shell ls | grep -Po "\w*(?=\.c)";)

all : $(TARGET)

% : %.c
	$(CC) $(CFLAGS) $< $(LDLIBS) -o Build/$@

clean:
	rm -rf Build
