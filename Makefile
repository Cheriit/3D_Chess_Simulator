LIBS=-lGL -lglfw -lGLEW
HEADERS=constants.h  src/libs/lodepng.h src/libs/OBJ_Loader.h  res/models/skybox.h src/ShaderProgram.h src/Drawable.h src/Drawer.h src/IndexBuffer.h src/Texture.h src/VertexArray.h src/VertexBuffer.h
FILES=src/libs/lodepng.cpp  src/main_file.cpp  src/ShaderProgram.cpp src/Drawable.cpp src/Drawer.cpp src/IndexBuffer.cpp src/Texture.cpp src/VertexArray.cpp src/VertexBuffer.cpp

main_file: $(FILES) $(HEADERS)
	g++ -o main_file $(FILES)  $(LIBS) -I.
