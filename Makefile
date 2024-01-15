all:
	g++ -I src/include -L src/lib as.cpp -o as -lmingw32 -lSDL2main -lSDL2 -lSDL2_ttf

