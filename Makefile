LDFLAGS :=  -L lib/
CFLAGS := -Wall -Wextra -std=gnu99 -O3 -s

LCC := gcc
LLDLIBS := external/raylib-6.2_linux_amd64/lib/libraylib.a -lpthread -lm -lX11

WCC := x86_64-w64-mingw32-gcc
WLDLIBS := external/raylib-6.2_win64_mingw-w64/lib/libraylib.a -lopengl32 -lgdi32 -lwinmm -lpthread -static

.PHONY: all clean run

all: lbuild wbuild

run: lbuild
	./lbuild

lbuild: ./src/main.c
	$(LCC) $(CFLAGS) -I./external/raylib-6.2_linux_amd64/include/ $< -o $@ -I include/ $(LDFLAGS) $(LLDLIBS)

wbuild: ./src/main.c
	$(WCC) -s $(CFLAGS) -I./external/raylib-6.2_win64_mingw-w64/include/ $< -o $@ -I include/ $(LDFLAGS) $(WLDLIBS)

clean:
	@rm -f wbuild.exe
	@rm -f lbuild
