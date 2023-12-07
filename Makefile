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
OBJS = \
    $(OBJDIR)\neuron.obj \
    $(OBJDIR)\layer.obj \
    $(OBJDIR)\tests.obj \
    $(OBJDIR)\activation.obj

# Target
$(BINDIR)\tests.exe: $(OBJS)
    $(CC) $(OBJS) $(LINKFLAGS) /out:$@

# Object files
$(OBJDIR)\neuron.obj: $(OBJDIR) $(SRCDIR)\neuron.c $(SRCDIR)\neuron.h
    $(CC) $(CFLAGS) /c $(SRCDIR)\neuron.c /Fo$@

$(OBJDIR)\layer.obj: $(OBJDIR) $(SRCDIR)\layer.c $(SRCDIR)\layer.h
    $(CC) $(CFLAGS) /c $(SRCDIR)\layer.c /Fo$@

$(OBJDIR)\activation.obj: $(OBJDIR) $(SRCDIR)\activation.c $(SRCDIR)\activation.h
    $(CC) $(CFLAGS) /c $(SRCDIR)\activation.c /Fo$@

$(OBJDIR)\tests.obj: $(TESTDIR)\tests.c
    $(CC) $(CFLAGS) /c $(TESTDIR)\tests.c /Fo$@

# Check if the obj directory exists and create it if not
$(OBJDIR):
    if not exist "$(OBJDIR)\nul" mkdir $(OBJDIR)

# Clean
clean:
    del $(OBJDIR)\*.obj
    del $(BINDIR)\*.exe

