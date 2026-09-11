LDFLAGS :=  -L lib/
CFLAGS := -Wall -Wextra -std=gnu99

CC := gcc
LDLIBS := -lraylib -lpthread -lm -lX11

WCC := x86_64-w64-mingw32-gcc
WLDLIBS := -lwraylib -lopengl32 -lgdi32 -lwinmm -lpthread -static

lbuild: main.c
	rm -f $@
	gcc $(CFLAGS) $< -o $@ -I include/ $(LDFLAGS) $(LDLIBS)

wbuild: main.c
	rm -f $@
	$(WCC) $(CFLAGS) $< -o $@ -I include/ $(LDFLAGS) $(WLDLIBS)
