#include "transformation.h"

Eigen::Matrix4f get_scaling_matrix(Eigen::Vector3f scaling) {
    Eigen::Matrix4f matrix = Eigen::Matrix4f::Identity();
   
    matrix << scaling(0),          0,          0, 0,
                       0, scaling(1),          0, 0,
                       0,          0, scaling(2), 0,
                       0,          0,          0, 1;       
    return matrix;
}

Eigen::Matrix4f getx_rotation_matrix(double rotation_angle) {
    Eigen::Matrix4f matrix = Eigen::Matrix4f::Identity();
   
    double angle_x = rotation_angle / 180.0 * PI;
    
    matrix << 1,            0,             0, 0,
              0, cos(angle_x), -sin(angle_x), 0,
              0, sin(angle_x),  cos(angle_x), 0,
              0,            0,             0, 1;
    return matrix;
}

Eigen::Matrix4f gety_rotation_matrix(double rotation_angle) {
    Eigen::Matrix4f matrix = Eigen::Matrix4f::Identity();
   
    double angle_y = rotation_angle / 180.0 * PI;
    
    matrix <<  cos(angle_y), 0, sin(angle_y), 0,
                          0, 1,            0, 0,
              -sin(angle_y), 0, cos(angle_y), 0,
                          0, 0,            0, 1;
                          0, 0,            0, 1;
    return matrix;
}

Eigen::Matrix4f getz_rotation_matrix(double rotation_angle) {
    Eigen::Matrix4f matrix = Eigen::Matrix4f::Identity();

    double angle_z = rotation_angle / 180.0 * PI;
    
    matrix << cos(angle_z), -sin(angle_z), 0, 0,
              sin(angle_z),  cos(angle_z), 0, 0,
                         0,             0, 1, 0,
                         0,             0, 0, 1;
    return matrix;
}

Eigen::Matrix4f get_translation_matrix(Eigen::Vector3f translation) {
    Eigen::Matrix4f matrix = Eigen::Matrix4f::Identity();

    matrix << 1, 0, 0, translation(0),
              0, 1, 0, translation(1),
              0, 0, 1, translation(2),
              0, 0, 0,              1;

    return matrix;
}


Eigen::Matrix4f get_view_matrix(Eigen::Vector3f from_position) {
    Eigen::Matrix4f matrix = Eigen::Matrix4f::Identity();
    
    double po_x = from_position(0);
    double po_y = from_position(1);
    double po_z = from_position(2);
    double po_n = sqrt(po_x * po_x + po_y * po_y + po_z * po_z);
    double po_u = sqrt(po_x * po_x + po_z * po_z);

    matrix <<                po_z / po_u,           0,               -po_x / po_u,     0,
              -po_y * po_x / po_n / po_u, po_u / po_n, -po_y * po_z / po_n / po_u,     0,
                             po_x / po_n, po_y / po_n,                po_z / po_n, -po_n,
                                       0,           0,                          0,     1;

    return matrix;
}

Eigen::Matrix4f get_projection_matrix(float fov, float aspect, float near, float far) {
    Eigen::Matrix4f matrix = Eigen::Matrix4f::Identity();
    fov = fov / 180 * PI;
    matrix << 1 / tan(fov / 2),                     0,                            0,                              0,
                             0, aspect / tan(fov / 2),                            0,                              0,
                             0,                     0, -(far + near) / (far - near), -2 * far * near / (far - near),
                             0,                     0,                           -1,                              0;

    return matrix;
}