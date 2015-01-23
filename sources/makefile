CFLAGS = -O3 -Wall -pedantic -std=c99    
TARGET =  main
SOURCES = Display.c Information_Window.c tower.c enemy.c actionQueueDataStructure.c parser.c gameProperties.c abilities.c Sound.c levelController.c projectiles.c $(TARGET).c

 
LIBS = -lm -framework SDL2 -framework SDL2_ttf -framework SDL2_image -framework SDL2_mixer
CC = gcc 

all: 
	$(CC) $(SOURCES) -o ./BASHdefender $(CFLAGS) $(LIBS)

	

clean:
	rm -f $(TARGET)

run: all
	$(TARGET) 

	$(TARGET) 
