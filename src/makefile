CC=g++
W32=i586-mingw32msvc-g++
W64=x86_64-w64-mingw32-g++

SOURCES=interface.cpp utility.cpp tasks.cpp database_types.cpp cycles.cpp
CFLAGS=
LDFLAGS=-Xlinker --enable-auto-import -lsoci_core_3_2 -lsoci_sqlite3_3_2
#LDFLAGSW32=-L/usr/i586-mingw32msvc/lib

EXECUTABLE=cycles.out
EXECUTABLEW32=cycles32.exe

all: $(SOURCES)
	$(CC) $(SOURCES) $(LDFLAGS) $(CFLAGS) -o $(EXECUTABLE)

win32: $(SOURCES)
	$(W32) $(SOURCES) $(CFLAGS) -o $(EXECUTABLEW32) $(LDFLAGSW32) $(LDFLAGS)
