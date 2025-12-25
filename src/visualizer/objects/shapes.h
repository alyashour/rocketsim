/**
 * Contains the basic constructors for simple VObjects like:
 * - Triangle
 * - Rectangle
 * - Cube
 * - Rectangular Prism
 * - Cylinder
 * - Sphere
 * - Plane
 */

#ifndef RS_SHAPES_H
#define RS_SHAPES_H

#include "vobject.h"

// Triangles
VObject createDefaultTriangle();
VObject createTriangle(float vertices[static 9]);
VObject createTriangle9f(float x1, float y1, float z1, float x2, float y2, float z2, float x3, float y3, float z3);

/*
// Rectangles
VObject createRectangle();
VObject createCube();
VObject createRectPrism();
VObject createCylinder();
VObject createSphere();
VObject createPlane();
*/

#endif // RS_SHAPES_H
