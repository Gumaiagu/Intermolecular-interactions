CC := gcc
CFLAGS := -g3 -Wall -Wextra 
LDLIBS := -lraylib -lm -lX11
LDFLAGS :=  -L lib/

build: main.c
	$(CC) $(CFLAGS) $< -o $@ -I include/ $(LDFLAGS) $(LDLIBS)
