#ifndef GEOMETRY_H
#define GEOMETRY_H

#include <vector>
#include <opencv2/core.hpp>

namespace e8
{

template<class T>
struct corr
{
        corr(T const& a, T const& b):
                a(a), b(b)
        {
        }

        T       a;
        T       b;
};

typedef corr<cv::Vec2i>         img_corr;
typedef corr<cv::Vec3f>         point_corr;

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

void            point_cloud_write(std::string const& path, point_cloud const& ps);
void            mesh_write(std::string const& path, mesh const& m);

cv::Matx33f     rotation_xyz_transform(float x, float y, float z);

}

#endif // GEOMETRY_H
