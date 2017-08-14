#include <algorithm>
#include <opencv2/imgproc.hpp>
#include "morphology.h"
#include "objectextractor.h"


e8::if_object_extractor::if_object_extractor()
{
}

e8::if_object_extractor::~if_object_extractor()
{
}


e8::fgbg_object_extractor::fgbg_object_extractor(cv::Mat3f const& background, unsigned num_comps, float threshold, float bg_scale):
        m_bg(background), m_n_comps(num_comps), m_thres(threshold), m_bg_scale(bg_scale)
{
}

e8::fgbg_object_extractor::~fgbg_object_extractor()
{
}

cv::Mat1b
e8::fgbg_object_extractor::compute_mask(cv::Mat3f const& foreground) const
{
        // subtract out the background.
        cv::Mat3f extracted = cv::abs(foreground - m_bg*m_bg_scale);

        // mask generation.
        cv::Mat1b mask(extracted.size());
        float threshold = m_thres;
        extracted.forEach([&mask, threshold](cv::Vec3f& px, int const* p) {
                mask.at<unsigned char>(p[0], p[1]) = px.dot(px) < threshold*threshold ? 0 : 0xFF;
        });

        // noise removal.
        unsigned fsize = (foreground.size().width + foreground.size().height)/2.0f*0.01f;
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

e8::box
e8::fgbg_object_extractor::compute_box(cv::Mat3f const& img) const
{
}

e8::ellipse
e8::fgbg_object_extractor::compute_ellipse(cv::Mat3f const& img) const
{
}


e8::generic_object_extractor::generic_object_extractor(cnn_feature_extractor const& fe)
{
}

e8::generic_object_extractor::~generic_object_extractor()
{
}

cv::Mat1b
e8::generic_object_extractor::compute_mask(cv::Mat3f const& img) const
{
}

e8::box
e8::generic_object_extractor::compute_box(cv::Mat3f const& img) const
{
}

e8::ellipse
e8::generic_object_extractor::compute_ellipse(cv::Mat3f const& img) const
{
}


e8::hog_dpm_object_extractor::hog_dpm_object_extractor()
{
}

e8::hog_dpm_object_extractor::~hog_dpm_object_extractor()
{
}

cv::Mat1b
e8::hog_dpm_object_extractor::compute_mask(cv::Mat3f const& img) const
{
}

e8::box
e8::hog_dpm_object_extractor::compute_box(cv::Mat3f const& img) const
{
}

e8::ellipse
e8::hog_dpm_object_extractor::compute_ellipse(cv::Mat3f const& img) const
{
}
