CFLAGS = -O4 -Wall -pedantic -std=c99 -F /usr/local/lib/ -I /usr/local/include -lSDL2 -lSDL2_ttf -lSDL2_image
#INCDIR = ../includes/
#INCS = $(INCDIR)parser.h
#		$(INCDIR)gameProperties.h	
#		$(INCDIR)actionQueueDataStructure.h
#		$(INCDIR)debug.h\
#		$(INCDIR)tower.h\
#		$(INCDIR)terminalwindow.h\
#		$(INCDIR)sput.h              
TARGET =  main
SOURCES = levelController.c Display.c Sound.c Information_Window.c tower.c enemy.c actionQueueDataStructure.c parser.c gameProperties.c $(TARGET).c
#	  		sTower2.c\
#	  		tower2.c\
#      		enemy2.c\
#          	terminalwindow.c\
 
#LIBS = -framework SDL2 -lm -framework SDL2_ttf -framework SDL2_image
LIBS = -lm
CC =gcc 

all: $(TARGET)

$(TARGET) $(SOURCES): $(INCS)
	$(CC) $(SOURCES) -o gameOut $(CFLAGS) $(LIBS)

clean:
	rm -f $(TARGET)

run: all
	$(TARGET) 
