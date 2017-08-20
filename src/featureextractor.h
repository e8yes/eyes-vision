#ifndef FEATUREEXTRACTOR_H
#define FEATUREEXTRACTOR_H

#include <vector>
#include <iostream>
#include "fit.h"
#include "imagesamples.h"
#include "neuralnet.h"

namespace e8
{

class cnn_feature_extractor: public if_feature_fittable
{
public:
        cnn_feature_extractor(unsigned image_size = 227);
        ~cnn_feature_extractor() override;
        float                   fit(cv::Mat3f const& x, std::vector<float> const& y) override;
        void*                   extract_intermediate(cv::Mat3f const& input) const override;
        std::vector<float>      extract_value(void *intermediate) const override;
        cv::Mat_<float>         extract_mapping(void* intermediate) const override;
        void                    import_params(std::istream& data) override;
        void                    export_params(std::ostream& data) const override;
private:
        unsigned                m_img_size;
        /*e8util::nn_layer*       m_conn;
        e8util::nn_layer*       m_full;
        e8util::nn_layer*       m_output;*/
};

}

#endif // FEATUREEXTRACTOR_H
