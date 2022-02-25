#include "utility.h"

#include "color.h"
#include "objectList.h"
#include "shape.h"
#include "camera.h"
#include "material.h"
#include <fstream>      // std::ofstream
#include <string>      // std::ofstream

color ray_color(const ObjectList& scene_objects, const Ray& ray, int depth, const color& bg_color);


// raytracer:
//(1) calculate the ray from the eye to the pixel
//(2) determine which objects the ray intersects
//(3) compute a color for that intersection point.

int main(int argc, char** argv) {

    if (argc != 3) {
        std::cerr << "incomplete input" << std::endl;
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
        std::cerr << "Error opening file" << std::endl;
        return -1;
    }

    // Image
   // using rectangular image for debugging (so transposing will be different) // 256
    int screen_width = 400;
    int screen_height = 400;
    const int max_depth = 50;
    // default bg to black
    color bg_color = color(0);

    ObjectList scene_objects;    
    // TODO: should we find the furthest front object?  and set camera?
 
    //std::string line;
    while (ifs.good()) {
        std::string type;
        ifs >> type;
        if (type == "BACKGROUND") {
            std::cout << "adding background" << std::endl;
            ifs >> bg_color.x >> bg_color.y >> bg_color.z;
        }
        if (type == "RESOLUTION") {
            std::cout << "adding resolution" << std::endl;
            ifs >> screen_width >> screen_height;
        }
        if (type == "ANTIALIAS") {
            std::cout << "adding antialias" << std::endl;
        }
        if (type == "MAXDEPTH") {
            // for specular
            std::cout << "adding maxdepth" << std::endl;
        }
        if (type == "LIGHT") {
            std::cout << "starting light" << std::endl;
            while (ifs.good()) {
                ifs >> type;
                if (type == "DIFF") {
                    std::cout << "adding diffuse to light" << std::endl;
                }
                else if (type == "SPEC") {
                    std::cout << "adding spec to light" << std::endl;
                }
                else if (type == "POS") {
                    std::cout << "adding position to light" << std::endl;

                }
                else if (type == "//" || type.find("//") == 0) {
                    // skip if comment
                    std::cout << "comment skipping till newline" << type << std::endl;
                    ifs.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    //discard characters until newline is found
                }
                else {
                    std::cout << "--break--" << type << std::endl;
                    for (int i = 0; i < type.length(); ++i) { //we use a colon instead of in
                        ifs.unget();
                    }
                    break;
                }
            }
            std::cout << "adding light" << std::endl;
        }
        if (type == "SPHERE") {
            point3 pos = point3(0); // default to center if not specified
            double radius = 1.0; //default to unit sphere
            color diffuse = color(0);
            color specular = color(0);
            float shininess = 0.0f;
            //std::string line;

            while (ifs.good()){
                ifs >> type;
                if (type == "POS") {
                    ifs >> pos.x >> pos.y >> pos.z;
                }
                else if (type == "RADIUS") {
                    ifs >> radius;
                }
                else if (type == "DIFF") {
                    ifs >> diffuse.x >> diffuse.y >> diffuse.z;
                }
                else if (type == "SPEC") {
                    ifs >> specular.x >> specular.y >> specular.z;
                }
                else if (type == "SHININESS") {
                    ifs >> shininess;
                }
                else if (type == "//" || type.find("//") == 0) {
                    // skip if comment
                    ifs.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    //discard characters until newline is found
                }
                else {
                    for (int i = 0; i < type.length(); ++i) { //we use a colon instead of in
                        ifs.unget();
                    }
                    break;
                }
            }
            Material material = Material(diffuse, specular, shininess);
            scene_objects.add(make_shared<Sphere>(pos, radius, &material));
            std::cout << "adding sphere\n    pos: " << pos.x << "," << pos.y << "," << pos.z  << "\n    radius: " << radius << "\n    diffuse: " << diffuse.x << "," << diffuse.y << "," << diffuse.z << "\n    specular: " << specular.x << "," << specular.y << "," << specular.z << "\n    shininess: " << shininess << std::endl;
        }
        if (type == "QUAD") {
            std::cout << "starting quad" << std::endl;

            point3 pos_array[3]; // default to center if not specified
            point3* pos_ptr = pos_array;
            color diffuse = color(0);
            color specular = color(0);
            float shininess = 0.0f;
            while (ifs.good()) {
                ifs >> type;
                if (type == "POS") {
                    ifs >> pos_ptr->x >> pos_ptr->y >> pos_ptr->z;
                    pos_ptr++;
                }
                else if (type == "DIFF") {
                    ifs >> diffuse.x >> diffuse.y >> diffuse.z;
                }
                else if (type == "SPEC") {
                    ifs >> specular.x >> specular.y >> specular.z;
                }
                else if (type == "SHININESS") {
                    ifs >> shininess;
                }
                else if (type == "//" || type.find("//") == 0) {
                    // skip if comment
                    ifs.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    //discard characters until newline is found
                }
                else {
                    //std::cout << "--break--" << type << std::endl;
                    for (int i = 0; i < type.length(); ++i) { //we use a colon instead of in
                        ifs.unget();
                    }
                    break;
                }
            }
            Material material = Material(diffuse, specular, shininess);
            scene_objects.add(make_shared<Quad>(pos_array[0], pos_array[1], pos_array[2], &material));
            std::cout << "adding quad\n    pos_array[0]: " << pos_array[0].x << "," << pos_array[0].y << "," << pos_array[0].z << "\n    pos_array[1]: " << pos_array[1].x << "," << pos_array[1].y << "," << pos_array[1].z << "\n    pos_array[2]: " << pos_array[2].x << "," << pos_array[2].y << "," << pos_array[2].z << "\n    diffuse: " << diffuse.x << "," << diffuse.y << "," << diffuse.z << "\n    specular: " << specular.x << "," << specular.y << "," << specular.z << "\n    shininess: " << shininess << std::endl;

        }
        if (type == "//" || type.find("//") == 0) {
            // skip if comment
            ifs.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            //discard characters until newline is found
        }
        // dont need eof because io stream
    }
   
    std::cout << "WIDTH: " << screen_width << " HEIGHT: " << screen_height << " BG: " << bg_color.x << "," << bg_color.y << "," << bg_color.z << std::endl;

    // Camera
    Camera camera(screen_width, screen_height);
  
    // Render

    //The pixels are written out in rows with pixels left to right.
    //The rows are written out from top to bottom.
    //By convention, each of the red / green / blue components range from 0.0 to 1.0.
    
    /*writing into file image.ppm*/
    std::ofstream ofs(out_file, std::ios::out);
    ofs << "P3\n" << screen_width << ' ' << screen_height << "\n255\n";
   
    // bottom left up and to the right
    for (int j = 0; j < screen_height; j++) {
        std::cerr << "\rScanlines remaining: " << screen_height - j - 1 << ' ' << std::flush;
        color pixel_color(0, 0, 0);
        for (int i = 0; i < screen_width; i++) {
            Ray primary_ray = camera.get_ray(i, j);
            pixel_color = ray_color(scene_objects, primary_ray, max_depth, bg_color);
            write_color(ofs, pixel_color);
        }
        //break;
    }
    ofs.close();

    std::cerr << "\nDone.\n";
}


// function that returns the color of the background (a simple gradient)
// depth to prevent infinite loop
color ray_color(const ObjectList& scene_objects, const Ray& in_ray, int depth, const color& bg_color) {
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
        if (intersect.material->scatter(in_ray, intersect, attenuation, scattered)) {
            //std::cout << "attenuation:" << attenuation.x << "," << attenuation.y << "," << attenuation.z << std::endl;
            //return 0.5 * color(intersect.normal.x + 1, intersect.normal.y + 1, intersect.normal.z + 1);
            return attenuation * ray_color(scene_objects, scattered, depth - 1, bg_color);
        }
            
        return color(0, 0, 0);

    }

    // background color graient
    //glm::dvec3 unit_direction = glm::normalize(in_ray.direction());
    //double t = 0.5 * (unit_direction.y + 1.0);
    //return (1.0 - t) * color(1.0, 1.0, 1.0) + t * color(0.5, 0.7, 1.0);

    return bg_color;
    //glm::dvec3 unit_direction = glm::normalize(r.direction());
    //double t = 0.5 * (unit_direction.y + 1.0f);
    //return 1.0 - t * color(1.0f, 1.0f, 1.0f) + t * color(0.5f, 0.7f, 1.0f);
}
