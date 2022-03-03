#include "utility.h"

#include "color.h"
#include "shape.h"
#include "camera.h"
#include "material.h"
#include "light.h"
#include <fstream> // std::ofstream
#include <string> 
#include <omp.h>
#include <iostream>
#include <vector>

// forward declaration
color ray_color(const std::vector<std::shared_ptr<Object>>& scene_objects, const std::vector<Light>& scene_lights, const Ray& ray, int depth, const color& bg_color, const Camera& camera);
color get_phong(const intersection& intersection, const Light& light, const Camera& camera);


// raytracer's main:
// (1) calculate the ray from the eye to the pixel
// (2) determine which objects the ray intersects
// (3) compute a color for that intersection point.
// USAGE: .exe scene_description.txt output_file_path.ppm

int main(int argc, char** argv) {

    // check user inputs
    if (argc != 3) {
        std::cerr << "incomplete input" << std::endl;
        return -1;
    }
    // argv[0] is .exe
    const char* scene_des = argv[1];
    const char* out_file = argv[2];
    //if (std::experimental::filesystem::is_directory(scene_des)) {
    //    for (const auto& file : std::experimental::filesystem::directory_iterator("")) {
    //        std::cout << file.path() << std::endl;
    //        // output many images and use ffmpeg to combine 
    //    }
    //}
    
    std::cout << "scene description file: " << scene_des << "\noutput: " << out_file << std::endl;

    // Opening and parsing of scene description file
    std::ifstream ifs;
    ifs.open(scene_des, std::ios::in);
    if (!ifs.is_open()) {
        std::cerr << "Error opening file" << std::endl;
        return -1;
    }

    // Image specification (default)
    int screen_width = 400;
    int screen_height = 400;
    int max_depth = 50; // to limit specular reflection bounces
    color bg_color = color(0); // black

    // list of scene shapes and lights
    std::vector<std::shared_ptr<Object> > scene_objects;
    std::vector<Light> scene_lights;

    // start reading file to build scene
    while (ifs.good()) {
        std::string type;
        ifs >> type;
        if (type == "BACKGROUND") {
            ifs >> bg_color.x >> bg_color.y >> bg_color.z;
        }
        else if (type == "RESOLUTION") {
            ifs >> screen_width >> screen_height;
        }
        else if (type == "ANTIALIAS") {
            std::cout << "adding antialias" << std::endl;
        }
        else if (type == "MAXDEPTH") {
            ifs >> max_depth;
        }
        else if (type == "LIGHT") {
            point3 pos;
            color diffuse;
            color specular;

            while (ifs.good()) {
                ifs >> type;
                if (type == "DIFF") {
                    ifs >> diffuse.x >> diffuse.y >> diffuse.z;
                }
                else if (type == "SPEC") {
                    ifs >> specular.x >> specular.y >> specular.z;
                }
                else if (type == "POS") {
                    ifs >> pos.x >> pos.y >> pos.z;
                }
                else if (type == "//" || type.find("//") == 0) {
                    // skip if comment: discard characters until newline is found
                    ifs.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                }
                else {
                    for (int i = 0; i < type.length(); ++i) { 
                        // put the type back into outer level: check if its any objects
                        ifs.unget();
                    }
                    break;
                }
            }

            // create light in scene
            Light light = Light(pos, diffuse, specular);
            scene_lights.push_back(light);
            std::cout << "adding light\n    pos: " << pos.x << "," << pos.y << "," << pos.z << "\n    diffuse: " << diffuse.x << "," << diffuse.y << "," << diffuse.z << "\n    specular: " << specular.x << "," << specular.y << "," << specular.z << std::endl;

        }

        else if (type == "SPHERE") {
            point3 pos = point3(0); // default to center if not specified
            double radius = 1.0; //default to unit sphere
            color diffuse = color(0);
            color specular = color(0);
            float shininess = 0.0f;

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
                    // skip if comment: discard characters until newline is found
                    ifs.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                }
                else {
                    for (int i = 0; i < type.length(); ++i) { 
                        ifs.unget();
                    }
                    break;
                }
            }

            // adding sphere
            scene_objects.push_back(std::make_shared<Sphere>(pos, radius, new Material(diffuse, specular, shininess)));
            std::cout << "adding sphere\n    pos: " << pos.x << "," << pos.y << "," << pos.z << "\n    radius: " << radius << "\n    diffuse: " << diffuse.x << "," << diffuse.y << "," << diffuse.z << "\n    specular: " << specular.x << "," << specular.y << "," << specular.z << "\n    shininess: " << shininess << std::endl;
        }

        else if (type == "QUAD") {

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
                    // skip if comment: discard characters until newline is found
                    ifs.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                }
                else {
                    for (int i = 0; i < type.length(); ++i) { 
                        ifs.unget();
                    }
                    break;
                }
            }

            // adding quad
            scene_objects.push_back(std::make_shared<Quad>(pos_array[0], pos_array[1], pos_array[2], new Material(diffuse, specular, shininess)));
            std::cout << "adding quad\n    pos_array[0]: " << pos_array[0].x << "," << pos_array[0].y << "," << pos_array[0].z << "\n    pos_array[1]: " << pos_array[1].x << "," << pos_array[1].y << "," << pos_array[1].z << "\n    pos_array[2]: " << pos_array[2].x << "," << pos_array[2].y << "," << pos_array[2].z << "\n    diffuse: " << diffuse.x << "," << diffuse.y << "," << diffuse.z << "\n    specular: " << specular.x << "," << specular.y << "," << specular.z << "\n    shininess: " << shininess << std::endl;

            }

        // if outer comment: discard characters until newline is found
        else if (type == "//" || type.find("//") == 0) {
            ifs.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }

        // dont need to check eof because using io stream
    }

    std::cout << "MAXDEPTH: " << max_depth << "\nWIDTH: " << screen_width << "\nHEIGHT: " << screen_height << "\nBG: " << bg_color.x << "," << bg_color.y << "," << bg_color.z << std::endl;

    // creating Camera
    Camera camera(screen_width, screen_height);

    // Render

    // The pixels are written out in rows with pixels left to right (bottom up).
    std::vector<std::vector<color> > image_data(screen_height, std::vector<color>(screen_width));

    // preparing output ppm file
    std::ofstream ofs(out_file, std::ios::out);
    ofs << "P3\n" << screen_width << ' ' << screen_height << "\n255\n";

    // enable openmp parallel 
    #pragma omp parallel
    // we start drawing from bottom left up 
    for (int j = screen_height - 1; j >= 0; --j) {
        for (int i = 0; i < screen_width; ++i) {
            // shoots primary ray from camera
            Ray primary_ray = camera.get_ray(i, j);
            // recursive get ray color
            color pixel_color = ray_color(scene_objects, scene_lights, primary_ray, max_depth, bg_color, camera);
            // store in image data
            image_data[i][j] = pixel_color;
        }
    }

    // write out image data
    // image coordinate has 0,0 on top
    for (int j = screen_height - 1; j >= 0; --j) {
        std::cerr << "\rScanlines remaining: " << j << ' ' << std::flush;
        for (int i = 0; i < screen_width; ++i) {
            write_color(ofs, image_data[i][j]);
        }
    }

    ofs.close();

    std::cerr << "\nDone.\n";
}

