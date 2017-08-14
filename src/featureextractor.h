#ifndef FEATUREEXTRACTOR_H
#define FEATUREEXTRACTOR_H

#include <vector>
#include <iostream>

namespace e8
{

class if_blind_feature_extractor
{
public:
        if_blind_feature_extractor();
        virtual ~if_blind_feature_extractor();
        virtual float                   train() = 0;
        virtual std::vector<float>      extract() const = 0;
        virtual void                    import_params(std::istream& data) = 0;
        virtual void                    export_params(std::ostream& data) const = 0;
};

class cnn_feature_extractor: public if_blind_feature_extractor
{
public:
        cnn_feature_extractor();
        ~cnn_feature_extractor() override;
        float                   train() override;
        std::vector<float>      extract() const override;
        void                    import_params(std::istream& data) override;
        void                    export_params(std::ostream& data) const override;
};

}

#endif // FEATUREEXTRACTOR_H
