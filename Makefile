LIBS=-lGL -lglfw -lGLEW
HEADERS=constants.h  src/vendor/stb_image/stb_image.h src/vendor/OBJ_Loader/OBJ_Loader.h  res/models/skybox.h src/ShaderProgram.h src/Drawable.h src/ElementBuffer.h src/Texture.h src/VertexArray.h src/VertexBuffer.h src/Skybox.h src/Animated.h src/Camera.h
FILES=src/vendor/stb_image/stb_image.cpp  src/main_file.cpp  src/ShaderProgram.cpp src/Drawable.cpp src/ElementBuffer.cpp src/Texture.cpp src/VertexArray.cpp src/VertexBuffer.cpp src/Skybox.cpp src/Animated.cpp src/Camera.cpp

main_file: $(FILES) $(HEADERS)
	g++ -o main_file $(FILES)  $(LIBS) -I.
