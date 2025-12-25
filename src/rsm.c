#include "rsm.h"

/* MAT4 */
Mat4 rsmM4Identity() {
    return (Mat4){ .data = {
        1.f, 0.f, 0.f, 0.f,
        0.f, 1.f, 0.f, 0.f,
        0.f, 0.f, 1.f, 0.f,
        0.f, 0.f, 0.f, 1.f,
    }};
}

Mat4 rsmTranslate(const Mat4* const mat, const vec3* const vec) {
    const float* const m = mat->data;
    const float* const v = vec->data;
    return (Mat4){ .data = {
        m[0], m[1], m[2], m[3],
        m[4], m[5], m[6], m[7],
        m[8], m[9], m[10], m[11],
        m[12] + v[0], m[13] + v[1], m[14] + v[2], m[15]
    }};
}

float rsmGetM4(const unsigned int col, const unsigned int row, const Mat4* const mat) {
    return mat->data[4 * col + row];
}

/* VEC3 */
vec3 rsmV3(const float x, const float y, const float z) {
    return (vec3) { .data = { x, y, z } };
}
float rsmV3Get(const unsigned int i, const vec3* const vec) {
    return vec->data[i];
}
