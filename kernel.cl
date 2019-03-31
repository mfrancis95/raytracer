__kernel void main(__global float *output) {
    output[get_global_id(0) * 4] = 1;
}