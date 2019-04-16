#include <string>
#include <vector>
#include "primitive.h"

struct OBJ {

    static std::vector<Primitive *> read(const std::string &file);

};