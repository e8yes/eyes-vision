#include "neuralnet.h"
#include "featureextractor.h"


e8::if_blind_feature_extractor::if_blind_feature_extractor()
{
}

e8::if_blind_feature_extractor::~if_blind_feature_extractor()
{
}



e8::cnn_feature_extractor::cnn_feature_extractor()
{
}

e8::cnn_feature_extractor::~cnn_feature_extractor()
{
}

float
e8::cnn_feature_extractor::train()
{
}

std::vector<float>
e8::cnn_feature_extractor::extract() const
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
