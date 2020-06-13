LIBS=-lGL -lglfw -lGLEW
HEADERS=constants.h  src/libs/lodepng.h src/libs/OBJ_Loader.h  src/models/myCube.h src/models/skybox.h  src/models/myTeapot.h  shaders/shaderprogram.h 
FILES=src/libs/lodepng.cpp  main_file.cpp  shaders/shaderprogram.cpp

main_file: $(FILES) $(HEADERS)
	g++ -o main_file $(FILES)  $(LIBS) -I.
