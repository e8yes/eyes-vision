#ifndef IMAGE_SAMPLES_H
#define IMAGE_SAMPLES_H

#include <map>
#include <vector>
#include <string>
#include "geometry.h"


namespace e8util
{

typedef std::map<unsigned, std::string>         id_name_t;
typedef std::pair<unsigned, std::string>        id_name_pair_t;


struct image_annotation
{
        image_annotation(e8::if_region const& region, unsigned id);
        e8::if_region const&    region;
        unsigned                id;
};

class image_samples
{
public:
        image_samples();
        virtual ~image_samples();

        class iterator
        {
        public:
                virtual std::vector<image_annotation>   annotations() const = 0;
                virtual cv::Mat3f                       image() const = 0;
        };

        virtual id_name_t       id2name() const = 0;
        virtual unsigned        sample_count() const = 0;
        virtual iterator const& get(unsigned i) const = 0;
};

class fddb_samples: public image_samples
{
public:
        fddb_samples(std::string const& annotations, std::string const& imgs, std::string const& normalized);
        fddb_samples(std::string const& annotations, std::string const& normalized);
        ~fddb_samples() override;

        class fddb_iterator: public image_samples::iterator
        {
        public:
                fddb_iterator(std::vector<e8::ellipse> const& regions, std::string const& img_path);
                std::vector<image_annotation>   annotations() const override;
                cv::Mat3f                       image() const override;
                void                            switch_base_path(std::string const& base_path);
                void                            override_image(cv::Mat3f const& img);
                void                            scale_regions(float x, float y);
        private:
                std::vector<e8::ellipse>        m_regions;
                std::string                     m_img_path;
                std::string                     m_base_path;
        };

        id_name_t               id2name() const override;
        unsigned                sample_count() const override;
        fddb_iterator const&    get(unsigned i) const override;
private:
        std::vector<fddb_iterator>      m_iters;
};

}

#endif // IMAGE_SAMPLES_H
