struct Vector {

    double x, y, z;

    Vector();
    Vector(double x, double y, double z);
    Vector &operator+=(const Vector &vector);

};