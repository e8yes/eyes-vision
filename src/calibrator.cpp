#include <iostream>
#include <algorithm>
#include <opencv2/imgproc.hpp>
#include "util.h"
#include "calibrator.h"


e8::if_calibrator::if_calibrator()
{
}

e8::if_calibrator::~if_calibrator()
{
}


e8::checker_calibrator::checker_calibrator(cv::Mat1b const& checker, float width, unsigned num_grids, camera const& init):
        m_checker(checker), m_width(width), m_grids(std::sqrt(num_grids)), m_init(init)
{
        if (m_grids < 2)
                throw std::string("not enough grid points.");
        for (unsigned i = 0; i < 3; i ++)
                m_planes[i] = std::vector<cv::Vec2i>();
}

e8::checker_calibrator::~checker_calibrator()
{
}

bool
e8::checker_calibrator::is_equivalent_lines(cv::Vec4i const& a, cv::Vec4i const& b, cv::Vec2i const&, cv::Vec2i const&) const
{
        cv::Vec3f va = cv::Vec3f(a[0], a[1], 1.0f) - cv::Vec3f(a[2], a[3], 1.0f);
        cv::Vec3f vb = cv::Vec3f(b[0], b[1], 1.0f) - cv::Vec3f(b[2], b[3], 1.0f);

        // normalize.
        float la = std::sqrt(va.dot(va));
        float lb = std::sqrt(vb.dot(vb));
        va /= la;
        vb /= lb;

        // are they almost parallel?
        cv::Vec3f k = va.cross(vb);
        if (k.dot(k) > 0.01f)
                return false;

        // are their distance close?
        cv::Vec2f d = cv::Vec2f(b[0], b[1]) - cv::Vec2f(a[0], a[1]);
        float comp = d.dot(cv::Vec2f(va[0], va[1]));
        float dd2 = d.dot(d);
        float h = 1.0f/std::max(la, lb);
        if ((dd2 - comp*comp)*h*h > 0.01f)
                return false;
        return true;
}

std::vector<cv::Vec4i>
e8::checker_calibrator::find_principle_axes(std::vector<cv::Vec4i> const& lines, std::vector<keypoint> const& kps) const
{
        // find intersection points that joins 3 lines.
        std::vector<keypoint> candids;
        for (unsigned i = 0; i < kps.size(); i ++) {
                if (kps[i].lines.size() == 3)
                        candids.push_back(kps[i]);
        }

        // exclude maximas.
        keypoint const* central = nullptr;
        std::vector<cv::Vec4i> axes;
        std::vector<cv::Vec4i> ordered_axes;
        for (unsigned i = 0; i < candids.size(); i ++) {
                // principle axes are lines that each pair of them separates a region,
                // and each of those regions must consist of equal number of lines.

                keypoint const& c = candids[i];
                axes.clear();
                for (cv::Vec4i const& line: c.lines) {
                        // test if the line is flipped.
                        cv::Vec2i d_a = cv::Vec2i(line[0], line[1]) - c.kp;
                        cv::Vec2i d_b = cv::Vec2i(line[2], line[3]) - c.kp;
                        if (d_a.dot(d_a) < d_b.dot(d_b)) {
                                // the line is good.
                                axes.push_back(line);
                        } else {
                                // the line is flipped.
                                axes.push_back(line);
                        }
                }

                // make lines into correct order.
                cv::Vec2i y(0, 1);
                cv::Vec2i x(1, 0);

        }

        if (central == nullptr)
                return std::vector<cv::Vec4i>();
        else
                return ordered_axes;
}

std::vector<cv::Vec4i>
e8::checker_calibrator::partition(std::vector<cv::Vec2i> planes[3], std::vector<cv::Vec4i> const& lines, std::vector<keypoint> const& kps) const
{
        std::vector<cv::Vec4i> const& axes = find_principle_axes(lines, kps);
        return axes;
}

cv::Matx33f
e8::checker_calibrator::homography(std::vector<cv::Vec2i> const& kps) const
{
}

std::vector<cv::Vec2f>
e8::checker_calibrator::correspondences(std::vector<cv::Vec2i> const& kps) const
{
}

std::vector<cv::Vec4i>
e8::checker_calibrator::hough_lines(cv::Mat1b const& edges, cv::Vec2i const& pmin, cv::Vec2i const& pmax) const
{
        cv::Vec2i const& scale = pmax - pmin;
        float m_scale = (scale[0] + scale[1])/2;

        std::vector<cv::Vec4i> lines;
        cv::HoughLinesP(edges, lines, 1, CV_PI/180, m_scale * 0.03f, m_scale * 0.05f, m_scale * 0.1f);
        return lines;
}

