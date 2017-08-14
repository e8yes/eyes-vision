#include <cmath>
#include <fstream>
#include "geometry.h"


e8::vertex::vertex(cv::Vec3f const& p, cv::Vec3f const& n):
        p(p), n(n)
{
}

void
e8::point_cloud_write(std::string const& path, point_cloud const& ps)
{
        std::ofstream file(path);
        file << "ply" << std::endl;
        file << "format ascii 1.0" << std::endl;
        file << "comment Created by e8yesvision" << std::endl;
        file << "element vertex " << ps.size() << std::endl;
        file << "property float x" << std::endl;
        file << "property float y" << std::endl;
        file << "property float z" << std::endl;
        file << "property float nx" << std::endl;
        file << "property float ny" << std::endl;
        file << "property float nz" << std::endl;
        file << "end_header" << std::endl;
        for (unsigned i = 0; i < ps.size(); i ++) {
                file << ps[i].p[0] << ' ' << ps[i].p[1] << ' ' << ps[i].p[2] << ' ';
                file << ps[i].n[0] << ' ' << ps[i].n[1] << ' ' << ps[i].n[2] << std::endl;
        }
        file.close();
}

void
e8::mesh_write(std::string const& path, mesh const& m)
{
}


cv::Matx33f
e8::rotation_xyz_transform(float x, float y, float z)
{
        cv::Matx33f rx(1, 0,            0,
                       0, std::cos(x),  -std::sin(x),
                       0, std::sin(x),  std::cos(x));
        cv::Matx33f ry(std::cos(y),     0, std::sin(y),
                       0,               1, 0,
                       -std::sin(y),    0, std::cos(y));
        cv::Matx33f rz(std::cos(z),     -std::sin(z),   0,
                       std::sin(z),     std::cos(z),    0,
                       0,               0,              1);

        return rz*ry*rx;
}

e8::box::box(cv::Vec2f const& min, cv::Vec2f const& max):
        min(min), max(max)
{
}

bool
e8::box::is_inside(cv::Vec2f const& pt) const
{
        return pt[0] >= min[0] && pt[0] <= max[0] &&
               pt[1] >= min[1] && pt[1] <= max[1];
}

e8::ellipse::ellipse(float major_axis, float minor_axis, cv::Vec2f const& c, float rot):
        major_axis(major_axis), minor_axis(minor_axis)
{
        cv::Matx33f t = cv::Matx33f(1, 0, c[0],
                                    0, 1, c[1],
                                    0, 0, 1);
        cv::Matx33f inv_t = cv::Matx33f(1, 0, -c[0],
                                        0, 1, -c[1],
                                        0, 0, 1);
        cv::Matx33f r = rotation_xyz_transform(0, 0, rot);
        t_f = t*r;
        t_i = r.t()*inv_t;
}

bool
e8::ellipse::is_inside(cv::Vec2f const& p) const
{
        cv::Vec3f const& lp = t_i*cv::Vec3f(p[0], p[1], 1);
        return lp[0]*lp[0]/(major_axis*major_axis) + lp[1]*lp[1]/(minor_axis*minor_axis) <= 1;
}
