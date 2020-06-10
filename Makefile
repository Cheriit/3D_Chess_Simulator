LIBS=-lGL -lglfw -lGLEW
HEADERS=constants.h  lodepng.h  src/models/myCube.h  src/models/myTeapot.h  shaders/shaderprogram.h
FILES=lodepng.cpp  main_file.cpp  shaders/shaderprogram.cpp

main_file: $(FILES) $(HEADERS)
	g++ -o main_file $(FILES)  $(LIBS) -I.
