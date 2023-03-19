#version 430 core
#define EPSILON 1e-5


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

struct Plane {
    vec3 corner;
    vec3 normal;
    vec3 s1, s2;
};

struct Material {
    vec3 color;
};

struct HitInfo {
    vec3 hit_point;
    vec3 normal;
    Material material;
};

// Calculates a vector, that starts at the origin of the world(0,0,0) and points to the pixel gl_GlobalInvocationID.xy.
// the image is positionated at the world, 1 unit away from the origin, in the negative z direction.
vec3 CalculateRayDirection();

// Returns the smallest t for which the point (ray.origin + t * ray.direction) its also a point of the sphere.
// If the ray and the sphere dont interest, returns -1;
float IntersectSphere(Ray ray, Sphere sphere, out vec3 hit_point, out vec3 normal);

// Returns the smallest t for which the point (ray.origin + t * ray.direction) its also a point of the sphere.
// If the ray and the plane dont interest, returns -1;
float IntersectPlane(Ray ray, Plane plane, out vec3 hit_point, out vec3 normal);



void main() {
    Ray ray = Ray(u_camera_position, CalculateRayDirection());
    Sphere test_sphere = Sphere(vec3(0,0,-5),0.975);
    Plane p = Plane(vec3(0,0,-7), vec3(0,0,-1),vec3(0),vec3(0));
    vec3 normal, hit_point, color;

    vec3 sNormal, sHit_point;
    float tS = IntersectSphere(ray, test_sphere, sHit_point, sNormal);
    vec3 pNormal, pHit_point;
    float tP = IntersectPlane(ray, p,pHit_point, pNormal);
    float t = -1;
    if(tS > 0 && tP > 0) {
        t = tS>tP ? tP : tS;
        if (t == tS)
        {
            color = vec3(1, 0, 0);
            normal = sNormal;
            hit_point = sHit_point;
        }
        else
        {
            color = vec3(0, 1, 0);
            normal = pNormal;
            hit_point = pHit_point;
        }
    }else if(tS>0) {
        t = tS;
        color = vec3(1, 0, 0);
        normal = sNormal;
        hit_point = sHit_point;
    }else if(tP>0) {
        t = tP;
        color = vec3(0, 1, 0);
        normal = pNormal;
        hit_point= pHit_point;
    }

    if (t != -1)
    {
        imageStore(img, ivec2(gl_GlobalInvocationID.xy), vec4(color,1));

    }
    else
        imageStore(img, ivec2(gl_GlobalInvocationID.xy), vec4(0.1, 0.1, 0.1, 1));

}


vec3 CalculateRayDirection() {
    vec2 img_size = gl_NumWorkGroups.xy * gl_WorkGroupSize.xy;
    float aspect_ratio = img_size.x / img_size.y;
    float tan_u_fov = 2.0f * tan(u_fov / 2.0f);
    float plane_width =  tan_u_fov * aspect_ratio;
    float plane_height = tan_u_fov;
    vec2 pixel_coords = gl_GlobalInvocationID.xy + vec2(0.5, 0.5);
    vec2 pixel_relative = pixel_coords / img_size;
    vec2 world_pos = (pixel_relative - 0.5f) * vec2(plane_width, plane_height);
    //return normalize(vec3(world_pos, -1));
    vec3 pixel_world_pos = u_camera_forwards;
    pixel_world_pos += world_pos.x * u_camera_right;
    pixel_world_pos += world_pos.y * u_camera_up;
    return normalize(pixel_world_pos);

}

float IntersectSphere(Ray ray, Sphere sphere, out vec3 hit_point, out vec3 normal) {
    float radius2 = sphere.radius * sphere.radius;
    vec3 L = sphere.position - ray.origin;
    float tca = dot(L, ray.direction);
    // if (tca < 0) return false;
    float d2 = dot(L,L) - tca * tca;
    if (d2 > radius2) return -1;
    float thc = sqrt(radius2 - d2);
    float t0 = tca - thc;
    float t1 = tca + thc;
    if (t0 > t1) {
        float tmp = t0;
        t0 = t1;
        t1 = tmp;
    }

    if (t0 < 0) {
        t0 = t1; 
        if (t0 < 0) return -1;
    }

    hit_point = ray.origin + ray.direction * t0;
    //divide by sphere.radius to normalize the normal,.
    normal = (hit_point - sphere.position) / sphere.radius;
    return t0;
}
float IntersectPlane(Ray ray, Plane plane, out vec3 hit_point, out vec3 normal) {
    float denom = dot(ray.direction, plane.normal);
    if(denom > EPSILON) {
        vec3 p0r0 = plane.corner - ray.origin;
        float t = dot(p0r0, plane.normal);
        if(t>0) {
            hit_point = ray.origin + t * ray.direction;
            normal = plane.normal;
            return t;
        }
    }

    return -1;
}
