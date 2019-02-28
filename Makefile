all: compute_renderer fragment_renderer software_renderer

camera.o: camera.cpp camera.h ray.h
	g++ $(FLAGS) -c camera.cpp

compute_renderer: camera.o compute_renderer.o main.o opengl_renderer.o phong.o sphere.o triangle.o vector.o
	g++ $(FLAGS) -o compute_renderer camera.o compute_renderer.o main.o opengl_renderer.o phong.o sphere.o triangle.o vector.o -lGL -lSDL2

compute_renderer.o: compute_renderer.cpp opengl_renderer.h
	g++ $(FLAGS) -c compute_renderer.cpp

clean:
	rm -f *.o compute_renderer fragment_renderer software_renderer

fragment_renderer: camera.o fragment_renderer.o main.o opengl_renderer.o phong.o sphere.o triangle.o vector.o
	g++ $(FLAGS) -o fragment_renderer camera.o fragment_renderer.o main.o opengl_renderer.o phong.o sphere.o triangle.o vector.o -lGL -lSDL2

fragment_renderer.o: fragment_renderer.cpp opengl_renderer.h
	g++ $(FLAGS) -c fragment_renderer.cpp

group.o: group.cpp group.h primitive.h
	g++ $(FLAGS) -c group.cpp

main.o: main.cpp
	g++ $(FLAGS) -c main.cpp

opengl_renderer.o: opengl_renderer.cpp renderer.h
	g++ $(FLAGS) -c opengl_renderer.cpp

phong.o: illumination.h intersection.h light.h material.h phong.cpp phong.h
	g++ $(FLAGS) -c phong.cpp

sphere.o: primitive.h sphere.cpp sphere.h
	g++ $(FLAGS) -c sphere.cpp

software_renderer: camera.o group.o main.o phong.o sphere.o software_renderer.o triangle.o vector.o
	g++ $(FLAGS) -fopenmp -o software_renderer camera.o group.o main.o phong.o sphere.o software_renderer.o triangle.o vector.o -lSDL2

software_renderer.o: renderer.h software_renderer.cpp
	g++ $(FLAGS) -c -fopenmp software_renderer.cpp

triangle.o: primitive.h triangle.cpp triangle.h
	g++ $(FLAGS) -c triangle.cpp

vector.o: vector.cpp vector.h
	g++ $(FLAGS) -c vector.cpp