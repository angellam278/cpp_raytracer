
#include "shape.h"

Sphere::Sphere() {}
Sphere::Sphere(point3 center, double radius, std::shared_ptr<Material> material) {
    this->position = center;

    // make matrices
    //translate(center);
    //scale(glm::vec3(radius));
    // center is position
    this->radius = radius;
    this->material_ptr = material;
}

Sphere::~Sphere() {}

Quad::Quad() {}
Quad::~Quad() {}
Quad::Quad(point3 p1, point3 p2, point3 p3, std::shared_ptr<Material> material) {
    this->p1 = p1;
    this->p2 = p2;
    this->p3 = p3;
    this->material_ptr = material;
}

void Quad::rotateX(float degrees) {

}

bool Quad::getIntersect(const Ray& ray, double t_min, double t_max, intersection& intersect) {
    // bottom left front corner: position
    // bottom plane: x = btc + u(0, 0, -height) + v(width, 0, 0)

    glm::dvec3 v1 = p2 - p1;
    glm::dvec3 v2 = p3 - p1;
    //print_vec3("v1", v1);
    //print_vec3("v2", v2);

    double width = glm::l2Norm(p2, p1);
    double height = glm::l2Norm(p3, p1);
    //std::cout << width << std::endl;
    //std::cout << height << std::endl;
    glm::dvec3 normal = glm::cross(v2, v1);
    /*print_vec3("end1", end1);
    print_vec3("end2", end2);
    print_vec3("v1", v1);
    print_vec3("v2", v2);
    print_vec3("normal", normal);*/
    double dd = glm::dot(normal, p1);
    //std::cout << dd << std::endl;
    double denom = glm::dot(normal, ray.direction());
    //std::cout << denom << std::endl;
    //  make overloaded near_zero: parallel 
    if (glm::abs(denom) < epsilon) {
        //std::cout << "denom near 0" << std::endl;
        return false;
    }
    double t = -(glm::dot(normal, (ray.origin() - p1))) / denom;
    point3 poi = ray.at(t);
    double u = glm::dot((poi - p1), v1);
    double v = glm::dot((poi - p1), v2);

    if ((t < infinity) && (t > 0.0) && (abs(u) <= width) && (abs(v) <= height)) {
        intersect.t = t;
        // point of intersection in global coordinate
        intersect.point = ray.at(intersect.t);
        // unnormalized normal
        intersect.set_face_normal(ray, normal);
        // return point of intersection ? or object just updates its normal? 
        intersect.material_ptr = material_ptr;

        return true;
    }
     
    //if (t < 0) {
    //    // behind
    //    return false;
    //}
    //intersect.t = t;
    //// point of intersection in global coordinate
    //intersect.point = ray.at(intersect.t);
    //// normalized normal
    //intersect.set_face_normal(ray, normal);
    //// return point of intersection ? or object just updates its normal? 
    //intersect.material_ptr = material_ptr;

    //return true;
    //std::cout << t << std::endl;
    //if ((t < infinity) && (t > 0.0)) {
    //    intersect.t = t;
    //    // point of intersection in global coordinate
    //    intersect.point = ray.at(intersect.t);
    //    // normalized normal
    //    intersect.set_face_normal(ray, normal);
    //    // return point of intersection ? or object just updates its normal? 
    //    intersect.material_ptr = material_ptr;

    //    return true;
    //}
    //// t = (by - ay)/ ry
    //// u = (bz - az - kz ((by - ay)/ky))/h 
    //// v = (ax - bx + kx ((by - ay)/ky))/w
    //glm::dvec3 d = ray.direction();
    ////k.Normalize();
    //double kx = d.x;
    //double ky = d.y;
    //double kz = d.z;

    //double ax = ray.origin().x;
    //double ay = ray.origin().y;
    //double az = ray.origin().z;
    ///*double t = (ay + 1.0) / -ky;

    //double u = ax + kx * t;
    //double v = az + kz * t;*/

    //double t = (ay - position.y) / ky;
    //double u = ax - position.x - kx * t;
    //double v = position.z - az + kz * t;

    //if ((t < infinity) && (t > 0.0) && (abs(u) <= width) && (abs(v) <= height)) {
    //    intersect.t = t;
    //    // point of intersection in global coordinate
    //    intersect.point = ray.at(intersect.t);
    //    // normalized normal
    //    glm::dvec3 outward_normal = glm::dvec3(0, 1, 0); // top
    //    intersect.set_face_normal(ray, outward_normal);
    //    // return point of intersection ? or object just updates its normal? 
    //    intersect.material_ptr = material_ptr;

    //    return true;
    //}

    return false;
}
//
//Box::Box() {}
//Box::~Box() {}
//Box::Box(point3 center, double width, double height, double depth, std::shared_ptr<Material> material) {
//    this->position = center;
//    this->width = width;
//    this->height = height;
//    this->depth = depth;
//    this->material_ptr = material;
//}
//
//bool Box::getIntersect(const Ray& ray, double t_min, double t_max, intersection& intersect) {
//    // Extract values of a.
//    double rx = ray.origin().x;
//    double ry = ray.origin().y;
//    double rz = ray.origin().z;
//
//    // Extract the value of k.
//    glm::dvec3 d = ray.direction();
//    //k.Normalize();
//    double dx = d.x;
//    double dy = d.y;
//    double dz = d.z;
//
//    return false;
//
//    // plane: x = p0 + u(p1) + v(p2)
//    // line: x = a + td
//    // intersection = a + td = p0 + u(p1) + v(p2)
//
//    // bottom left front corner: btc = position + (-width/2, -height/2, depth/2)  << negative z is into the screen
//    // bottom plane: x = btc + u(0, 0, -depth) + v(width, 0, 0)
//    // front plane: x = btc + u(0, height, 0) + v(width, 0, 0)
//    // right plane: x = (btc + (width, 0, 0)) + u(0, height, 0) + v(0, 0, -depth)
//    // left plane: x = btc + u(0, height, 0) + v(0, 0, -depth)
//    // back plane: x = (btc + (0, 0, depth)) + u(0, height, 0) + v(0, width, 0)
//    // top plane: x = (btc + (0, height, 0)) + u(0, 0, -depth) + v(0, width, 0)
//    // 
//    // Test for intersections with each plane (side of the box).
//    // Top and bottom.
//    //if (glm::abs(dz) > epsilon)
//    //{
//    //    t[0] = (az - 1.0) / -dz;
//    //    t[1] = (az + 1.0) / -dz;
//    //    u[0] = ax + kx * t[0];
//    //    v[0] = ay + ky * t[0];
//    //    u[1] = ax + kx * t[1];
//    //    v[1] = ay + ky * t[1];
//    //}
//    //else
//    //{
//    //    t[0] = 100e6;
//    //    t[1] = 100e6;
//    //    u[0] = 0.0;
//    //    v[0] = 0.0;
//    //    u[1] = 0.0;
//    //    v[1] = 0.0;
//    //}
//
//    //// Left and right.
//    //if (!CloseEnough(kx, 0.0))
//    //{
//    //    t[2] = (ax + 1.0) / -kx;
//    //    t[3] = (ax - 1.0) / -kx;
//    //    u[2] = az + kz * t[2];
//    //    v[2] = ay + ky * t[2];
//    //    u[3] = az + kz * t[3];
//    //    v[3] = ay + ky * t[3];
//    //}
//    //else
//    //{
//    //    t[2] = 100e6;
//    //    t[3] = 100e6;
//    //    u[2] = 0.0;
//    //    v[2] = 0.0;
//    //    u[3] = 0.0;
//    //    v[3] = 0.0;
//    //}
//
//    //// Front and back.
//    //if (!CloseEnough(ky, 0.0))
//    //{
//    //    t[4] = (ay + 1.0) / -ky;
//    //    t[5] = (ay - 1.0) / -ky;
//    //    u[4] = ax + kx * t[4];
//    //    v[4] = az + kz * t[4];
//    //    u[5] = ax + kx * t[5];
//    //    v[5] = az + kz * t[5];
//    //}
//    //else
//    //{
//    //    t[4] = 100e6;
//    //    t[5] = 100e6;
//    //    u[4] = 0.0;
//    //    v[4] = 0.0;
//    //    u[5] = 0.0;
//    //    v[5] = 0.0;
//    //}
//
//    //// Find the index of the smallest non-negative value of t.
//    //double finalU = 0.0;
//    //double finalV = 0.0;
//    //double finalT = 100e6;
//    //int finalIndex = 0;
//    //bool validIntersection = false;
//    //for (int i = 0; i < 6; ++i)
//    //{
//    //    if ((t[i] < finalT) && (t[i] > 0.0) && (abs(u[i]) <= 1.0) && (abs(v[i]) <= 1.0))
//    //    {
//    //        finalT = t[i];
//    //        finalIndex = i;
//    //        finalU = u[i];
//    //        finalV = v[i];
//    //        validIntersection = true;
//    //    }
//    //}
//
//    //if (validIntersection)
//    //{
//    //    // Compute the point of intersection.
//    //    qbVector<double> poi = bckRay.m_point1 + finalT * k;
//
//    //    // Compute the normal vector
//    //    qbVector<double> normalVector{ 3 };
//    //    switch (finalIndex)
//    //    {
//    //    case 0:
//    //        normalVector = std::vector<double>{ 0.0, 0.0, 1.0 }; // Down.
//    //        break;
//
//    //    case 1:
//    //        normalVector = std::vector<double>{ 0.0, 0.0, -1.0 }; // Up.
//    //        break;
//
//    //    case 2:
//    //        normalVector = std::vector<double>{ -1.0, 0.0, 0.0 }; // Left.
//    //        break;
//
//    //    case 3:
//    //        normalVector = std::vector<double>{ 1.0, 0.0, 0.0 }; // Right.
//    //        break;
//
//    //    case 4:
//    //        normalVector = std::vector<double>{ 0.0, -1.0, 0.0 }; // Backwards (towards the camera).
//    //        break;
//
//    //    case 5:
//    //        normalVector = std::vector<double>{ 0.0, 1.0, 0.0 }; // Forwards (away from the camera).
//    //        break;
//
//    //    }
//
//    //    // Transform the intersection point back into world coordinates.
//    //    intPoint = m_transformMatrix.Apply(poi, qbRT::FWDTFORM);
//
//    //    // Transform the normal vector.
//    //    localNormal = m_transformMatrix.ApplyNorm(normalVector);
//    //    localNormal.Normalize();
//
//    //    // Return the base color.
//    //    localColor = m_baseColor;
//
//    //    // Store the (u,v) coordinates for possible later use.
//    //    m_uvCoords.SetElement(0, finalU);
//    //    m_uvCoords.SetElement(1, finalV);
//
//    //    return true;
//    //}
//    //else
//    //{
//    //    return false;
//    //}
//
//}


