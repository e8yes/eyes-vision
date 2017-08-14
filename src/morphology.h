#ifndef MORPHOLOGY_H
#define MORPHOLOGY_H

#include <vector>
#include <opencv2/core.hpp>

namespace e8util
{

struct island
{
        island(unsigned id);

        unsigned char                   id;
        std::vector<cv::Point2i>        pixel_locs;
        unsigned                        area;
};

std::vector<island>     islands(cv::Mat1b const& bw, cv::Mat1b& id_map, unsigned target = 0xFF);

}

#endif // MORPHOLOGY_H
