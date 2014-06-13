// Copyright 2001 softSurfer, 2012 Dan Sunday
// This code may be freely used and modified for any purpose
// providing that this copyright notice is included with it.
// SoftSurfer makes no warranty for this code, and cannot be held
// liable for any real or imagined damage resulting from its use.
// Users of this code must verify correctness for their application.


// Assume that classes are already given for the objects:
//    Point and Vector with
//        coordinates {float x, y, z;}
//        operators for:
//            == to test  equality
//            != to test  inequality
//            (Vector)0 =  (0,0,0)         (null vector)
//            Point   = Point ¡À Vector
//            Vector =  Point - Point
//            Vector =  Scalar * Vector    (scalar product)
//            Vector =  Vector * Vector    (cross product)
//    Line and Ray and Segment with defining  points {Point P0, P1;}
//        (a Line is infinite, Rays and  Segments start at P0)
//        (a Ray extends beyond P1, but a  Segment ends at P1)
//    Plane with a point and a normal {Point V0; Vector  n;}
//    Triangle with defining vertices {Point V0, V1, V2;}
//    Polyline and Polygon with n vertices {int n;  Point *V;}
//        (a Polygon has V[n]=V[0])
//===================================================================

#include "Vertex.h"
#include "Vec3d.h"
#include <vector>


#define SMALL_NUM   0.00000001 // anything that avoids division overflow
// dot product (3D) which allows vector operations in arguments
#define dot(u,v)   ((u).x * (v).x + (u).y * (v).y + (u).z * (v).z)

class RealTriangle {
public:
	//inline RealTriangle() {
	//	v[0] = v[1] = v[2] = 0;
	//}
	inline RealTriangle(const RealTriangle & t) {
		v.push_back(t.v[0]);
		v.push_back(t.v[1]);
		v.push_back(t.v[2]);
	}
	inline RealTriangle(Vec3Df v0, Vec3Df v1, Vec3Df v2) {
		v.push_back(v0);
		v.push_back(v1);
		v.push_back(v2);
	}
	inline virtual ~RealTriangle() {}
	inline RealTriangle & operator= (const RealTriangle & t) {
		v.push_back(t.v[0]);
		v.push_back(t.v[1]);
		v.push_back(t.v[2]);
		return (*this);
	}

	//unsigned int v[3];
	std::vector<Vec3Df> v;
};


// intersect3D_RayTriangle(): find the 3D intersection of a ray with a triangle
//    Input:  a ray R, and a triangle T
//    Output: *I = intersection point (when it exists)
//    Return: -1 = triangle is degenerate (a segment or point)
//             0 =  disjoint (no intersect)
//             1 =  intersect in unique point I1
//             2 =  are in the same plane
int
intersect3D_RayTriangle(Vec3Df R1, Vec3Df R2, RealTriangle T)//, Vec3Df* I)
{
	Vec3Df* I = &Vec3Df(0,0,0);
	Vec3Df    u, v, n;              // triangle vectors
	Vec3Df    dir, w0, w;           // ray vectors
	float     r, a, b;              // params to calc ray-plane intersect

	// get triangle edge vectors and plane normal
	u = T.v[1] - T.v[0];
	v = T.v[2] - T.v[0];
	n = Vec3Df::crossProduct(u, v);              // cross product
	if (n == Vec3Df(0, 0, 0))             // triangle is degenerate
		return -1;                  // do not deal with this case

	dir = R2 - R1;              // ray direction vector
	w0 = R1 - T.v[0];
	a = -Vec3Df::dotProduct(n, w0);
	b = Vec3Df::dotProduct(n, dir);
	if (fabs(b) < SMALL_NUM) {     // ray is  parallel to triangle plane
		if (a == 0)                 // ray lies in triangle plane
			return 2;
		else return 0;              // ray disjoint from plane
	}

	// get intersect point of ray with triangle plane
	r = a / b;
	if (r < 0.0)                    // ray goes away from triangle
		return 0;                   // => no intersect
	// for a segment, also test if (r > 1.0) => no intersect

	*I = R1 + r * dir;            // intersect point of ray and plane

	// is I inside T?
	float    uu, uv, vv, wu, wv, D;
	uu = Vec3Df::dotProduct(u, u);
	uv = Vec3Df::dotProduct(u, v);
	vv = Vec3Df::dotProduct(v, v);
	w = *I - T.v[0];
	wu = Vec3Df::dotProduct(w, u);
	wv = Vec3Df::dotProduct(w, v);
	D = uv * uv - uu * vv;

	// get and test parametric coords
	float s, t;
	s = (uv * wv - vv * wu) / D;
	if (s < 0.0 || s > 1.0)         // I is outside T
		return 0;
	t = (uv * wu - uu * wv) / D;
	if (t < 0.0 || (s + t) > 1.0)  // I is outside T
		return 0;

	return 1;                       // I is in T
}