//// Overload operators.
//// transform the origin and direction of the ray 
//Ray operator*(const glm::mat4 mat, const Ray in_ray) {
//    // make the direction and origin 
//    // 4th component is 1 for point
//    return Ray(point3(mat * glm::dvec4(in_ray.origin(), 1.0)), glm::dvec3(mat * glm::dvec4(in_ray.direction(), 0)));
//}

//Sphere::Sphere(point3 center, double radius, shared_ptr<Material> material) :
//    position(center), radius(radius), material_ptr(material) {};

/*
    Sphere intersection
        - solving equation for a point on ray which is at distance r from center
*/
bool Sphere::getIntersect(const Ray& ray, double t_min, double t_max, intersection& intersect) {
    // (p-c).(p-c) = r^2 where p = point on sphere, c = center of sphere and r is the radius
    // p = ray_origin + t(ray_direction), we are trying to find the t in which the ray intersects the sphere
    // --> ((ray_origin + t(ray_direction)) - c).((ray_origin + t(ray_direction)) - c) = r^2
    // --> ((ray_origin - c) + t(ray_direction)).((ray_origin - c) + t(ray_direction)) = r^2
    // -- > (origin_center + t(ray_direction)).(origin_center + t(ray_direction)) = r^2
    // -- > (ray_direction.ray_direction)t^2 + 2(origin_center.ray_direction)t + (origin_center.origin_center - r^2) = 0
    // -- > at^2 + 2bt + c = 0

    // bring ray to local coordinate
    //Ray back_ray = glm::inverse(transform_matrix) * ray;

    //ray_direction.ray_direction
    //double a = 1.0; 
    //glm::length2(back_ray.direction());
    glm::dvec3 oc = ray.origin() - position;
    double a = glm::length2(ray.direction());
    auto half_b = glm::dot(oc, ray.direction());
    auto c = glm::length2(oc) - (radius * radius);

    auto discriminant = half_b * half_b - a * c;
    if (discriminant < 0) return false;
    auto sqrtd = sqrt(discriminant);

    // Find the nearest root that lies in the acceptable range.
    auto root = (-half_b - sqrtd) / a;
    if (root < t_min || t_max < root) {
        root = (-half_b + sqrtd) / a;
        if (root < t_min || t_max < root)
            return false;
    }

    // record intersection info: DO WE NEED?
    intersect.t = root;
    // point of intersection in global coordinate
    intersect.point = ray.at(intersect.t);
    // normalized normal
    glm::dvec3 outward_normal = (intersect.point - position) / radius;
    intersect.set_face_normal(ray, outward_normal);
    // return point of intersection ? or object just updates its normal? 
    intersect.material_ptr = material_ptr;


    return true;

    //// TODO: if new method, always working in LOCAL COORDINATE SO THE POSITION IS 0 so no need this 
    ////glm::dvec3 origin_center = back_ray.origin();
    //// get squared length to avoid expensive sqrt = origin_center.origin_center
    //double oc_sqr_length = glm::length2(origin_center);
    //// this is 2 . w . v where 2 is factored out because it will be divided later
    ////double half_b = glm::dot(origin_center, glm::normalize(back_ray.direction()));
    //double b = 2.0 * glm::dot(back_ray.origin(), glm::normalize(back_ray.direction()));
    //// 1 instead of radius * radius because we are dealing with local coordinates (unit sphere)
    ////double c = oc_sqr_length - 1;
    //double c = glm::dot(back_ray.origin(), back_ray.origin()) - 1.0;
    //// b^2 - 4ac, but 4 is factored out because it will be sqrt and divided by 2
    ////double discriminant = (half_b * half_b) - (a * c);
    //double discriminant = (b * b) - (4.0 * c); // a = 1

    //if (discriminant < 0) {
    //    // no hit
    //    return false;
    //}
    //else {
    //    // Find the nearest root that lies in the acceptable range.
    //    auto sqrtd = sqrt(discriminant);
    //    auto t1 = (-b - sqrtd) / 2.0; // t
    //    auto t2 = (-b + sqrtd) / 2.0;
    //    /*if (t < t_min || t_max < t) {
    //        t = (-half_b + sqrtd) / a;
    //        if (t < t_min || t_max < t) {
    //            return false;
    //        }
    //    }*/
    //    double t;
    //    // check if both t are within range
    //    if ((t1 > t_min && t1 < t_max) || (t2 > t_min && t2 < t_max)) {
    //        // find the min t 
    //        t = std::min(t1, t2);
    //    }
    //    else {
    //        return false;
    //    }
    //    
    //    // record intersection info: DO WE NEED?
    //    intersect.t = t;
    //    // point of intersection in local coordinate
    //    point3 poi = back_ray.at(intersect.t);
    //    // transform back to global coordinates
    //    intersect.point = transform_matrix * glm::dvec4(poi, 1.0);
    //    // don't need to -position because we are dealing local coordinates, 
    //    // normalized (don't need to /radius) because radius is 1
    //    // transform normals (for scaling)
    //    glm::dvec3 local_normal = glm::transpose(glm::inverse(transform_matrix)) * glm::dvec4(poi, 0.0);
    //    glm::dvec3 outward_normal = transform_matrix * glm::vec4(local_normal, 0.0);
    //    intersect.set_face_normal(back_ray, outward_normal);
    //    // return point of intersection ? or object just updates its normal? 
    //    intersect.material_ptr = material_ptr;

    //    return true;
    //}
}