std::vector<cv::Vec4i>
e8::checker_calibrator::merge(std::vector<cv::Vec4i> const& lines, cv::Vec2i const& pmin, cv::Vec2i const& pmax) const
{
        std::vector<bool> has_merged(lines.size());
        for (unsigned i = 0; i < has_merged.size(); i ++)
                has_merged[i] = false;

        // find equivalence classes.
        std::vector<std::vector<cv::Vec4i>> equivalence;
        for (unsigned i = 0; i < lines.size(); i ++) {
                if (has_merged[i])
                        continue;

                std::vector<cv::Vec4i> curr_class;
                curr_class.push_back(lines[i]);

                // look for mergeable candidates and put them into currernt class.
                for (unsigned j = i + 1; j < lines.size(); j ++) {
                        if (has_merged[j])
                                continue;

                        // test if the jth line is mergeable with the ith line.
                        if (is_equivalent_lines(lines[i], lines[j], pmin, pmax)) {
                                has_merged[j] = true;
                                curr_class.push_back(lines[j]);
                        }
                }

                equivalence.push_back(curr_class);
        }

        // merge line within the same equivalance class.
        std::vector<cv::Vec4i> merged(equivalence.size());
        for (unsigned i = 0; i < equivalence.size(); i ++) {
                std::vector<cv::Vec4i> const& clazz = equivalence[i];

                // find the principle component.
                cv::Vec4i const& il = clazz[0];
                cv::Vec2f v = cv::Vec2f(il[0], il[1]) - cv::Vec2f(il[2], il[3]);
                float w = std::sqrt(v.dot(v));
                cv::Vec2f centroid = w*(cv::Vec2f(il[0], il[1]) + cv::Vec2f(il[2], il[3]))/2.0f;
                for (unsigned j = 1; j < clazz.size(); j ++) {
                        cv::Vec4i const& l = clazz[j];
                        cv::Vec2f vj = cv::Vec2f(l[0], l[1]) - cv::Vec2f(l[2], l[3]);
                        v += vj.dot(v) > 0 ? vj : -vj;

                        float wj = vj.dot(vj);
                        centroid += wj*(cv::Vec2f(l[0], l[1]) + cv::Vec2f(l[2], l[3]))/2.0f;

                        w += wj;
                }

                centroid /= w;
                v /= static_cast<float>(clazz.size());
                v /= std::sqrt(v.dot(v));

                // pick the most extreme points.
                float t_min = 0.0f;
                float t_max = 0.0f;
                for (unsigned j = 0; j < clazz.size(); j ++) {
                        cv::Vec4i const& l = clazz[j];
                        cv::Vec2f v0 = cv::Vec2f(l[0], l[1]) - centroid;
                        cv::Vec2f v1 = cv::Vec2f(l[2], l[3]) - centroid;
                        float t0 = v.dot(v0);
                        float t1 = v.dot(v1);
                        if (t0 < t1) {
                                t_min = std::min(t_min, t0);
                                t_max = std::max(t_max, t1);
                        } else {
                                t_min = std::min(t_min, t1);
                                t_max = std::max(t_max, t0);
                        }
                }

                cv::Vec2f final_s = centroid + v*t_min;
                cv::Vec2f final_e = centroid + v*t_max;
                merged[i] = cv::Vec4i(static_cast<int>(final_s[0]), static_cast<int>(final_s[1]),
                                      static_cast<int>(final_e[0]), static_cast<int>(final_e[1]));
        }
        return merged;
}

std::vector<cv::Vec4i>
e8::checker_calibrator::vectorize(cv::Mat1b const& edges, cv::Vec2i const& pmin, cv::Vec2i const& pmax) const
{
        return merge(hough_lines(edges, pmin, pmax), pmin, pmax);
}

std::vector<e8::checker_calibrator::keypoint>
e8::checker_calibrator::keypoints(std::vector<cv::Vec4i> const& lines, cv::Vec2i const& pmin, cv::Vec2i const& pmax) const
{
        // compute intersections.
        float const bias = 0.1f;
        std::vector<keypoint> pts;
        for (unsigned i = 0; i < lines.size(); i ++) {
                for (unsigned j = i + 1; j < lines.size(); j ++) {
                        cv::Vec4i const& li = lines[i];
                        cv::Vec4i const& lj = lines[j];

                        cv::Vec2f vi = cv::Vec2f(li[2], li[3]) - cv::Vec2f(li[0], li[1]);
                        cv::Vec2f vj = cv::Vec2f(lj[2], lj[3]) - cv::Vec2f(lj[0], lj[1]);
                        cv::Matx22f v(vi[0], -vj[0], vi[1], -vj[1]);
                        cv::Vec2f o = cv::Vec2f(lj[0], lj[1]) - cv::Vec2f(li[0], li[1]);

                        cv::Vec2f t;
                        cv::solve(v, o, t);

                        if (t[0] < -bias || t[0] > 1.0f + bias)
                                continue;
                        if (t[1] < -bias || t[1] > 1.0f + bias)
                                continue;

                        cv::Vec2f const& p = cv::Vec2f(li[0], li[1]) + vi*t[0];
                        std::set<unsigned> lines({i, j});
                        pts.push_back(keypoint(p, lines));
                }
        }

        // compute equivalence classes of points.
        std::vector<bool> has_merged(pts.size());
        for (unsigned i = 0; i < has_merged.size(); i ++)
                has_merged[i] = false;

        std::vector<std::vector<keypoint>> equivalence;
        cv::Vec2i const& scale = pmax - pmin;
        float radius = (scale[0] + scale[1])/2 * 0.1f;
        for (unsigned i = 0; i < pts.size(); i ++) {
                if (has_merged[i])
                        continue;

                std::vector<keypoint> curr_class;
                curr_class.push_back(pts[i]);

                for (unsigned j = i + 1; j < pts.size(); j ++) {
                        if (has_merged[j])
                                continue;

                        keypoint const& pi = pts[i];
                        keypoint const& pj = pts[j];
                        float dx = pi.kp[0] - pj.kp[0];
                        float dy = pi.kp[1] - pj.kp[1];

                        if (dx*dx + dy*dy < radius*radius) {
                                has_merged[j] = true;
                                curr_class.push_back(pj);
                        }
                }
                equivalence.push_back(curr_class);
        }

        // merge points.
        std::vector<keypoint> merged;
        for (unsigned i = 0; i < equivalence.size(); i ++) {
                std::vector<keypoint> const& clazz = equivalence[i];

                // compute average point.
                cv::Vec2f mu = clazz[0].kp;
                std::set<unsigned> u(clazz[0].lines.begin(), clazz[0].lines.end());
                for (unsigned j = 1; j < clazz.size(); j ++) {
                        mu += clazz[j].kp;
                        u.insert(clazz[j].lines.begin(), clazz[j].lines.end());
                }
                mu /= static_cast<float>(clazz.size());
                merged.push_back(keypoint(cv::Vec2i(static_cast<unsigned>(mu[0]), static_cast<unsigned>(mu[1])), u));
        }

        return merged;
}

