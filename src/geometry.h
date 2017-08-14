#ifndef GEOMETRY_H
#define GEOMETRY_H

#include <vector>
#include <opencv2/core.hpp>

namespace e8
{

template<class T>
struct corr
{
        corr(T const& a, T const& b);
        T       a;
        T       b;
};

template<class T>
corr<T>::corr(T const& a, T const& b):
        a(a), b(b)
{
}

typedef corr<cv::Vec2i>         img_corr;
typedef corr<cv::Vec3f>         point_corr;


////////////////////////////////////////////////
struct vertex
{
        vertex(cv::Vec3f const& p, cv::Vec3f const& n);
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


//////////////////////////////////////////////////////////
cv::Matx33f     rotation_xyz_transform(float x, float y, float z);


///////////////////////////////////////////////////////////
struct box
{
        box(cv::Vec2f const& min, cv::Vec2f const& max);
        bool is_inside(cv::Vec2f const& pt) const;

        cv::Vec2f       min;
        cv::Vec2f       max;
};

struct ellipse
{
        ellipse(float major_axis, float minor_axis, cv::Vec2f const& c, float rot);
        bool    is_inside(cv::Vec2f const& p) const;

        float           major_axis;
        float           minor_axis;
        cv::Matx33f     t_f;
        cv::Matx33f     t_i;
};

}

#endif // GEOMETRY_H
