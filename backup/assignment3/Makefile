CC = gcc
CFLAGS = -Wall

SRCDIR = lexyacc-code_lab3
SRC = $(SRCDIR)/y.tab.c $(SRCDIR)/lex.yy.c
OBJ = lexyacc-code_lab3/y.tab.o lexyacc-code_lab3/lex.yy.o
BINDIR = bin
PROGRAMS = calc3a.exe calc3b.exe calc3g.exe calc3i.exe

all: $(PROGRAMS)

y.tab.c: $(SRCDIR)/calc3.y
	bison -y -d $(SRCDIR)/calc3.y

lex.yy.c: $(SRCDIR)/calc3.l
	flex $(SRCDIR)/calc3.l

%.o: $(SRCDIR)/%.c
	$(CC) $(CFLAGS) -c $<

calc3a.exe: $(OBJ) $(SRCDIR)/calc3a.c
	$(CC) $(OBJ) $(SRCDIR)/calc3a.c -o $(BINDIR)/$@

calc3b.exe: $(OBJ) $(SRCDIR)/calc3b.c
	$(CC) $(OBJ) $(SRCDIR)/calc3b.c -o $(BINDIR)/$@

calc3g.exe: $(OBJ) $(SRCDIR)/calc3g.c
	$(CC) $(OBJ) $(SRCDIR)/calc3g.c -o $(BINDIR)/$@

calc3i.exe: $(OBJ) $(SRCDIR)/calc3i.c
	$(CC) $(OBJ) $(SRCDIR)/calc3i.c -o $(BINDIR)/$@

clean:
	rm -f $(OBJ) y.tab.c y.tab.h lex.yy.c $(BINDIR)/*.exe


