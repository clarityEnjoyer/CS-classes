CC      = gcc
CFLAGS  = -Wall -Wextra -Wno-implicit-fallthrough -std=gnu23 -fPIC -O2
LDFLAGS = -shared -Wl,--wrap=malloc -Wl,--wrap=calloc -Wl,--wrap=realloc -Wl,--wrap=reallocarray -Wl,--wrap=free -Wl,--wrap=strdup -Wl,--wrap=strndup

.PHONY: all clean

# Domyślny cel: buduje bibliotekę
all: librstack.so rstack_example

# 1. Główny cel wymagany w zadaniu: budowa biblioteki
# Kompiluje i dołącza memory_tests zgodnie ze specyfikacją
librstack.so: rstack.o memory_tests.o
	$(CC) $(LDFLAGS) -o $@ $^

# 2. Opcjonalny cel do budowy programu testującego
rstack_example: rstack_example.o librstack.so
	$(CC) -o $@ $< -L. -lrstack -Wl,-rpath=.

# 3. Uniwersalna reguła kompilacji plików obiektowych (.c do .o)
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# 4. Jawne zależności od plików nagłówkowych (kompilacja tylko zmienionych plików)
rstack.o: rstack.c rstack.h
memory_tests.o: memory_tests.c memory_tests.h
rstack_example.o: rstack_example.c rstack.h memory_tests.h

# 5. Sprzątanie (usuwa tylko pliki utworzone przez polecenie make)
clean:
	rm -f *.o *.so rstack_example