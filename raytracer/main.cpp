#include "utility.h"

#include "color.h"
#include "objectList.h"
#include "shape.h"
#include "camera.h"
#include "material.h"
#include "light.h"
#include <fstream>      // std::ofstream
#include <string>      // std::ofstream

color ray_color(std::vector<shared_ptr<Object>>& scene_objects, const std::vector<Light>& scene_lights, const Ray& ray, int depth, const color& bg_color, const Camera& camera);

color ambient_light = color(0.5);

color getDiffuseReflection(const intersection& intersection, const Light light) {
    // c = diffuse * intensity * max (0, n dot l) to not produce negative
    // get intersection to light direction
    glm::dvec3 light_dir = glm::normalize(light.position - intersection.point);
    double nl = glm::dot(intersection.normal, light_dir);

    return intersection.material->diffuse * light.diffuse * std::max(0.0, nl);
}

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
    int max_depth = 50;
    // default bg to black
    color bg_color = color(0);

    //ObjectList scene_objects;    
    std::vector<shared_ptr<Object> > scene_objects;
    std::vector<Light> scene_lights;

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
            // for specular
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
            Light light = Light(pos, diffuse, specular);
            scene_lights.push_back(light);
            std::cout << "adding light\n    pos: " << pos.x << "," << pos.y << "," << pos.z <<  "\n    diffuse: " << diffuse.x << "," << diffuse.y << "," << diffuse.z << "\n    specular: " << specular.x << "," << specular.y << "," << specular.z << std::endl;

        }
        else if (type == "SPHERE") {
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
            scene_objects.push_back(std::make_shared<Sphere>(pos, radius, &material));
            std::cout << "adding sphere\n    pos: " << pos.x << "," << pos.y << "," << pos.z  << "\n    radius: " << radius << "\n    diffuse: " << diffuse.x << "," << diffuse.y << "," << diffuse.z << "\n    specular: " << specular.x << "," << specular.y << "," << specular.z << "\n    shininess: " << shininess << std::endl;
        }
        else if (type == "QUAD") {
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
            scene_objects.push_back(std::make_shared<Quad>(pos_array[0], pos_array[1], pos_array[2], &material));
            std::cout << "adding quad\n    pos_array[0]: " << pos_array[0].x << "," << pos_array[0].y << "," << pos_array[0].z << "\n    pos_array[1]: " << pos_array[1].x << "," << pos_array[1].y << "," << pos_array[1].z << "\n    pos_array[2]: " << pos_array[2].x << "," << pos_array[2].y << "," << pos_array[2].z << "\n    diffuse: " << diffuse.x << "," << diffuse.y << "," << diffuse.z << "\n    specular: " << specular.x << "," << specular.y << "," << specular.z << "\n    shininess: " << shininess << std::endl;

        }
        else if (type == "//" || type.find("//") == 0) {
            // skip if comment
            ifs.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            //discard characters until newline is found
        }
        // dont need eof because io stream
    }
   
    std::cout << "MAXDEPTH: " << max_depth << " WIDTH: " << screen_width << " HEIGHT: " << screen_height << " BG: " << bg_color.x << "," << bg_color.y << "," << bg_color.z << std::endl;

    // Camera
    Camera camera(screen_width, screen_height);
  
    // Render

    //The pixels are written out in rows with pixels left to right.
    //The rows are written out from top to bottom.
    //By convention, each of the red / green / blue components range from 0.0 to 1.0.
    
    /*writing into file image.ppm*/
    std::ofstream ofs(out_file, std::ios::out);
    ofs << "P3\n" << screen_width << ' ' << screen_height << "\n255\n";
   
    // we start drawing from bottom left up 
    for (int j = screen_height - 1; j >= 0 ; --j) {
        std::cerr << "\rScanlines remaining: " << j << ' ' << std::flush;
        color pixel_color(0, 0, 0);
        for (int i = 0; i < screen_width; ++i) {
            Ray primary_ray = camera.get_ray(i, j);
            pixel_color = ray_color(scene_objects, scene_lights, primary_ray, max_depth, bg_color, camera);
            // image coordinate has 0,0 on top
            write_color(ofs, pixel_color);
        }
        //break;
    }
    ofs.close();

    std::cerr << "\nDone.\n";
}

