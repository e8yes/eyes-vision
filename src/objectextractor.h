#ifndef OBJECT_EXTRACTOR_H
#define OBJECT_EXTRACTOR_H

#include <opencv2/core.hpp>
#include "geometry.h"
#include "extractor.h"
#include "fit.h"
#include "featureextractor.h"

namespace e8
{

typedef if_extractor<if_region*, unsigned char>         if_object_extractor;


class fgbg_object_extractor: public if_object_extractor
{
public:
        fgbg_object_extractor(cv::Mat3f const& background,
                              unsigned num_components = 1, float threshold = 0.05f, float bg_scale = 1.0f);
        ~fgbg_object_extractor() override;
        void*                   extract_intermediate(cv::Mat3f const& x) const override;
        std::vector<if_region*> extract_value(void* intermediate) const override;
        cv::Mat_<unsigned char> extract_mapping(void* intermediate) const override;
private:
        cv::Mat3f const&        m_bg;
        unsigned                m_n_comps;
        float                   m_thres;
        float                   m_bg_scale;
};

class generic_object_extractor: public if_region_fittable
{
public:
        generic_object_extractor(if_feature_fittable& fe);
        ~generic_object_extractor() override;
        float                   fit(cv::Mat3f const& x, std::vector<if_region*> const& y);
        void*                   extract_intermediate(cv::Mat3f const& x) const override;
        std::vector<if_region*> extract_value(void* intermediate) const override;
        cv::Mat_<unsigned char> extract_mapping(void *intermediate) const override;
        void                    import_params(std::istream& data) override;
        void                    export_params(std::ostream& data) const override;
private:
        if_feature_fittable&    m_fe;
        cv::Ptr<cv::ml::SVM>    m_svm;
};

class hog_dpm_object_extractor: public if_region_fittable
{
public:
        hog_dpm_object_extractor();
        ~hog_dpm_object_extractor() override;
        float                   fit(cv::Mat3f const& x, std::vector<if_region*> const& y);
        void*                   extract_intermediate(cv::Mat3f const& x) const override;
        std::vector<if_region*> extract_value(void* intermediate) const override;
        cv::Mat_<unsigned char> extract_mapping(void *intermediate) const override;
        void                    import_params(std::istream& data) override;
        void                    export_params(std::ostream& data) const override;
};

}

#endif // OBJECT_EXTRACTOR_H
