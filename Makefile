LIBS=-lGL -lglfw -lGLEW
HEADERS=src/constants.h  src/libs/lodepng.h src/libs/OBJ_Loader.h  res/models/myCube.h res/models/skybox.h  res/models/myTeapot.h  res/shaders/shaderprogram.h 
FILES=src/libs/lodepng.cpp  src/main_file.cpp  res/shaders/shaderprogram.cpp

main_file: $(FILES) $(HEADERS)
	g++ -o main_file $(FILES)  $(LIBS) -I.
