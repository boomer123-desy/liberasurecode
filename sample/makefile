CC = gcc
CFLAGS = -Wall -g
LDFLAGS = -L../install/lib -lerasurecode -ldl -I../install/include/liberasurecode
LIBNAME = libec_coding.a
SRC = ec_coding.o

all: $(LIBNAME)

$(LIBNAME): $(SRC)
	ar rcs $@ $^

%.o:%.c
	$(CC) $(CFLAGS) $(LDFLAGS) -c $< -o $@

clean:
	rm -f $(LIBNAME) $(SRC)
