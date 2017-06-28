#ifndef SCANNER_H
#define SCANNER_H

#include <string>
#include <vector>
#include "camera.h"
#include "geometry.h"

namespace e8
{

class if_scanner
{
public:
        if_scanner();
        virtual ~if_scanner();

        virtual point_cloud     scan(std::vector<camera> const& cams) const = 0;
};

class struct_light_scanner: if_scanner
{
public:
        struct_light_scanner(std::string const& path, std::string const& suffix, unsigned num_bits);
        ~struct_light_scanner();

        point_cloud     scan(std::vector<camera> const& cams) const override;
private:
        unsigned                        m_num_bits;
        std::vector<std::string>        m_left_hori;
        std::vector<std::string>        m_left_vert;
        std::vector<std::string>        m_right_hori;
        std::vector<std::string>        m_right_vert;
};

}

#endif // SCANNER_H
