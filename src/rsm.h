/**
 * Rocket Sim Math.
 * Contains matrices and vectors for usage. Similar to GLM.
 * Row-first ordered because why in the world would I use column-first.
 */

#ifndef RS_MATH_H
#define RS_MATH_H

typedef struct {
    float data[16];
} Mat4;

typedef struct {
    float data[3];
} vec3;

/* MAT4 */
Mat4 rsmM4Identity();
Mat4 rsmTranslate(const Mat4* const mat, const vec3* const vec);
float rsmM4Get(const unsigned int i, const unsigned int j, const Mat4* const mat);

/* VEC3 */
vec3 rsmV3(const float x, const float y, const float z);
float rsmV3Get(const unsigned int i, const vec3* const vec);

#endif // RS_MATH_H
