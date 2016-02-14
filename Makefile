#compiler name
CC = g++

#compiler flags you want to use (other than profiling)
MYFLAGS = -O3 -lpthread

#libraries to use
LIBS = -lpthread

#flags for profiling (see hacker.doc for more information)
PROFILE = -g -pg
#PROFILE = 

#Directory which holds object files
OBJDIR = obj

#name of executable
EXECUTABLE = chessica

#any global include files
GLOBAL_INCLUDE = chess.h     \
                 constants.h \
                 game.h      \
                 hash.h      \
                 logger.h    \
                 reset.h     \
                 score.h     \
                 timer.h     \
                 tree.h

#########################################################################

CFLAGS = $(MYFLAGS) $(PROFILE)

#OPENINGS = b_k4_kga.o

OBJFILES = $(OPENINGS) \
           $(OBJDIR)/ab.o        \
           $(OBJDIR)/bishop.o    \
           $(OBJDIR)/capture.o   \
           $(OBJDIR)/chess.o     \
           $(OBJDIR)/constants.o \
           $(OBJDIR)/engine.o    \
           $(OBJDIR)/file.o      \
           $(OBJDIR)/game.o      \
           $(OBJDIR)/hash.o      \
           $(OBJDIR)/logger.o    \
           $(OBJDIR)/init.o      \
           $(OBJDIR)/io.o        \
           $(OBJDIR)/king.o      \
           $(OBJDIR)/knight.o    \
           $(OBJDIR)/lock.o      \
           $(OBJDIR)/moves.o     \
           $(OBJDIR)/movetree.o  \
           $(OBJDIR)/pawn.o      \
           $(OBJDIR)/print.o     \
           $(OBJDIR)/queen.o     \
           $(OBJDIR)/reset.o     \
           $(OBJDIR)/rook.o      \
           $(OBJDIR)/safe.o      \
           $(OBJDIR)/score.o     \
           $(OBJDIR)/search.o    \
           $(OBJDIR)/test_ab.o   \
           $(OBJDIR)/test_main.o \
           $(OBJDIR)/test_safe.o \
           $(OBJDIR)/timer.o     \
           $(OBJDIR)/tree.o      \
           $(OBJDIR)/xboard.o 
#OBJs = $(addprefix $(OBJDIR)/$(OBJFILES)

all: $(EXECUTABLE)

$(EXECUTABLE) : $(OBJFILES)

# Dependencies for the main prog

# Main Program

$(OBJDIR)/%.o : %.cpp $(GLOBAL_INCLUDE)
	$(CC) -c $(CFLAGS) $< -o $@

$(EXECUTABLE) : $(GLOBAL_INCLUDE)
	$(CC) $(MYFLAGS) -o $(EXECUTABLE) $(PROFILE) $(OBJFILES) $(LIBS)

clean : 
	rm -f $(EXECUTABLE) $(OBJDIR)/*.o

tarball :
	tar cf openings.tar openings

