#include <opencv2/imgproc.hpp>
#include <string>
#include "util.h"
#include "scanner.h"


e8::if_scanner::if_scanner()
{
}

e8::if_scanner::~if_scanner()
{
}


static void
load_struct_light_scan(std::string const& path, std::string const& suffix, unsigned num_bits,
                       std::vector<std::string>& left_hori, std::vector<std::string>& left_vert,
                       std::vector<std::string>& right_hori, std::vector<std::string>& right_vert)
{
        // left horizontal.
        for (unsigned i = 1; i <= num_bits; i ++) {
                if (i < 10) {
                        left_hori.push_back(path + "/l_0" + std::to_string(i) + "." + suffix);
                        left_hori.push_back(path + "/l_0" + std::to_string(i) + "_i." + suffix);
                } else {
                        left_hori.push_back(path + "/l_" + std::to_string(i) + "." + suffix);
                        left_hori.push_back(path + "/l_" + std::to_string(i) + "_i." + suffix);
                }
        }

        // left vertical.
        for (unsigned i = num_bits + 1; i <= 2*num_bits; i ++) {
                if (i < 10) {
                        left_vert.push_back(path + "/l_0" + std::to_string(i) + "." + suffix);
                        left_vert.push_back(path + "/l_0" + std::to_string(i) + "_i." + suffix);
                } else {
                        left_vert.push_back(path + "/l_" + std::to_string(i) + "." + suffix);
                        left_vert.push_back(path + "/l_" + std::to_string(i) + "_i." + suffix);
                }
        }

        // right horizontal.
        for (unsigned i = 1; i <= num_bits; i ++) {
                if (i < 10) {
                        right_hori.push_back(path + "/r_0" + std::to_string(i) + "." + suffix);
                        right_hori.push_back(path + "/r_0" + std::to_string(i) + "_i." + suffix);
                } else {
                        right_hori.push_back(path + "/r_" + std::to_string(i) + "." + suffix);
                        right_hori.push_back(path + "/r_" + std::to_string(i) + "_i." + suffix);
                }
        }

        // right vertical.
        for (unsigned i = num_bits + 1; i <= 2*num_bits; i ++) {
                if (i < 10) {
                        right_vert.push_back(path + "/r_0" + std::to_string(i) + "." + suffix);
                        right_vert.push_back(path + "/r_0" + std::to_string(i) + "_i." + suffix);
                } else {
                        right_vert.push_back(path + "/r_" + std::to_string(i) + "." + suffix);
                        right_vert.push_back(path + "/r_" + std::to_string(i) + "_i." + suffix);
                }
        }
}

e8::struct_light_scanner::struct_light_scanner(std::string const& path, std::string const& suffix, unsigned num_bits):
        m_num_bits(num_bits)
{
        load_struct_light_scan(path, suffix, num_bits, m_left_hori, m_left_vert, m_right_hori, m_right_vert);
}

e8::struct_light_scanner::~struct_light_scanner()
{
}

static cv::Mat2i
decode(std::vector<std::string> const& scan, unsigned num_bits)
{
        // extract gray code.
        float const thres = 0.01f;
        cv::Mat2i c;
        for (unsigned i = 0; i < scan.size(); i += 2) {
                cv::Mat1f const& i0 = e8util::im2grayfloat(cv::imread(scan[i]));
                cv::Mat1f const& i1 = e8util::im2grayfloat(cv::imread(scan[i + 1]));

                if (i == 0) {
                        // initialize the code map.
                        c = cv::Mat2i::zeros(i0.size());
                        c.forEach([](cv::Vec2i& v, int const*) {
                                v[1] = 0xffffff;
                        });
                }

                // decode gray code.
                unsigned i_bit = num_bits - i/2 - 1;
                i0.forEach([&c, &i1, i_bit, thres](float const& v0, int const* p) {
                        int* code = &c.at<cv::Vec2i>(p[0], p[1])[0];
                        int* validity = &c.at<cv::Vec2i>(p[0], p[1])[1];
                        if (validity == 0x0)
                                return;

                        float d = v0 - i1.at<float>(p[0], p[1]);
                        if (std::abs(d) < thres) {
                                // bad pixel.
                                *validity = 0x0;
                                *code = 0x0;
                        } else {
                                // decode;
                                if (d > 0.0f)   *code |= 1 << i_bit;
                                else            *code |= 0 << i_bit;
                        }
                });
        }

        // convert to binary code.
        c.forEach([&c, &num_bits](cv::Vec2i const& v, int const* p) {
                if (v[1] == 0x0)
                        return;
                unsigned bin = v[0] & (1 << (num_bits - 1));
                for (unsigned k = num_bits - 1; k > 0; k --) {
                        unsigned m = bin & (1 << k);
                        unsigned m2 = v[0] & (1 << (k - 1));
                        m >>= 1;
                        unsigned p = m ^ m2;
                        bin |= p;
                }
                c.at<cv::Vec2i>(p[0], p[1])[0] = bin;
        });
        return c;
}

