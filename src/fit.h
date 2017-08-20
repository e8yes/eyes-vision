#ifndef FIT_H
#define FIT_H

#include <opencv2/opencv.hpp>
#include "extractor.h"
#include "imagesamples.h"

namespace e8
{

template<class T, class M = void*>
class if_fittable: public if_extractor<T, M>
{
public:
        if_fittable() {}
        virtual ~if_fittable() {}
        virtual float           fit(cv::Mat3f const& x, std::vector<T> const& y) = 0;
        virtual void            import_params(std::istream& data) = 0;
        virtual void            export_params(std::ostream& data) const = 0;
};

typedef if_fittable<float, float>               if_feature_fittable;
typedef if_fittable<if_region*, unsigned char>  if_region_fittable;

class if_fitter
{
public:
        if_fitter();
        virtual ~if_fitter();
        virtual float                   fit() = 0;
        virtual void                    backup() = 0;
        virtual void                    recover() = 0;
        virtual float                   error() const = 0;
};

class annotated_region_fitter: public if_fitter
{
public:
        annotated_region_fitter(e8util::image_samples const& samples,
                                if_region_fittable& fittable,
                                std::string const& param_file,
                                std::string const& log_file);
        float                           fit() override;
        void                            backup() override;
        void                            recover() override;
        float                           error() const override;
private:
        e8util::image_samples const&    m_samples;
        if_region_fittable&             m_fittable;
        std::string                     m_param_file;
        std::string                     m_log_file;
};

}

#endif // FIT_H
