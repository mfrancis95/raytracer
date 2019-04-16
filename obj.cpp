#include <fstream>
#include <map>
#include <sstream>
#include "obj.h"
#include "triangle.h"

std::vector<Primitive *> OBJ::read(const std::string &file) {
    std::vector<Primitive *> primitives;
    std::map<int, Vector> vertices;
    auto index = 1;
    std::ifstream stream{file};
    std::string line;
    while (std::getline(stream, line)) {
        std::istringstream stringStream{line};
        char type;
        stringStream >> type;
        if (type == 'f') {
            std::string vertex1;
            std::string vertex2;
            std::string vertex3;
            stringStream >> vertex1 >> vertex2 >> vertex3;
            auto vertexIndex = vertex1.find("/");
            if (vertexIndex != -1) {
                vertex1 = vertex1.substr(0, vertexIndex);
            }
            vertexIndex = vertex2.find("/");
            if (vertexIndex != -1) {
                vertex2 = vertex2.substr(0, vertexIndex);
            }
            vertexIndex = vertex3.find("/");
            if (vertexIndex != -1) {
                vertex3 = vertex3.substr(0, vertexIndex);
            }
            primitives.push_back(
                new Triangle{
                    vertices[stoi(vertex1)], vertices[stoi(vertex2)],
                    vertices[stoi(vertex3)]
                }
            );
        }
        else if (type == 'v') {
            double x;
            double y;
            double z;
            stringStream >> x >> y >> z;
            vertices[index++] = {x, y, z};
        }
    }
    return primitives;
}