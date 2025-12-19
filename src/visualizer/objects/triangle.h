#ifndef RS_TRIANGLE_OBJECT_H
#define RS_TRIANGLE_OBJECT_H

typedef struct TriangleObj TriangleObj;

TriangleObj* createTriangle();

void drawTriangle(const TriangleObj* const t);

void destroyTriangle(TriangleObj* const t);

#endif // RS_TRIANGLE_OBJECT_H
