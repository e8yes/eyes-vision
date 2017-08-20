#include "neuralnet.h"
#include "featureextractor.h"


e8::cnn_feature_extractor::cnn_feature_extractor(unsigned image_size):
        m_img_size(image_size)
{
}

e8::cnn_feature_extractor::~cnn_feature_extractor()
{
}

float
e8::cnn_feature_extractor::fit(cv::Mat3f const& x, std::vector<float> const& y)
{
}

void*
e8::cnn_feature_extractor::extract_intermediate(cv::Mat3f const& input) const
{
}

std::vector<float>
e8::cnn_feature_extractor::extract_value(void *intermediate) const
{
}

cv::Mat_<float>
e8::cnn_feature_extractor::extract_mapping(void* intermediate) const
{
}

void
e8::cnn_feature_extractor::import_params(std::istream& data)
{
}

void
e8::cnn_feature_extractor::export_params(std::ostream& data) const
{
}
