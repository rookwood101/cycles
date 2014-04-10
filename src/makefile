CC=g++
W32=i586-mingw32msvc-g++
W64=x86_64-w64-mingw32-g++

SOURCES=interface.cpp utility.cpp tasks.cpp database_types.cpp cycles.cpp
CFLAGS=-Wall
LDFLAGSLNX=-lsoci_core -lsoci_sqlite3 -lboost_date_time
LDFLAGSW32=-Xlinker --enable-auto-import -lsoci_core_3_2 -lsoci_sqlite3_3_2 -lboost_date_time-mt

EXECUTABLE=cycles.out
EXECUTABLEW32=cycles32.exe

all: $(SOURCES)
	$(CC) $(SOURCES) $(LDFLAGSLNX) $(CFLAGS) -o $(EXECUTABLE)

win32: $(SOURCES)
	$(W32) $(SOURCES) $(CFLAGS) -o $(EXECUTABLEW32) $(LDFLAGSW32)
