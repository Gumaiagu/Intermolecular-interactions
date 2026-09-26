LDFLAGS :=  -L lib/
CFLAGS := -Wall -Wextra -std=gnu99 -O3

LCC := gcc
LLDLIBS := -lraylib -lpthread -lm -lX11

WCC := x86_64-w64-mingw32-gcc
WLDLIBS := -lwraylib -lopengl32 -lgdi32 -lwinmm -lpthread -static

.PHONY: all clean wbuild lbuild

all: lbuild wbuild

lbuild: ./src/main.c
	$(LCC) $(CFLAGS) $< -o $@ -I include/ $(LDFLAGS) $(LLDLIBS)
	strip $@

wbuild: ./src/main.c
	$(WCC) $(CFLAGS) $< -o $@ -I include/ $(LDFLAGS) $(WLDLIBS)

clean:
	@rm wbuild.exe
	@rm lbuild