// gets reflected ray from point of intersection
Ray get_reflected_ray(const Ray& ray, const intersection& intersect) {
    // from intersection out (the ray in incoming direction hitting the object)
    glm::dvec3 normalized_normal = glm::normalize(intersect.normal);
    glm::dvec3 r = ray.direction() - 2 * (glm::dot(ray.direction(), normalized_normal)) * normalized_normal;

    // adding 1e-4 offset to prevent self collision again 
    return Ray(intersect.point + r * 1e-4, r);
}

//Ray get_refracted_ray(const Ray& ray, const intersection& intersect) {
//    // send out reflected ray 
//    // from intersection out
//
//    return Ray(intersect.point, glm::normalize(v_parallel - v_perp));
//}

// goes through all lights in scene and return a list of lights contributing to the color of object at intersection
std::vector<Light> send_shadow_rays(const std::vector<Light>& scene_lights, const std::vector<std::shared_ptr<Object>>& scene_objects, const intersection& intersect) {

    std::vector<Light> contributing_lights;

    for (const Light& light : scene_lights) {
        // light to intersection
        glm::dvec3 ray_dir = glm::normalize(intersect.point - light.position);

        bool in_shadow = false;
        Ray shadow_ray = Ray(light.position, ray_dir);

        // get where object at intersection hits the light
        intersection object_intersect;
        intersect.object->get_intersect(shadow_ray, DBL_EPSILON, INFINITY, object_intersect);
        double obj_t = object_intersect.t;

        double closest_so_far = INFINITY;
        // check other objects in scene
        for (const auto& object : scene_objects) {
            // get object's intersection with light 
            // epsilon to prevent it from checking intersection with itself
            if (object->get_intersect(shadow_ray, DBL_EPSILON, closest_so_far, object_intersect)) {
                // if the sceneobj has a smaller t = closer to the light, the object at intersection is in shadow
                if (object_intersect.t < obj_t) {
                    in_shadow = true;
                    // can skip 
                    break;
                }
            }
        }

        if (!in_shadow) {
            // not in shadow: light is contributing 
            contributing_lights.push_back(light);
        }
    }

    return contributing_lights;
}

