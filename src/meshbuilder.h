#ifndef IF_MESH_BUILDER_H
#define IF_MESH_BUILDER_H

#include "geometry.h"

namespace e8
{

class if_mesh_builder
{
public:
        if_mesh_builder();

        virtual mesh    build_mesh() const = 0;
};

class poisson_mesh_builder: public if_mesh_builder
{
public:
        poisson_mesh_builder();

        mesh    build_mesh() const;
};

}


#endif // IF_MESH_BUILDER_H
