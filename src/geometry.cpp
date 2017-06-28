#include <fstream>
#include "geometry.h"


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
