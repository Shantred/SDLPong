game:
	g++ *.cpp -o play -I include -L lib -lSDL2-2.0.0 -lSDL2_image -lSDL2_ttf -std=c++11 -stdlib=libc++