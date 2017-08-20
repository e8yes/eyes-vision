#ifndef EXTRACTOR_H
#define EXTRACTOR_H

#include <vector>
#include <opencv2/core.hpp>

namespace e8
{

template<class T, class M>
class if_extractor
{
public:
        if_extractor() {}
        virtual ~if_extractor() {}
        virtual void*           extract_intermediate(cv::Mat3f const& x) const = 0;
        virtual std::vector<T>  extract_value(void* intermediate) const = 0;
        virtual cv::Mat_<M>     extract_mapping(void* intermediate) const = 0;
};

}

#endif // EXTRACTOR_H
