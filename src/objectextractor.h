#ifndef OBJECT_EXTRACTOR_H
#define OBJECT_EXTRACTOR_H

#include <opencv2/core.hpp>

namespace e8
{

class if_object_extractor
{
public:
        if_object_extractor();
        virtual ~if_object_extractor();

        virtual cv::Mat1b       compute_mask() const = 0;
};

class fgbg_extractor: public if_object_extractor
{
public:
        fgbg_extractor(cv::Mat3f const& foreground, cv::Mat3f const& background, unsigned num_components = 1, float threshold = 0.05f, float bg_scale = 1.0f);
        ~fgbg_extractor() override;

        cv::Mat1b       compute_mask() const;
private:
        cv::Mat3f const&        m_fg;
        cv::Mat3f const&        m_bg;
        unsigned                m_n_comps;
        float                   m_thres;
        float                   m_bg_scale;
};

}

#endif // OBJECT_EXTRACTOR_H
