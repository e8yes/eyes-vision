#ifndef OBJECT_EXTRACTOR_H
#define OBJECT_EXTRACTOR_H

#include <opencv2/core.hpp>
#include "featureextractor.h"

namespace e8
{

class if_object_extractor
{
public:
        if_object_extractor();
        virtual ~if_object_extractor();

        virtual cv::Mat1b       compute_mask(cv::Mat3f const& img) const = 0;
};

class fgbg_object_extractor: public if_object_extractor
{
public:
        fgbg_object_extractor(cv::Mat3f const& background,
                              unsigned num_components = 1, float threshold = 0.05f, float bg_scale = 1.0f);
        ~fgbg_object_extractor() override;
        cv::Mat1b       compute_mask(cv::Mat3f const& foreground) const;
private:
        cv::Mat3f const&        m_bg;
        unsigned                m_n_comps;
        float                   m_thres;
        float                   m_bg_scale;
};

class generic_object_extractor: public if_object_extractor
{
public:
        generic_object_extractor(cnn_feature_extractor const& fe);
        ~generic_object_extractor();
        cv::Mat1b       compute_mask(cv::Mat3f const& img) const override;
};

}

#endif // OBJECT_EXTRACTOR_H