static cv::Mat2i
decode_location(std::vector<std::string> const& horizontal, std::vector<std::string> const& vertical, unsigned num_bits)
{
        cv::Mat2i const& h = decode(horizontal, num_bits);
        cv::Mat2i const& v = decode(vertical, num_bits);

        cv::Mat2i merged(h.size());
        merged.forEach([&h, &v, &merged, num_bits](cv::Vec2i const&, int const* p) {
                if (h.at<cv::Vec2i>(p[0], p[1])[1] == 0x0 ||
                    v.at<cv::Vec2i>(p[0], p[1])[1] == 0x0) {
                        merged.at<cv::Vec2i>(p[0], p[1])[1] = 0x0;
                        merged.at<cv::Vec2i>(p[0], p[1])[0] = 0;
                } else {
                        merged.at<cv::Vec2i>(p[0], p[1])[1] = 0xffffff;
                        merged.at<cv::Vec2i>(p[0], p[1])[0] = h.at<cv::Vec2i>(p[0], p[1])[0] | (v.at<cv::Vec2i>(p[0], p[1])[0] << num_bits);
                }
        });
        return merged;
}

static std::vector<e8::img_corr>
match(cv::Mat2i const& a, cv::Mat2i const& b)
{
        std::unordered_map<unsigned, cv::Vec2i> b_matched;
        for (int j = 0; j < b.rows; j ++) {
                for (int i = 0; i < b.cols; i ++) {
                        cv::Vec2i const& v = b.at<cv::Vec2i>(j, i);
                        if (v[1] == 0x0)
                                continue;
                        if (b_matched.find(v[0]) != b_matched.end())
                                continue;
                        b_matched[v[0]] = cv::Vec2i(i, j);
                }
        }

        std::unordered_map<unsigned, bool> existence_marker;
        std::vector<e8::img_corr> corr;
        for (int j = 0; j < a.rows; j ++) {
                for (int i = 0; i < a.cols; i ++) {
                        cv::Vec2i const& v = a.at<cv::Vec2i>(j, i);
                        if (v[1] == 0x0)
                                continue;
                        auto it = b_matched.find(v[0]);
                        if (it == b_matched.end() || existence_marker[v[0]])
                                continue;
                        existence_marker[v[0]] = true;
                        corr.push_back(e8::img_corr(cv::Vec2i(i, j), it->second));
                }
        }
        return corr;
}

static std::vector<cv::Vec3f>
solve_point_location(std::vector<e8::img_corr> const& corr, e8::camera const& left, e8::camera const& right)
{
        // prepare unscaled correspondences.
        std::vector<cv::Vec3f> q_left(corr.size()), q_right(corr.size());
        for (unsigned i = 0; i < corr.size(); i ++) {
                q_left[i] = cv::Vec3f((corr[i].a[0] - left.c()[0])/left.f(),
                                      (corr[i].a[1] - left.c()[1])/left.f(),
                                      1.0f);
                q_right[i] = cv::Vec3f((corr[i].b[0] - right.c()[0])/right.f(),
                                       (corr[i].b[1] - right.c()[1])/right.f(),
                                       1.0f);
        }

        // make the left camera to the origin.
        cv::Matx33f const& r = left.inv_r()*right.r();
        cv::Vec3f const& t = left.inv_r()*(right.t() - left.t());

        // now solve the equation:
        // Z_l * qL = Z_r * R*qR + t for the Zs.
        std::vector<cv::Vec3f> pts(corr.size());
        for (unsigned i = 0; i < corr.size(); i ++) {
                cv::Vec3f const& r_qr = r*q_right[i];
                cv::Matx32f a(q_left[i][0], -r_qr[0],
                              q_left[i][1], -r_qr[1],
                              q_left[i][2], -r_qr[2]);
                cv::Vec2f z;
                cv::solve(a, t, z, cv::DECOMP_SVD);

                cv::Vec3f const& x_left = z(0)*q_left[i];
                cv::Vec3f const& x_right = z(1)*q_right[i];

                cv::Vec3f const& x_a = left.r()*x_left + left.t();
                cv::Vec3f const& x_b = right.r()*x_right + right.t();

                pts[i] = (x_a + x_b)/2.0f;
        }

        return pts;
}
/*
static std::vector<unsigned>
triangulate(std::vector<e8::img_corr> const& corr)
{
}
*/
e8::point_cloud
e8::struct_light_scanner::scan(std::vector<camera> const& cams) const
{
        cv::Mat2i const& left = decode_location(m_left_hori, m_left_vert, m_num_bits);
        cv::Mat2i const& right = decode_location(m_right_hori, m_right_vert, m_num_bits);
        std::vector<img_corr>const& corr = match(left, right);
        std::vector<cv::Vec3f> const& p3 = solve_point_location(corr, cams[0], cams[1]);

        e8::point_cloud pts;
        for (unsigned i = 0; i < p3.size(); i ++) {
                pts.push_back(vertex(p3[i], cv::Vec3f(0,0,0)));
        }
        return pts;
}