glm::dvec3 Sphere::getNormal(const point3 point) {
    // normalized normal (center = position)
    glm::dvec3 normal = point - position / radius;
    return normal;
}

//
//Triangle::Triangle(point3 p1, point3 p2, point3 p3, std::shared_ptr<Material> material) {
//
//    this->p1 = p1;
//    this->p2 = p2;
//    this->p3 = p3;
//
//    // make matrices
//   // this->transform_matrix = glm::translate(glm::dmat4(1.0), center);
//    // center is position
//    this->material_ptr = material;
//}
//
//glm::dvec3 Triangle::getNormal() {
//    // normalized normal (center = position)
//    glm::dvec3 a = p2 - p1; // edge 0 
//    glm::dvec3 b = p3 - p1; // edge 1 
//    glm::dvec3 c = cross(a, b); // this is the triangle's normal 
//    //std::cout << "NORMAL: " << c.x << " " << c.y << " " << c.z << std::endl;
//    return glm::normalize(c);
//}
//
//bool Triangle::getIntersect(const Ray& ray, double t_min, double t_max, intersection& intersect) {
//    glm::dvec3 normal = this->getNormal();
//    // see triangle as a plane : Ax+By+Cz+D=0 
//    // where D is the distance from the worlds origin to the plane (parallel to plane's normal)
//    // to find D we take dot product of triangle's normal with any point (project point onto normal)
//    double d = glm::dot(normal, this->p1);
//
//    // if the ray and the plane are parallel they wont intersect 
//    double dot_n_dir = glm::dot(normal, ray.direction());
//    if (glm::abs(dot_n_dir) < epsilon) {
//        // very close to zero, cannot use as denominator to find t
//        //std::cout << "f2: " << dot_n_dir << std::endl;
//        return false;
//    }
//
//    // we want to find t that makes ray intersect the triangle
//    // substituting ray input to x,y,z of plane's equation gives us
//    double t = (d - glm::dot(normal, ray.origin())) / dot_n_dir;
//
//    if (t < 0) {
//        // the triangle is behind the ray's origin (behind = not visible)
//        //std::cout << "f3" << t << " " << d  << " " << dot_n_dir << std::endl;
//        return false;
//    }
//    point3 intersection_point = ray.at(t);
//
//    // check if intersection point is inside the triangle (inside-outside test)
//    glm::dvec3 c; // vector perpendicular to triangle's plane 
//
//    // edge 1
//    glm::dvec3 edge1 = this->p2 - this->p1;
//    glm::dvec3 vp1 = intersection_point - this->p1;
//    c = glm::cross(edge1, vp1);
//    if (glm::dot(normal, c) < 0) return false; // P is on the right side 
//
//    // edge 2
//    glm::dvec3 edge2 = this->p3 - this->p2;
//    glm::dvec3 vp2 = intersection_point - this->p2;
//    c = glm::cross(edge2, vp2);
//    if (glm::dot(normal, c) < 0)  return false; // P is on the right side 
//
//    // edge 3
//    glm::dvec3 edge3 = this->p1 - this->p3;
//    glm::dvec3 vp3 = intersection_point - this->p3;
//    c = glm::cross(edge3, vp3);
//    if (glm::dot(normal, c) < 0) return false; // P is on the right side; 
//
//    // record intersection info: 
//    intersect.t = t;
//    // point of intersection 
//    intersect.point = intersection_point;
//    intersect.set_face_normal(ray, normal);
//    // return point of intersection ? or object just updates its normal? 
//    intersect.material_ptr = material_ptr;
//    //std::cout << "intersection_point: " << intersection_point.x << "," << intersection_point.y << "," << intersection_point.z << std::endl;
//    return true; // this ray hits the triangle 
//}

