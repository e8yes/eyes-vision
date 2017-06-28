#ifndef GEOMETRY_H
#define GEOMETRY_H

#include <vector>
#include <opencv2/core.hpp>

namespace e8
{

struct img_corr
{
        img_corr(cv::Vec2i const& a, cv::Vec2i const& b):
                a(a), b(b)
        {
        }

        cv::Vec2i       a;
        cv::Vec2i       b;
};

struct vertex
{
        vertex(cv::Vec3f const& p, cv::Vec3f const& n):
                p(p), n(n)
        {
        }

        cv::Vec3f       p;
        cv::Vec3f       n;
};

typedef std::vector<vertex>     point_cloud;

struct mesh
{
        point_cloud             vs;
        std::vector<unsigned>   is;
};

void    point_cloud_write(std::string const& path, point_cloud const& ps);
void    mesh_write(std::string const& path, mesh const& m);

}

#endif // GEOMETRY_H
