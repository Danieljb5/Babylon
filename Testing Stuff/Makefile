GPPPARAMS = -I . -I ./headers/ -I include -w -lpthread -lstdc++fs -Wall -Wextra -Werror -pedantic -std=c++17
LDPARAMS = -I . -I ./headers/ -I include lib/libsfml-graphics.so.2.5.1 lib/libsfml-window.so.2.5.1 lib/libsfml-system.so.2.5.1 lib/libsfml-audio.so.2.5.1 -lpthread -lstdc++fs -Wall -Wextra -Werror -pedantic -std=c++17
SOURCES = main.cpp
OBJECTS = objects/main.o
OBJECTSDIR = objects
OUTPUT = build/main
OUTPUTDIR = build
DEBUG = -Og -g
RELEASE = -Os -fopenmp -D_GLIBCXX_PARALLEL
SELECTED = $(DEBUG)

$(OBJECTSDIR):
	@mkdir -p $(OBJECTSDIR)

$(OBJECTS): $(SOURCES) $(OBJECTSDIR)
	@g++ -c $(SOURCES) $(GPPPARAMS) $(SELECTED)
	@mv *.o $(OBJECTSDIR)

$(OUTPUTDIR):
	@mkdir -p $(OUTPUTDIR)

$(OUTPUT): $(OBJECTS) $(OUTPUTDIR)
	@g++ -o $(OUTPUT) $(OBJECTS) $(LDPARAMS) $(SELECTED)
	@cp lib $(OUTPUTDIR) -r

.PHONY: run
run: $(OUTPUT)
	@./$(OUTPUT)

.PHONY: clean
clean:
	@rm $(OUTPUTDIR) -r || true
	@rm $(OBJECTSDIR) -r || true

# Windows-specific targets for cross compilation

.PHONY: windows-build
windows-build:
	@x86_64-w64-mingw32-g++ -c $(SOURCES) -I . -I ./headers/ -I include -w -lpthread -lstdc++fs -Wall -Wextra -Werror -pedantic -std=c++17 $(SELECTED) -Wl,--enable-auto-image-base -Wl,--add-stdcall-alias -Wl,--enable-auto-import -mthreads -fmessage-length=0 -fexceptions -fident
	@mkdir -p windows-objects
	@mv main.o windows-objects
	@mkdir -p windows-build
	@cp windows-lib/* windows-build
	@x86_64-w64-mingw32-g++ -o windows-build/main.exe windows-objects/main.o -I . -I ./headers/ -I include -lpthread -lstdc++fs -Wall -Wextra -Werror -pedantic -std=c++17 $(SELECTED) windows-build/libstdc++-6.dll -Wl,--enable-auto-image-base -Wl,--add-stdcall-alias -Wl,--enable-auto-import -mthreads -fmessage-length=0 -fexceptions -fident windows-build/sfml-graphics-2.dll windows-build/sfml-window-2.dll windows-build/sfml-system-2.dll windows-build/sfml-audio-2.dll

.PHONY: windows-run
windows-run: windows-build
	@wine windows-build/main.exe

.PHONY: windows-clean
windows-clean:
	@rm windows-objects -r || true
	@rm windows-build -r || true