// check for closest intersection between ray and scene_objects
bool get_objects_intersect(const std::vector<std::shared_ptr<Object>>& scene_objects, const Ray& r, double t_min, double t_max, intersection& intersect) {
    intersection temp_intersect;
    bool hit_anything = false;
    double closest_so_far = t_max;

    // auto so can be any object
    for (const auto& object : scene_objects) {
        if (object->get_intersect(r, t_min, closest_so_far, temp_intersect)) {
            hit_anything = true;
            closest_so_far = temp_intersect.t;
            intersect = temp_intersect;
        }
    }

    return hit_anything;
}

// function that returns the color of the background (a simple gradient)
// depth to prevent infinite loop
color ray_color(const std::vector<std::shared_ptr<Object>>& scene_objects, const std::vector<Light>& scene_lights, const Ray& in_ray, int depth, const color& bg_color, const Camera& camera) {
  
    intersection intersect;

    // using epsilon to prevent self collision
    bool first_intersection = get_objects_intersect(scene_objects, in_ray, DBL_EPSILON, INFINITY, intersect);
    if (!first_intersection) {
        return bg_color;
    }

    // get contributing lights
    std::vector<Light> contributing_lights = send_shadow_rays(scene_lights, scene_objects, intersect);

    // local illumination
    color total_color = color(0.0, 0.0, 0.0);
    // for all lights, accumulate the phone local illumination value
    for (const Light& light : contributing_lights) {
        total_color += get_phong(intersect, light, camera);
    }

    // If we've exceeded the ray bounce limit, no more light is gathered.
    if (depth <= 0) {
        return total_color;
    }

    // add ambient (0.5, 0.5, 0.5)
    total_color *= color(0.5);

    // if material has specular properties, get flected ray
    if (glm::length2(intersect.material->specular) > 0.0) {
        Ray reflected_ray = get_reflected_ray(in_ray, intersect);
        double rind = (double) intersect.material->refraction_index;

        // recursive call to get color of the reflected ray, each time depth is decreased and rind lowers the effect of reflection color
        total_color += (rind * ray_color(scene_objects, scene_lights, reflected_ray, depth - 1, bg_color, camera));
    }
    
    //Ray refracted_ray = get_refracted_ray(in_ray, intersect);
    //total_color += ray_color(scene_objects, scene_lights, refracted_ray, depth-1, bg_color, camera);

    return total_color;
}

// gets color based on phong lighting model
color get_phong(const intersection& intersection, const Light& light, const Camera& camera) {
    // vector point from point to light
    glm::dvec3 l = glm::normalize(light.position - intersection.point);
    // vector pointing to eye/camera
    glm::dvec3 v = glm::normalize(camera.position - intersection.point);
    // vector pointing to the light and normal
    glm::dvec3 r = glm::reflect(-l, intersection.normal);

    double nl = glm::dot(intersection.normal, l);
    double sdot = std::max(0.0, nl);

    color diffuse = intersection.material->diffuse * light.diffuse * sdot;
    color specular = color(0.0);
    if (sdot > 0) {
        specular = pow(
            std::max(0.0, glm::dot(v, r)), intersection.material->shininess)
            * light.specular * intersection.material->specular;
    }

    return diffuse + specular;
}