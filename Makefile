all:
	g++ -I src/include -L src/lib SnakeUltra.cpp -o SnakeUltra -lmingw32 -lSDL2main -lSDL2 -lSDL2_ttf -lSDL2_mixer