cv::Mat1b
e8::checker_calibrator::binarize(cv::Mat1b const& image, cv::Vec2i& pmin, cv::Vec2i& pmax) const
{
        // binarize checker image.
        unsigned char max = 0;
        unsigned char min = 255;
        image.forEach([&min, &max](unsigned char const& v, int const*) {
                min = std::min(min, v);
                max = std::max(max, v);
        });

        cv::Mat1b bw(image.size());
        unsigned char split = min + (max - min) * 0.25f;
        image.forEach([&bw, &split](float const& v, int const* p) {
                bw.at<unsigned char>(p[0], p[1]) = v < split ? 0 : 0xff;
        });

        // detect scale.
        pmin = cv::Vec2i(bw.cols, bw.rows);
        pmax = cv::Vec2i(0, 0);
        for (int j = 0; j < bw.rows; j ++) {
                for (int i = 0; i < bw.cols; i ++) {
                        if (bw.at<unsigned char>(j, i) != 0) {
                                pmin[0] = std::min(i, pmin[0]);
                                pmin[1] = std::min(j, pmin[0]);

                                pmax[0] = std::max(i, pmax[0]);
                                pmax[1] = std::max(j, pmax[0]);
                        }
                }
        }
        return bw;
}

bool
e8::checker_calibrator::detect(cv::Mat& detect_map)
{
        // preprocess the checker image.
        cv::Vec2i pmin, pmax;
        cv::Mat1b const& bw = binarize(m_checker, pmin, pmax);

        // detect edges.
        cv::Mat1b edges(bw.size());
        cv::Canny(bw, edges, 40, 120);

        // detect lines.
        std::vector<cv::Vec4i> const& lines = vectorize(edges, pmin, pmax);

        // get key points.
        std::vector<keypoint> const& kps = keypoints(lines, pmin, pmax);

        // draw result onto the detect map.
        cv::Vec2i const& scale = pmax - pmin;
        float thickness = (scale[0] + scale[1])/2 * 0.005f;
        detect_map = cv::Mat3i::zeros(edges.size());
        for (unsigned i = 0; i < lines.size(); i ++) {
                cv::Vec4i const& line = lines[i];
                cv::line(detect_map, cv::Point(line[0], line[1]), cv::Point(line[2], line[3]), cv::Scalar(255, 255, 255), thickness);
        }
        for (unsigned i = 0; i < kps.size(); i ++) {
                keypoint const& p = kps[i];
                cv::circle(detect_map, p.kp, thickness, cv::Scalar(255, 0, 0), 10);
        }

        // test if the result is reasonable.
        unsigned n = m_grids*m_grids*3 + m_grids*3 + 1;
        if (kps.size() != n)
                return false;

        // partition the keypoints into its own plane.
        std::vector<cv::Vec4i> const& axes = partition(m_planes, lines, kps);
        if (axes.empty())
                return false;
        cv::line(detect_map, cv::Point(axes[0][0], axes[0][1]), cv::Point(axes[0][2], axes[0][3]), cv::Scalar(0, 0, 255), thickness);
        cv::line(detect_map, cv::Point(axes[1][0], axes[1][1]), cv::Point(axes[1][2], axes[1][3]), cv::Scalar(0, 255, 0), thickness);
        cv::line(detect_map, cv::Point(axes[2][0], axes[2][1]), cv::Point(axes[2][2], axes[2][3]), cv::Scalar(255, 0, 0), thickness);
        return true;
}

bool
e8::checker_calibrator::calibrate(camera& cam, cv::Mat& project_map) const
{
}
