struct Vector {

    double x, y, z;

    Vector() = default;
    Vector(double x, double y, double z);
    double length();
    Vector &multiply(double scalar);
    Vector &normalise();
    Vector &operator+=(const Vector &vector);

};