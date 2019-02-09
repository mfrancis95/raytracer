all: software_renderer

camera.o: camera.cpp camera.h ray.h
	g++ $(FLAGS) -c camera.cpp

clean:
	rm -f *.o software_renderer

main.o: main.cpp
	g++ $(FLAGS) -c main.cpp

phong.o: illumination.h intersection.h light.h material.h phong.cpp phong.h
	g++ $(FLAGS) -c phong.cpp

renderer.o: renderer.cpp renderer.h scene.h
	g++ $(FLAGS) -c renderer.cpp

sphere.o: primitive.h sphere.cpp sphere.h
	g++ $(FLAGS) -c sphere.cpp

software_renderer: camera.o main.o phong.o renderer.o sphere.o software_renderer.o vector.o
	g++ $(FLAGS) -o software_renderer camera.o main.o phong.o renderer.o sphere.o software_renderer.o vector.o -lGL -lSDL2

software_renderer.o: renderer.h software_renderer.cpp
	g++ $(FLAGS) -c software_renderer.cpp

vector.o: vector.cpp vector.h
	g++ $(FLAGS) -c vector.cpp