#include <algorithm>
#include <opencv2/imgproc.hpp>
#include "util.h"
#include "objectextractor.h"


e8::if_object_extractor::if_object_extractor()
{
}

e8::if_object_extractor::~if_object_extractor()
{
}


e8::fgbg_extractor::fgbg_extractor(cv::Mat3f const& foreground, cv::Mat3f const& background, unsigned num_comps, float threshold):
        m_fg(foreground), m_bg(background), m_n_comps(num_comps), m_thres(threshold)
{
}

e8::fgbg_extractor::~fgbg_extractor()
{
}

cv::Mat1b
e8::fgbg_extractor::compute_mask() const
{
        // subtract out the background.
        cv::Mat3f extracted = cv::abs(m_fg - m_bg);

        // mask generation.
        cv::Mat1b mask(extracted.size());
        float threshold = m_thres;
        extracted.forEach([&mask, threshold](cv::Vec3f& px, int const* p) {
                mask.at<unsigned char>(p[0], p[1]) = px.dot(px) < threshold*threshold ? 0 : 0xFF;
        });

        // noise removal.
        unsigned fsize = (m_fg.size().width + m_fg.size().height)/2.0f*0.01f;
        fsize = fsize + 1 - (fsize % 2);
        cv::Mat1b filtered(mask.size());
        cv::medianBlur(mask, filtered, fsize);

        // non-maximal area suppression.
        cv::Mat1b id_map;
        std::vector<e8util::island> const& islands = e8util::islands(filtered, id_map);
        cv::Mat1b suppressed = cv::Mat1b::zeros(filtered.size());
        for (unsigned i = 0; i < m_n_comps && i < islands.size(); i ++) {
                e8util::island const& island = islands[i];
                for (unsigned j = 0; j < island.pixel_locs.size(); j ++) {
                        unsigned row = island.pixel_locs[j].x;
                        unsigned col = island.pixel_locs[j].y;
                        suppressed.at<unsigned char>(row, col) = 0xFF;
                }
        }
        return suppressed;
}
