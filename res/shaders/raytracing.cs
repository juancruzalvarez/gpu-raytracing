#version 430 core

layout(local_size_x = 10, local_size_y = 10, local_size_z = 1) in;
layout(rgba32f, binding = 0) uniform image2D img;

uniform float u_fov;
uniform vec3 u_camera_position;
uniform vec3 u_camera_forwards;
uniform vec3 u_camera_right;
uniform vec3 u_camera_up;



struct Ray {
    vec3 origin;
    vec3 direction;
};

struct Sphere {
    vec3 position;
    float radius;
};

// Calculates a vector, that starts at the origin of the world(0,0,0) and points to the pixel gl_GlobalInvocationID.xy.
// the image is positionated at the world, 1 unit away from the origin, in the negative z direction.
vec3 CalculateRayDirection();

// Returns the smallest t for which the point (ray.origin + t * ray.direction) its also a point of the sphere.
// If the ray and the sphere dont interest, returns -1;
float IntersectSphere(Ray ray, Sphere sphere);



void main() {
    Ray ray = Ray(u_camera_position, CalculateRayDirection());
    Sphere test_sphere = Sphere(vec3(0,0,-5),0.975);
    float t = IntersectSphere(ray, test_sphere);
    if (t != -1)
    {
        vec3 hit_point = ray.origin + ray.direction * t;
        vec3 normal = normalize(hit_point - test_sphere.position);

        imageStore(img, ivec2(gl_GlobalInvocationID.xy), vec4(0.8, 0.3, 0.5, 1)*dot(normal, -ray.direction));

    }
    else
        imageStore(img, ivec2(gl_GlobalInvocationID.xy), vec4(0.1, 0.3, 0.2, 1));

}


vec3 CalculateRayDirection() {
    vec2 img_size = gl_NumWorkGroups.xy * gl_WorkGroupSize.xy;
    float aspect_ratio = img_size.x / img_size.y;
    float plane_width = 2.0f * tan(u_fov / 2.0f) * aspect_ratio;
    float plane_height = 2.0f * tan(u_fov / 2.0f);
    vec2 pixel_coords = gl_GlobalInvocationID.xy + vec2(0.5, 0.5);
    vec2 pixel_relative = pixel_coords / img_size;
    vec2 world_pos = (pixel_relative - 0.5f) * vec2(plane_width, plane_height);
    //return normalize(vec3(world_pos, -1));
    vec3 pixel_world_pos = u_camera_forwards;
    pixel_world_pos += world_pos.x * u_camera_right;
    pixel_world_pos += world_pos.y * u_camera_up;
    return normalize(pixel_world_pos);

}

float IntersectSphere(Ray ray, Sphere sphere) {
    float radius2 = sphere.radius * sphere.radius;
    vec3 L = sphere.position - ray.origin;
    float tca = dot(L, ray.direction);
    // if (tca < 0) return false;
    float d2 = dot(L,L) - tca * tca;
    if (d2 > radius2) return -1;
    float thc = sqrt(radius2 - d2);
    float t0 = tca - thc;
    float t1 = tca + thc;
    if (t0 > t1){
        float tmp = t0;
        t0 = t1;
        t1 = tmp;
    }

    if (t0 < 0)
    {
        t0 = t1; 
        if (t0 < 0) return -1;
    }

    return t0;
}
