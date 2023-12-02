# Compiler settings
CC = cl.exe
SINGLETHREADED = /MD
MULTITHREADED = /MT
CFLAGS = /D _CONSOLE /WX /D NDEBUG $(SINGLETHREADED) /O2 /D WIN32 /TC /nologo
LINKFLAGS = /link /DEFAULTLIB:user32.lib /OPT:ICF

# Directories
SRCDIR = .\src
OBJDIR = .\obj
BINDIR = .\bin
TESTDIR = .\test

# Files
TESTS = $(TESTDIR)\tests.c
OBJS = $(OBJDIR)\neuron.obj $(OBJDIR)\tests.obj

# Target
$(BINDIR)\tests.exe: $(OBJS)
    $(CC) $(OBJS) $(LINKFLAGS) /out:$@

# Object files
$(OBJDIR)\neuron.obj: $(OBJDIR) $(SRCDIR)\neuron.c $(SRCDIR)\neuron.h
    $(CC) $(CFLAGS) /c $(SRCDIR)\neuron.c /Fo$@

$(OBJDIR)\tests.obj: $(TESTS)
    $(CC) $(CFLAGS) /c $(TESTS) /Fo$@

# Check if the obj directory exists and create it if not
$(OBJDIR):
    if not exist "$(OBJDIR)\nul" mkdir $(OBJDIR)

# Clean
clean:
    del $(OBJDIR)\*.obj
    del $(BINDIR)\*.exe

