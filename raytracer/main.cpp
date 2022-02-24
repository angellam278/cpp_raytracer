#include "utility.h"

#include "color.h"
#include "objectList.h"
#include "shape.h"
#include "camera.h"
#include "material.h"
#include <fstream>      // std::ofstream
color ray_color(const ObjectList& scene_objects, const Ray& ray, int depth);


// raytracer:
//(1) calculate the ray from the eye to the pixel
//(2) determine which objects the ray intersects
//(3) compute a color for that intersection point.

int main(int argc, char** argv) {

    if (argc != 3) {
        std::cout << "incomplete input" << std::endl;
        return -1;
    }
    // argv[0] is .exe
    const char* scene_des = argv[1];
    const char* out_file = argv[2];
    std::cout << "scene description file: " << scene_des << "\noutput: " << out_file << std::endl;
    /*Opening and parsing of scene description file*/
    std::ifstream ifs;
    ifs.open(scene_des, std::ios::in);
    if (!ifs.is_open()) {
        std::cout << "Error opening file" << std::endl;
        return -1;
    }

    while (ifs.good()) {
        std::string type;
        ifs >> type;
        std::cout << "type: " << type << std::endl;
        if (type == "LIGHTS") {
        }
    }
    // Image
    // using rectangular image for debugging (so transposing will be different) // 256
    const double aspect_ratio = 16.0 / 9.0;
    const int image_width = 400;
    const int image_height = static_cast<int>(image_width / aspect_ratio);
    const int max_depth = 50;
    // TODO let camera input these info 
    
    // World


    //make sphere
    // into the screen is the negative z-axis
    ObjectList scene_objects;
    auto material_ground = make_shared<Lambertian>(color(0.8, 0.8, 0.0));
    auto material_center = make_shared<Lambertian>(color(0.7, 0.3, 0.3));
    auto material_left = make_shared<Metal>(color(0.8, 0.8, 0.8));
    //scene_objects.add(make_shared<Sphere>(point3(0.0, -100.5, -1.0), 100.0, material_ground));
    scene_objects.add(make_shared<Quad>(point3(1, - 1, 0), point3(1, - 1, - 2), point3(1, 1, 0), material_center));
    //scene_objects.add(make_shared<Sphere>(point3(0.0, 0.0, -2.0), 1.0, material_center));
    //scene_objects.add(make_shared<Triangle>(point3(-1.0, 0.0, -2.0), point3(0.0, 0.0, -2.0), point3(-1.0, 1.0, -2.0), material_ground));
    //scene_objects.add(make_shared<Sphere>(point3(-1.0, 0.0, -1.0), 0.5, material_left));
    /*Cube cube = Cube(point3(0.0, 0.0, 0.0), 10, material_left);
    for (auto t : cube.triangle_list) {
        scene_objects.add(make_shared<Triangle>(t.p1, t.p2, t.p3, cube.material_ptr));
    }*/
    double length = 100.0;
    double half = length / 2;

    // left front corner at 0,0,0
    // bottom square
    point3 offset = point3(-half, 0.0, -1.5);
    point3 p1 = point3(half, 0.0, 0.0) + offset;
    point3 p2 = point3(0.0, 0.0, 0.0) + offset;
    point3 p3 = point3(half, 0.0, half) + offset;
    point3 p4 = point3(0.0, 0.0, half) + offset;

    // top square
    point3 p5 = point3(half, half, 0.0) + offset;
    point3 p6 = point3(0.0, half, 0.0) + offset;
    point3 p7 = point3(half, half, half) + offset;
    point3 p8 = point3(0.0, half, half) + offset;

    //std::cout << "p1: " << p1.x << " " << p1.y << " " << p1.z << std::endl;
    //std::cout << "p2: " << p2.x << " " << p2.y << " " << p2.z << std::endl;
    //std::cout << "p3: " << p3.x << " " << p3.y << " " << p3.z << std::endl;
    //std::cout << "p4: " << p4.x << " " << p4.y << " " << p4.z << std::endl;
    //std::cout << "p5: " << p5.x << " " << p5.y << " " << p5.z << std::endl;
    //std::cout << "p6: " << p6.x << " " << p6.y << " " << p6.z << std::endl;
    //std::cout << "p7: " << p7.x << " " << p7.y << " " << p7.z << std::endl;
    //std::cout << "p8: " << p8.x << " " << p8.y << " " << p8.z << std::endl;

    // Floor:
    //scene_objects.add(make_shared<Triangle>(p1, p2, p4, material_ground));
    //scene_objects.add(make_shared<Triangle>(p3, p4, p2, material_ground));

    //// back wall:
    //scene_objects.add(make_shared<Triangle>(p7, p4, p3, material_left));
    //scene_objects.add(make_shared<Triangle>(p7, p8, p4, material_left));
    

    // Camera
    Camera camera;
  
    // Render

    std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

    //The pixels are written out in rows with pixels left to right.
    //The rows are written out from top to bottom.
    //By convention, each of the red / green / blue components range from 0.0 to 1.0.
    
    /*writing into file image.ppm*/
    std::ofstream ofs(out_file, std::ios::out);
   
    for (int j = image_height - 1; j >= 0; --j) {
        std::cerr << "\rScanlines remaining: " << j << ' ' << std::flush;
        color pixel_color(0, 0, 0);
        for (int i = 0; i < image_width; ++i) {

            // x, y coordinates on the image 
            double u = double(i) / (image_width - 1.0);
            double v = double(j) / (image_height - 1.0);

            Ray primary_ray = camera.get_ray(u, v);
            pixel_color = ray_color(scene_objects, primary_ray, max_depth);

            //color pixel_color(double(i) / (image_width - 1), double(j) / (image_height - 1), 0.25);
            write_color(ofs, pixel_color);
        }
    }
    ofs.close();

    std::cerr << "\nDone.\n";
}