//
//Cube::Cube(point3 center, double length, std::shared_ptr<Material> material) {
//    this->position = center;
//    this->length = length;
//    this->material_ptr = material;
//
//    double half = length / 2;
//
//    // left front corner at 0,0,0
//    // bottom square
//    this->p1 = point3(half, 0.0, 0.0);
//    this->p2 = point3(0.0, 0.0, 0.0);
//    this->p3 = point3(half, 0.0, half);
//    this->p4 = point3(0.0, 0.0, half);
//
//    // top square
//    this->p5 = point3(half, half, 0.0);
//    this->p6 = point3(0.0, half, 0.0);
//    this->p7 = point3(half, half, half);
//    this->p8 = point3(0.0, half, half);
//
//    // Floor:
//    triangle_data t1;
//    t1.p1 = p1;
//    t1.p2 = p2;
//    t1.p3 = p4;
//    triangle_data t2;
//    t1.p1 = p3;
//    t1.p2 = p4;
//    t1.p3 = p2;
//    triangle_list.push_back(t1);
//    triangle_list.push_back(t2);
//
//    // Left wall
//    //triangle_list.push_back(Triangle(p1, p5, p3, material));
//    //triangle_list.push_back(Triangle(p3, p5, p6, material));
//
//    //// Right wall
//    //triangle_list.push_back(Triangle(p6, p2, p4, material));
//    //triangle_list.push_back(Triangle(p7, p6, p4, material));
//
//    //// Ceiling
//    //triangle_list.push_back(Triangle(p5, p6, p7, material));
//    //triangle_list.push_back(Triangle(p6, p8, p7, material));
//
//    //// Back wall
//    //triangle_list.push_back(Triangle(p7, p4, p3, material));
//    //triangle_list.push_back(Triangle(p7, p8, p4, material));
//}