Ray get_reflected_ray(const Ray& ray, const intersection& intersect) {
    // send out reflected ray 
    // from intersection out (the ray in incoming direction hitting the object)
    glm::dvec3 v = -ray.direction();
    glm::dvec3 normalized_normal = glm::normalize(intersect.normal);
    glm::dvec3 v_parallel = normalized_normal * glm::dot(normalized_normal, v);
    glm::dvec3 v_perp = v - v_parallel;
    return Ray(intersect.point, glm::normalize(v_parallel - v_perp));
}

//Ray get_refracted_ray(const Ray& ray, const intersection& intersect) {
//    // send out reflected ray 
//    // from intersection out
//
//    return Ray(intersect.point, glm::normalize(v_parallel - v_perp));
//}

bool getObjectIntersect(const std::vector<shared_ptr<Object>>& scene_objects, const Ray& r, double t_min, double t_max, intersection& intersect) {
    intersection temp_intersect;
    bool hit_anything = false;
    double closest_so_far = t_max;

    // auto so can be any object
    for (const auto& object : scene_objects) {
        if (object->getIntersect(r, t_min, closest_so_far, temp_intersect)) {
            hit_anything = true;
            closest_so_far = temp_intersect.t;
            intersect = temp_intersect;
        }
    }

    return hit_anything;
}


std::vector<Light> send_shadow_rays(const std::vector<Light>& scene_lights, const std::vector<shared_ptr<Object>>& scene_objects, const intersection& intersect) {
    // go through all lights, if intersect, then true 
    std::vector<Light> contributing_lights;
    // returns contributing lights
    for (const Light& light : scene_lights) {
        //std::cout << "getting phong" << std::endl;
        // ray from light to intersection point :  glm::normalize(intersect.point - light.position);
        // ... or intersection to light? I guess doesn't matter
        //
        // light to intersection
        glm::dvec3 ray_dir = glm::normalize(intersect.point - light.position);
        bool in_shadow = false;
        Ray shadow_ray = Ray(light.position, ray_dir);

        // need to get t for where light is!!!!
        intersection object_intersect;
        intersect.object->getIntersect(shadow_ray, epsilon, infinity, object_intersect);
        double obj_t = object_intersect.t;

        double closest_so_far = infinity;
        for (const auto& object : scene_objects) {
            // get object's intersction with light 
            if (object->getIntersect(shadow_ray, epsilon, closest_so_far, object_intersect)) {
                // point where light hits object
                if (object_intersect.t < obj_t) {
                    // this object is closer to light than the point of itnersection's - > in shadow
                    // also accounts for not being the same point
                    in_shadow = true;
                    // can skip 
                    break;
                }
                else {
                    // this object is behind the object of intersection
                }
            }
        }

        if (!in_shadow) {
            // no hit -> light is contributing 
            contributing_lights.push_back(light);
        }
    }

    return contributing_lights;
}

// function that returns the color of the background (a simple gradient)
// depth to prevent infinite loop
color ray_color(std::vector<shared_ptr<Object>>& scene_objects, const std::vector<Light>& scene_lights, const Ray& in_ray, int depth, const color& bg_color, const Camera& camera) {
    intersection intersect;

    // to do use episilon? 
    bool first_intersection = getObjectIntersect(scene_objects, in_ray, epsilon, infinity, intersect);
    if (!first_intersection) {
        return bg_color;
    }

    std::vector<Light> contributing_lights = send_shadow_rays(scene_lights, scene_objects, intersect);

    // local illumination
    color total_color = color(0.0, 0.0, 0.0);
    // for all lights
    for (const Light& light : contributing_lights) {
        total_color += get_phong(intersect, light, camera);
        //print_vec3("co: ", total_color);
    }
    // add ambient 
    total_color *= ambient_light;

    // If we've exceeded the ray bounce limit, no more light is gathered.
    if (depth <= 0) {
        return total_color;
    }
        
    //Ray reflected_ray = get_reflected_ray(in_ray, intersect);
    //Ray refracted_ray = get_refracted_ray(in_ray, intersect);

    //total_color += ray_color(scene_objects, scene_lights, reflected_ray, depth-1, bg_color, camera);
    //total_color += ray_color(scene_objects, scene_lights, refracted_ray, depth-1, bg_color, camera);

    return total_color;
}
