
# Global build targets
.PHONY: default run clean gdb valgrind wdefault wrun wclean all clean-all zip wzip zip-all
DF = -Og -g
RF = -O3 -fopenmp -D_GLIBCXX_PARALLEL
GLOBAL_FLAGS = $(RF)

all: default wdefault


clean-all: clean wclean


zip-all: zip wzip
	
	

# UNIX build targets

DEFFLAGS = -I . -I ./headers/ -I include -w -lpthread -lstdc++fs -Wall -Wextra -Werror -pedantic -std=c++17

FLAGS = $(DEFFLAGS) $(GLOBAL_FLAGS)
LINKS = lib/libsfml-graphics.so.2.5.1 lib/libsfml-window.so.2.5.1 lib/libsfml-system.so.2.5.1 lib/libsfml-audio.so.2.5.1
SRC = .
INCLUDES = -I . -I $(SRC)
BUILD = build
TARGET = tallest-towers


default: $(BUILD)/$(TARGET)

$(BUILD)/$(TARGET): $(SRC)/$(TARGET).cpp
	@mkdir -p $(BUILD)
	@g++ -c $(SRC)/$(TARGET).cpp $(INCLUDES) $(FLAGS)
	@g++ -o $(BUILD)/$(TARGET) *.o $(INCLUDES) $(LINKS) $(FLAGS)
	@rm *.o
	@cp $(LINKS) $(BUILD)

run: $(BUILD)/$(TARGET)
	@./$(BUILD)/$(TARGET)

zip: $(BUILD).zip

$(BUILD).zip: $(BUILD)/$(TARGET)
	zip $(BUILD).zip $(BUILD) -r	
	
clean:
	@rm $(BUILD) -r || true
	@rm $(BUILD).zip || true

gdb: $(BUILD)/$(TARGET)
	@gdb $(BUILD)/$(TARGET)

valgrind: $(BUILD)/$(TARGET)
	@valgrind --leak-check=full -s ./$(BUILD)/$(TARGET)

# Windows-specific targets for cross compilation
WDEFFLAGS = -I . -I ./headers/ -I include -w -lpthread -lstdc++fs -Wall -Wextra -Werror -pedantic -std=c++17 -Wl,--enable-auto-image-base -Wl,--add-stdcall-alias -Wl,--enable-auto-import -mthreads -fmessage-length=0 -fexceptions -fident

WFLAGS = $(WDEFFLAGS) $(GLOBAL_FLAGS)
WLINKS = windows-lib/sfml-graphics-2.dll windows-lib/sfml-window-2.dll windows-lib/sfml-system-2.dll windows-lib/sfml-audio-2.dll
WSRC = .
WINCLUDES = -I . -I $(WSRC)
WBUILD = windows-build
WTARGET = tallest-towers.exe
WSRCFILE = tallest-towers.cpp


wdefault: $(WBUILD)/$(WTARGET)

$(WBUILD)/$(WTARGET): $(WSRC)/$(WSRCFILE)
	@mkdir -p $(WBUILD)
	@x86_64-w64-mingw32-g++ -c $(WSRC)/$(WSRCFILE) $(WINCLUDES) $(WFLAGS)
	@x86_64-w64-mingw32-g++ -o $(WBUILD)/$(WTARGET) *.o $(WINCLUDES) $(WLINKS) $(WFLAGS)
	@rm *.o
	@cp windows-lib/* $(WBUILD)

wrun: $(WBUILD)/$(WTARGET)
	@wine $(WBUILD)/$(WTARGET)

wzip: $(WBUILD).zip
	
	
$(WBUILD).zip: $(WBUILD)/$(WTARGET)
	zip $(WBUILD).zip $(WBUILD) -r

wclean:
	@rm $(WBUILD) -r || true
	@rm $(WBUILD).zip || true