// function that returns the color of the background (a simple gradient)
// depth to prevent infinite loop
color ray_color(const ObjectList& scene_objects, const Ray& in_ray, int depth) {
    intersection intersect;

    // If we've exceeded the ray bounce limit, no more light is gathered.
    if (depth <= 0)
        return color(0, 0, 0);

    // 0.001 instead of 0 to get rid of shadow acne, ignoring hits very near zero
    if (scene_objects.getIntersect(in_ray, 0.001, infinity, intersect)) {
        //point3 target = intersect.point + intersect.normal + random_in_unit_sphere();
        //True Lambertian has the probability higher for ray scattering close to the normal, but the distribution is more uniform. 
        //True Lambertian has the probability higher for ray scattering close to the normal, but the distribution is more uniform. 
        //point3 target = intersect.point + intersect.normal + random_unit_vector();
        // regular diffuse
        //point3 target = intersect.point + intersect.normal + random_in_hemisphere(intersect.normal);

        // absorb 50 and reflects 50%
        //return 0.5 * ray_color(scene_objects, ray(intersect.point, target - intersect.point), depth - 1);
    
        Ray scattered;
        color attenuation;
        if (intersect.material_ptr->scatter(in_ray, intersect, attenuation, scattered)) {
            //std::cout << "attenuation:" << attenuation.x << "," << attenuation.y << "," << attenuation.z << std::endl;
            //return 0.5 * color(intersect.normal.x + 1, intersect.normal.y + 1, intersect.normal.z + 1);
            return attenuation * ray_color(scene_objects, scattered, depth - 1);
        }
            
        return color(0, 0, 0);

    }
    glm::dvec3 unit_direction = glm::normalize(in_ray.direction());
    double t = 0.5 * (unit_direction.y + 1.0);
   
    // background color
    return (1.0 - t) * color(1.0, 1.0, 1.0) + t * color(0.5, 0.7, 1.0);

    //glm::dvec3 unit_direction = glm::normalize(r.direction());
    //double t = 0.5 * (unit_direction.y + 1.0f);
    //return 1.0 - t * color(1.0f, 1.0f, 1.0f) + t * color(0.5f, 0.7f, 1.0f);
}
