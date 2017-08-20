#include <algorithm>
#include <opencv2/imgproc.hpp>
#include "morphology.h"
#include "objectextractor.h"


e8::fgbg_object_extractor::fgbg_object_extractor(cv::Mat3f const& background, unsigned num_comps, float threshold, float bg_scale):
        m_bg(background), m_n_comps(num_comps), m_thres(threshold), m_bg_scale(bg_scale)
{
}

e8::fgbg_object_extractor::~fgbg_object_extractor()
{
}

void*
e8::fgbg_object_extractor::extract_intermediate(cv::Mat3f const& foreground) const
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
        cv::Mat1b* suppressed = new cv::Mat1b(filtered.size());
        suppressed->zeros(filtered.size());
        for (unsigned i = 0; i < m_n_comps && i < islands.size(); i ++) {
                e8util::island const& island = islands[i];
                for (unsigned j = 0; j < island.pixel_locs.size(); j ++) {
                        unsigned row = island.pixel_locs[j].x;
                        unsigned col = island.pixel_locs[j].y;
                        suppressed->at<unsigned char>(row, col) = 0xFF;
                }
        }
        return static_cast<void*>(suppressed);
}

std::vector<e8::if_region*>
e8::fgbg_object_extractor::extract_value(void* intermediate) const
{
}

cv::Mat_<unsigned char>
e8::fgbg_object_extractor::extract_mapping(void* intermediate) const
{
        return *static_cast<cv::Mat_<unsigned char>*>(intermediate);
}




e8::generic_object_extractor::generic_object_extractor(if_feature_fittable& fe):
        m_fe(fe)
{
        m_svm = cv::ml::SVM::create();
        m_svm->setType(cv::ml::SVM::C_SVC);
        m_svm->setKernel(cv::ml::SVM::POLY);
        m_svm->setDegree(2);
}

e8::generic_object_extractor::~generic_object_extractor()
{
}

float
e8::generic_object_extractor::fit(cv::Mat3f const& x, std::vector<if_region*> const& y)
{
}

void*
e8::generic_object_extractor::extract_intermediate(cv::Mat3f const& x) const
{
}

std::vector<e8::if_region*>
e8::generic_object_extractor::extract_value(void* intermediate) const
{
}

cv::Mat_<unsigned char>
e8::generic_object_extractor::extract_mapping(void *intermediate) const
{
}

void
e8::generic_object_extractor::import_params(std::istream& data)
{
}

void
e8::generic_object_extractor::export_params(std::ostream& data) const
{
}




e8::hog_dpm_object_extractor::hog_dpm_object_extractor()
{
}

e8::hog_dpm_object_extractor::~hog_dpm_object_extractor()
{
}

float
e8::hog_dpm_object_extractor::fit(cv::Mat3f const& x, std::vector<if_region*> const& y)
{
}

void*
e8::hog_dpm_object_extractor::extract_intermediate(cv::Mat3f const& x) const
{
}

std::vector<e8::if_region*>
e8::hog_dpm_object_extractor::extract_value(void* intermediate) const
{
}

cv::Mat_<unsigned char>
e8::hog_dpm_object_extractor::extract_mapping(void *intermediate) const
{
}

void
e8::hog_dpm_object_extractor::import_params(std::istream& data)
{
}

void
e8::hog_dpm_object_extractor::export_params(std::ostream& data) const
{
}
