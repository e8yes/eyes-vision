#include <iostream>
#include <fstream>
#include "image.h"
#include "imagesamples.h"


e8util::image_annotation::image_annotation(e8::if_region const& region, unsigned id):
        region(region), id(id)
{
}


e8util::image_samples::image_samples()
{
}

e8util::image_samples::~image_samples()
{
}



e8util::fddb_samples::fddb_iterator::fddb_iterator(std::vector<e8::ellipse> const& regions, std::string const& img_path):
        m_regions(regions), m_img_path(img_path)
{
}

std::vector<e8util::image_annotation>
e8util::fddb_samples::fddb_iterator::fddb_iterator::annotations() const
{
        std::vector<e8util::image_annotation> annos;
        for (unsigned i = 0; i < m_regions.size(); i ++)
                annos.push_back(e8util::image_annotation(m_regions[i], 1));
        return annos;
}

cv::Mat3f
e8util::fddb_samples::fddb_iterator::fddb_iterator::image() const
{
        return e8util::im2float(cv::imread(m_base_path + "/" + m_img_path, cv::IMREAD_COLOR));
}

void
e8util::fddb_samples::fddb_iterator::fddb_iterator::switch_base_path(std::string const& base_path)
{
        m_base_path = base_path;
}

void
e8util::fddb_samples::fddb_iterator::fddb_iterator::save_image(cv::Mat3f const& img)
{
        cv::imwrite(m_base_path + "/" + m_img_path, img*255.0f);
}

void
e8util::fddb_samples::fddb_iterator::scale_regions(float x, float y)
{
        for (unsigned i = 0; i < m_regions.size(); i ++) {
                m_regions[i].scale(x, y);
        }
}


static void
parse_fddb_annotations(std::string const& annotations, std::vector<e8util::fddb_samples::fddb_iterator>& iters)
{
        std::ifstream file(annotations);
        while (file.good()) {
                std::string path;
                file >> path;
                if (file.eof())
                        break;
                unsigned n_annos;
                file >> n_annos;
                std::vector<e8::ellipse> ellipses;
                for (unsigned i = 0; i < n_annos; i ++) {
                        float major_axis, minor_axis, cx, cy, angle, score;
                        file >> major_axis;
                        file >> minor_axis;
                        file >> angle;
                        file >> cx;
                        file >> cy;
                        file >> score;
                        ellipses.push_back(e8::ellipse(major_axis, minor_axis, cv::Vec2f(cx, cy), angle/180*M_PI));
                }
                e8util::fddb_samples::fddb_iterator it(ellipses, path + ".jpg");
                iters.push_back(it);
        }
}

static void
parse_all_fddb_annotations(std::string const& annotations, std::vector<e8util::fddb_samples::fddb_iterator>& iters)
{
        parse_fddb_annotations(annotations + "/FDDB-fold-01-ellipseList.txt", iters);
        parse_fddb_annotations(annotations + "/FDDB-fold-02-ellipseList.txt", iters);
        parse_fddb_annotations(annotations + "/FDDB-fold-03-ellipseList.txt", iters);
        parse_fddb_annotations(annotations + "/FDDB-fold-04-ellipseList.txt", iters);
        parse_fddb_annotations(annotations + "/FDDB-fold-05-ellipseList.txt", iters);
        parse_fddb_annotations(annotations + "/FDDB-fold-06-ellipseList.txt", iters);
        parse_fddb_annotations(annotations + "/FDDB-fold-07-ellipseList.txt", iters);
        parse_fddb_annotations(annotations + "/FDDB-fold-08-ellipseList.txt", iters);
        parse_fddb_annotations(annotations + "/FDDB-fold-09-ellipseList.txt", iters);
        parse_fddb_annotations(annotations + "/FDDB-fold-10-ellipseList.txt", iters);
}

e8util::fddb_samples::fddb_samples(std::string const& annotations, std::string const& imgs, std::string const& normalized)
{
        unsigned const target_size = 227;

        // parse annotated regions.
        parse_all_fddb_annotations(annotations, m_iters);

        // warp and find mean.
        cv::Mat3f mean(target_size, target_size);
        for (unsigned i = 0; i < m_iters.size(); i ++) {
                m_iters[i].switch_base_path(imgs);
                cv::Mat3f const& img = m_iters[i].image();
                // warp.
                float x_scaling = static_cast<float>(img.cols)/target_size;
                float y_scaling = static_cast<float>(img.rows)/target_size;
                m_iters[i].scale_regions(x_scaling, y_scaling);
                cv::Mat3f warped;
                cv::resize(img, warped, cv::Size(target_size, target_size), 0, 0, cv::INTER_LANCZOS4);
                // accumulate.
                mean += warped;
        }
        mean /= static_cast<float>(m_iters.size());

        // normalize and cache the normalized images.
        for (unsigned i = 0; i < m_iters.size(); i ++) {
                m_iters[i].switch_base_path(imgs);
                cv::Mat3f const& img = m_iters[i].image();
                cv::Mat3f warped;
                cv::resize(img, warped, cv::Size(target_size, target_size), 0, 0, cv::INTER_LANCZOS4);

                m_iters[i].switch_base_path(normalized);
                m_iters[i].save_image(warped - mean);
        }
}

e8util::fddb_samples::fddb_samples(std::string const& annotations, std::string const& normalized)
{
        parse_all_fddb_annotations(annotations, m_iters);

        for (unsigned i = 0; i < m_iters.size(); i ++) {
                m_iters[i].switch_base_path(normalized);
        }
}

e8util::fddb_samples::~fddb_samples()
{
}

e8util::id_name_t
e8util::fddb_samples::id2name() const
{
        id_name_t m;
        m[1] = "face";
        return m;
}

unsigned
e8util::fddb_samples::sample_count() const
{
        return m_iters.size();
}

e8util::fddb_samples::fddb_iterator const&
e8util::fddb_samples::get(unsigned i) const
{
        return m_iters[i];
}
