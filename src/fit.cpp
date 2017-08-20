#include <iostream>
#include <fstream>
#include "distribution.h"
#include "fit.h"


e8::if_fitter::if_fitter()
{
}

e8::if_fitter::~if_fitter()
{
}



e8::annotated_region_fitter::annotated_region_fitter(e8util::image_samples const& samples,
                                                     if_region_fittable& fittable,
                                                     std::string const& param_file,
                                                     std::string const& log_file):
        m_samples(samples), m_fittable(fittable), m_param_file(param_file), m_log_file(log_file)
{
}

float
e8::annotated_region_fitter::fit()
{
        std::ofstream log(m_log_file, std::fstream::app);

        e8util::rng rng;
        unsigned s = m_samples.sample_count();
        unsigned c = s*0.01f;

        float avg_loss = 0;
        for (unsigned i = 0; i < c; i ++) {
                unsigned selected = rng.draw()*s;

                e8util::image_samples::iterator const& it = m_samples.get(selected);
                cv::Mat3f const& img = it.image();
                std::vector<e8util::image_annotation> const& annos = it.annotations();

                std::vector<if_region*> regions(annos.size());
                for (unsigned i = 0; i < annos.size(); i ++)
                        regions.push_back(const_cast<if_region*>(&annos[i].region));

                float loss = m_fittable.fit(img, regions);
                avg_loss += loss;
                log << loss << std::endl;
        }

        return avg_loss/c;
}

void
e8::annotated_region_fitter::backup()
{
        std::ofstream file(m_param_file);
        if (!file.is_open())
                return;
        m_fittable.export_params(file);
}

void
e8::annotated_region_fitter::recover()
{
        std::ifstream file(m_param_file);
        if (!file.is_open())
                return;
        m_fittable.import_params(file);
}

float
e8::annotated_region_fitter::error() const
{
}
