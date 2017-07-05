#include <iostream>
#include <algorithm>
#include <lmmin.h>
#include <opencv2/imgproc.hpp>
#include "util.h"
#include "geometry.h"
#include "calibrator.h"


// Helper functions.
struct keypoint
{
        keypoint(cv::Vec2i const& kp, std::set<unsigned> const& lines):
                kp(kp), lines(lines)
        {
        }

        cv::Vec2i               kp;
        std::set<unsigned>      lines;
};

struct principle_region
{
        principle_region(cv::Vec4i const& a, cv::Vec4i const& b)
        {
                basis[0] = a;
                basis[1] = b;
        }

        cv::Vec4i               basis[2];
        std::vector<unsigned>   kps;
};

static void
draw_points(cv::Mat& mat, std::vector<cv::Vec2f> const& kps, unsigned thickness, cv::Scalar const& color)
{
        for (unsigned i = 0; i < kps.size(); i ++) {
                cv::Vec2i const& p = cv::Vec2i(static_cast<unsigned>(kps[i][0]), static_cast<unsigned>(kps[i][1]));
                cv::circle(mat, p, thickness, color, thickness/2);
        }
}

static void
draw_keypoints(cv::Mat& mat, std::vector<keypoint> const& kps, unsigned thickness, cv::Scalar const& color)
{
        for (unsigned i = 0; i < kps.size(); i ++) {
                keypoint const& p = kps[i];
                cv::circle(mat, p.kp, thickness, color, thickness/2);
        }
}

static void
draw_lines(cv::Mat& mat, std::vector<cv::Vec4i> const& lines, unsigned thickness, cv::Scalar const& color)
{
        for (unsigned i = 0; i < lines.size(); i ++) {
                cv::Vec4i const& line = lines[i];
                cv::line(mat, cv::Point(line[0], line[1]), cv::Point(line[2], line[3]), color, thickness);
        }
}

static void
draw_principle_region(cv::Mat& mat, cv::Vec4i const* axes, std::vector<cv::Vec2i> const* kps, unsigned thickness)
{
        cv::Scalar colors[3] = {cv::Scalar(0, 0, 255), cv::Scalar(0, 255, 0), cv::Scalar(255, 0, 0)};
        unsigned shift_scale = thickness*5;
        cv::Vec2i shift[3] = {cv::Vec2i(0, shift_scale), cv::Vec2i(shift_scale, 0), cv::Vec2i(-shift_scale, 0)};

        for (unsigned j = 0; j < 3; j ++) {
                // draw axis.
                cv::Vec4i const& axis = axes[j];
                cv::line(mat, cv::Point(axis[0], axis[1]), cv::Point(axis[2], axis[3]), colors[j], thickness);

                // draw key point order.
                for (unsigned i = 0; i < kps[j].size(); i ++) {
                        cv::putText(mat, std::to_string(i), kps[j][i] + shift[j], cv::FONT_HERSHEY_COMPLEX_SMALL, 1.5, colors[j], 2, CV_AA);
                }
        }
}

static bool
is_point_on_line(cv::Vec2i const& p, cv::Vec4i const& line, float threshold)
{
        cv::Vec2f const& v = cv::Vec2f(p[0], p[1]) - cv::Vec2f(line[0], line[1]);
        cv::Vec2f l = cv::Vec2f(line[2], line[3]) - cv::Vec2f(line[0], line[1]);
        float len = std::sqrt(l.dot(l));
        l /= len;
        float comp_lv = l.dot(v);

        float bias = len*threshold;
        if (comp_lv < -bias || comp_lv > len + bias)
                return false;
        return v.dot(v) - comp_lv*comp_lv < bias*bias;
}

static bool
is_point_on_the_left(cv::Vec2i const& p, cv::Vec4i const& line)
{
        cv::Vec3f const& l = cv::Vec3f(line[2], line[3], 0) - cv::Vec3f(line[0], line[1], 0);
        cv::Vec3f const& v = cv::Vec3f(p[0], p[1], 0) - cv::Vec3f(line[0], line[1], 0);
        cv::Vec3f const& z = l.cross(v);
        return z[2] < 0;
}

static bool
is_point_on_the_right(cv::Vec2i const& p, cv::Vec4i const& line)
{
        return !is_point_on_the_left(p, line);
}

static bool
is_equivalent_lines(cv::Vec4i const& a, cv::Vec4i const& b, cv::Vec2i const&, cv::Vec2i const&)
{
        cv::Vec3f va = cv::Vec3f(a[0], a[1], 0) - cv::Vec3f(a[2], a[3], 0);
        cv::Vec3f vb = cv::Vec3f(b[0], b[1], 0) - cv::Vec3f(b[2], b[3], 0);

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

static principle_region
build_region(std::vector<keypoint> const& kps, cv::Vec4i const& x, cv::Vec4i const& y)
{
        principle_region region(x, y);
        for (unsigned i = 0; i < kps.size(); i ++) {
                keypoint const& kp = kps[i];

                // if the point lies on any of the axes.
                if (is_point_on_line(kp.kp, x, 0.05f) ||
                    is_point_on_line(kp.kp, y, 0.05f)) {
                        region.kps.push_back(i);
                        continue;
                }

                if (is_point_on_the_left(kp.kp, x) &&
                    is_point_on_the_right(kp.kp, y)) {
                        region.kps.push_back(i);
                }
        }
        return region;
}

static std::vector<principle_region>
find_principle_regions(std::vector<cv::Vec4i> const& lines, std::vector<keypoint> const& kps)
{
        // find intersection points that joins 3 lines.
        std::vector<keypoint> candids;
        for (unsigned i = 0; i < kps.size(); i ++) {
                if (kps[i].lines.size() == 3)
                        candids.push_back(kps[i]);
        }

        // exclude non-central axes.
        std::vector<cv::Vec4i> axes(3);
        std::vector<cv::Vec4i> ordered_axes(3);
        for (unsigned i = 0; i < candids.size(); i ++) {
                // principle axes are lines that each pair of them separates a region,
                // and each of those regions must consist of equal number of lines.

                // test if the line is flipped and move them into the axes in arbitrary order.
                keypoint const& c = candids[i];
                unsigned axis = 0;
                for (unsigned il: c.lines) {
                        cv::Vec4i const& line = lines[il];
                        cv::Vec2i d_a = cv::Vec2i(line[0], line[1]) - c.kp;
                        cv::Vec2i d_b = cv::Vec2i(line[2], line[3]) - c.kp;
                        if (d_a.dot(d_a) < d_b.dot(d_b)) {
                                // the line is good.
                                axes[axis ++] = cv::Vec4i(c.kp[0], c.kp[1], line[2], line[3]);
                        } else {
                                // the line is flipped.
                                axes[axis ++] = cv::Vec4i(c.kp[0], c.kp[1], line[0], line[1]);
                        }
                }

                // make axes the correct order.
                // dot product against the global y axis will divide the 3 axes into two classes a and b.
                cv::Vec2i global_y(0, 1);
                unsigned a[3];
                unsigned b[3];
                unsigned as = 0;
                unsigned bs = 0;
                for (unsigned i = 0; i < 3; i ++) {
                        cv::Vec2i const& v = cv::Vec2i(axes[i][2], axes[i][3]) - cv::Vec2i(axes[i][0], axes[i][1]);
                        if (v.dot(global_y) < 0)        b[bs ++] = i;
                        else                            a[as ++] = i;
                }
                // the class with 1 element is the z axis while the other contains the x and y axes.
                unsigned* xy;
                if (as == 1) {
                        ordered_axes[2] = axes[a[0]];
                        xy = b;
                } else if (bs == 1) {
                        ordered_axes[2] = axes[b[0]];
                        xy = a;
                } else {
                        // failed.
                        continue;
                }

                // we can produce the local x axis and the dot product against this local x axis
                // will divide the xy class into the x and y axis respectively.
                cv::Vec3f local_z(ordered_axes[2][0], ordered_axes[2][1], 0.0f);
                cv::Vec3f global_z(0, 0, 1);
                cv::Vec3f const& local_x3 = global_z.cross(local_z);
                cv::Vec2i local_x(local_x3[0], local_x3[1]);

                for (unsigned i = 0; i < 2; i ++) {
                        cv::Vec2i const& v = cv::Vec2i(axes[xy[i]][2], axes[xy[i]][3]) - cv::Vec2i(axes[xy[i]][0], axes[xy[i]][1]);
                        if (local_x.dot(v) > 0) ordered_axes[0] = axes[xy[i]];
                        else                    ordered_axes[1] = axes[xy[i]];
                }

                // now count the number of points in the separated region.
                principle_region const& xy_region = build_region(kps, ordered_axes[0], ordered_axes[1]);
                principle_region const& yz_region = build_region(kps, ordered_axes[1], ordered_axes[2]);
                principle_region const& zx_region = build_region(kps, ordered_axes[2], ordered_axes[0]);

                if (xy_region.kps.size() == yz_region.kps.size() && yz_region.kps.size() == zx_region.kps.size()) {
                        // there is a match, and we found the central axes.
                        return std::vector<principle_region>({xy_region, yz_region, zx_region});
                }
        }

        // no matching axes.
        return std::vector<principle_region>();
}

static std::vector<principle_region>
partition(std::vector<cv::Vec2i> planes[3], std::vector<cv::Vec4i> const& lines, std::vector<keypoint> const& kps)
{
        std::vector<principle_region> regions = find_principle_regions(lines, kps);
        if (regions.empty())
                return regions;

        for (unsigned j = 0; j < 3; j ++) {
                principle_region& region = regions[j];

                // group the region keypoints by lines parallel to the first basis.
                std::map<unsigned, std::vector<unsigned>> group;
                std::vector<std::pair<unsigned, cv::Vec4i>> line_grp;

                cv::Vec2f const& basis1 = cv::Vec2f(region.basis[0][2], region.basis[0][3]) - cv::Vec2f(region.basis[0][0], region.basis[0][1]);
                cv::Vec2f const& basis2 = cv::Vec2f(region.basis[1][2], region.basis[1][3]) - cv::Vec2f(region.basis[1][0], region.basis[1][1]);
                cv::Vec4i const& basis2_line = region.basis[1];
                for (unsigned i = 0; i < region.kps.size(); i ++) {
                        unsigned ikp = region.kps[i];
                        keypoint const& kp = kps[ikp];

                        // similarity to the first basis.
                        unsigned close_line;
                        float closeness = 0.0f;
                        for (unsigned il: kp.lines) {
                                cv::Vec4i const& line = lines[il];
                                cv::Vec2f v = cv::Vec2f(line[2], line[3]) - cv::Vec2f(line[0], line[1]);
                                v /= std::sqrt(v.dot(v));
                                float similarity = std::abs(basis1.dot(v));
                                if (similarity > closeness) {
                                        closeness = similarity;
                                        close_line = il;
                                }
                        }

                        // build the line group where each line starts from the second basis.
                        if (group.find(close_line) == group.end()) {
                                cv::Vec4i const& line = lines[close_line];
                                if (is_point_on_line(cv::Vec2i(line[0], line[1]), basis2_line, 0.1f)) {
                                        // the line is in the correct orientation.
                                        line_grp.push_back(std::pair<unsigned, cv::Vec4i>(close_line, line));
                                } else {
                                        // the line is flipped.
                                        line_grp.push_back(std::pair<unsigned, cv::Vec4i>(close_line, cv::Vec4i(line[2], line[3], line[0], line[1])));
                                }
                        }
                        group[close_line].push_back(ikp);
                }

                // sort the line group based on the second basis.
                cv::Vec2f const& origin = cv::Vec2f(region.basis[1][0], region.basis[1][1]);
                std::sort(line_grp.begin(), line_grp.end(),
                          [&basis2, &origin](std::pair<unsigned, cv::Vec4i> const& a, std::pair<unsigned, cv::Vec4i> const& b) -> bool {
                        cv::Vec4i const& pa = a.second;
                        cv::Vec4i const& pb = b.second;
                        if (std::abs(basis2[0]) > std::abs(basis2[1])) {
                                return (pa[0] - origin[0])/basis2[0] < (pb[0] - origin[0])/basis2[0];
                        } else {
                                return (pa[1] - origin[1])/basis2[1] < (pb[1] - origin[1])/basis2[1];
                        }
                });

                // sort each line group based on the line axis.
                for (unsigned i = 0; i < line_grp.size(); i ++) {
                        std::vector<unsigned>& ikps = group[line_grp[i].first];
                        cv::Vec4i const& line_axis = line_grp[i].second;
                        cv::Vec2f const& line_origin = cv::Vec2f(line_axis[0], line_axis[1]);
                        cv::Vec2f const& line_v = cv::Vec2f(line_axis[2], line_axis[3]) - line_origin;
                        std::sort(ikps.begin(), ikps.end(),
                                  [&line_v, &line_origin, kps](unsigned a, unsigned b) -> bool {
                                cv::Vec2i const& pa = kps[a].kp;
                                cv::Vec2i const& pb = kps[b].kp;
                                if (std::abs(line_v[0]) > std::abs(line_v[1])) {
                                        return (pa[0] - line_origin[0])/line_v[0] < (pb[0] - line_origin[0])/line_v[0];
                                } else {
                                        return (pa[1] - line_origin[1])/line_v[1] < (pb[1] - line_origin[1])/line_v[1];
                                }
                        });
                }

                // flatten the region keypoints onto its corresponding plane
                std::vector<cv::Vec2i>& plane = planes[j];
                region.kps.clear();
                plane.clear();
                for (unsigned i = 0; i < line_grp.size(); i ++) {
                        std::vector<unsigned> const& ikps = group[line_grp[i].first];
                        for (unsigned j = 0; j < ikps.size(); j ++) {
                                region.kps.push_back(ikps[j]);
                                plane.push_back(kps[ikps[j]].kp);
                        }
                }
        }
        return regions;
}

static cv::Matx33f
homography(std::vector<cv::Vec2i> const& kps, bool is_algebraic = false)
{
        unsigned plane_size = static_cast<unsigned>(std::sqrt(kps.size()));

        if (is_algebraic) {
                cv::Mat1f a = cv::Mat1f::zeros(2*kps.size(), 9);
                for (unsigned j = 0; j < kps.size(); j ++) {
                        unsigned px = j % plane_size;
                        unsigned py = j / plane_size;

                        unsigned mj = j*2;
                        // Px(k)        Py(k)   1       0       0       0       -x*Px(k)        -y*Px(k)
                        // 0            0       0       Px(k)   Py(k)   1       -x*Py(k)        -y*Py(k)
                        a.at<float>(mj + 0, 0) = px;
                        a.at<float>(mj + 0, 1) = py;
                        a.at<float>(mj + 0, 2) = 1;
                        a.at<float>(mj + 0, 6) = -px*kps[j][0];
                        a.at<float>(mj + 0, 7) = -py*kps[j][0];
                        a.at<float>(mj + 0, 8) = -kps[j][0];

                        a.at<float>(mj + 1, 3) = px;
                        a.at<float>(mj + 1, 4) = py;
                        a.at<float>(mj + 1, 5) = 1;
                        a.at<float>(mj + 1, 6) = -px*kps[j][1];
                        a.at<float>(mj + 1, 7) = -py*kps[j][1];
                        a.at<float>(mj + 1, 8) = -kps[j][1];
                }

                cv::Mat1f w, u, vt;
                cv::SVDecomp(a, w, u, vt);

                // load the least square solution into h.
                cv::Matx33f h;
                for (unsigned j = 0; j < 3; j ++) {
                        for (unsigned i = 0; i < 3; i ++) {
                                h(j, i) = vt.at<float>(8, i + j*3);
                        }
                }

                return h;
        } else {
                std::vector<cv::Vec2i> src;
                for (unsigned j = 0; j < kps.size(); j ++) {
                        unsigned px = j % plane_size;
                        unsigned py = j / plane_size;
                        src.push_back(cv::Vec2i(px, py));
                }

                // load the least square solution to h.
                return cv::findHomography(src, kps, cv::noArray());
        }
}

static std::vector<e8::point_corr>
correspondences(std::vector<cv::Vec2i> const& kps, unsigned num_points, float grid_width, unsigned dimension)
{
        cv::Matx33f const& h = homography(kps);

        std::vector<e8::point_corr> corrs;
        for (unsigned j = 0; j < num_points; j ++) {
                unsigned size = static_cast<unsigned>(std::sqrt(kps.size()));

                for (unsigned i = 0; i < num_points; i ++) {
                        float const sy = static_cast<float>(j)/(num_points - 1);
                        float const sx = static_cast<float>(i)/(num_points - 1);

                        float const py = (size - 1)*sy;
                        float const px = (size - 1)*sx;

                        float const ry = sy*grid_width;
                        float const rx = sx*grid_width;

                        // projected screen points.
                        cv::Vec3f const& c = h*cv::Vec3f(px, py, 1.0f);

                        // real point.
                        cv::Vec3f rp;
                        rp[dimension] = rx;
                        rp[(dimension + 1)%3] = ry;
                        rp[(dimension + 2)%3] = 0.0f;

                        corrs.push_back(e8::point_corr(cv::Vec3f(c[0]/c[2], c[1]/c[2], 1.0f), rp));
                }
        }
        return corrs;
}

static std::vector<cv::Vec4i>
hough_lines(cv::Mat1b const& edges, cv::Vec2i const& pmin, cv::Vec2i const& pmax)
{
        cv::Vec2i const& scale = pmax - pmin;
        float m_scale = (scale[0] + scale[1])/2;

        std::vector<cv::Vec4i> lines;
        cv::HoughLinesP(edges, lines, 1, CV_PI/180, m_scale * 0.03f, m_scale * 0.05f, m_scale * 0.1f);
        return lines;
}

static std::vector<cv::Vec4i>
merge(std::vector<cv::Vec4i> const& lines, cv::Vec2i const& pmin, cv::Vec2i const& pmax)
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

static std::vector<cv::Vec4i>
vectorize(cv::Mat1b const& edges, cv::Vec2i const& pmin, cv::Vec2i const& pmax)
{
        return merge(hough_lines(edges, pmin, pmax), pmin, pmax);
}

static std::vector<keypoint>
keypoints(std::vector<cv::Vec4i> const& lines, cv::Vec2i const& pmin, cv::Vec2i const& pmax)
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

                        if (t[0] < -bias || t[0] > 1.0f + bias ||
                            t[1] < -bias || t[1] > 1.0f + bias)
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

static cv::Mat1b
binarize(cv::Mat1b const& image, cv::Vec2i& pmin, cv::Vec2i& pmax)
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

        m_scale = pmax - pmin;
        m_thickness = (m_scale[0] + m_scale[1])/2 * 0.005f;
        detect_map = cv::Mat3i::zeros(edges.size());

        // draw result onto the detect map.
        draw_lines(detect_map, lines, m_thickness, cv::Scalar(255, 255, 255));
        draw_keypoints(detect_map, kps, m_thickness, cv::Scalar(255, 0, 0));


        // test if the result is reasonable.
        unsigned n = m_grids*m_grids*3 + m_grids*3 + 1;
        if (kps.size() != n)
                return false;

        // partition the keypoints into its own plane.
        std::vector<principle_region> const& regions = partition(m_planes, lines, kps);
        if (regions.empty())
                return false;
        for (unsigned i = 0; i < 3; i ++)
                m_axes[i] = regions[i].basis[0];

        // draw the partition result.
        draw_principle_region(detect_map, m_axes, m_planes, m_thickness);
        return true;
}

// cam_params(f, thx, thz, tx, ty, tz).
struct param_vec
{
        union {
                struct {
                        double  f;
                        double  thx;
                        double  thz;
                        double  tx;
                        double  ty;
                        double  tz;
                };

                double  params[6];
        };

        param_vec()
        {
                for (unsigned i = 0; i < 6; i ++)
                        params[i] = 0;
        }
};

static std::ostream&
operator<< (std::ostream& os, param_vec const& vec)
{
        os << "param_vec=["
           << vec.f << ","
           << vec.thx/M_PI*180 << "," << vec.thz/M_PI*180 << ","
           << vec.tx << "," << vec.ty << "," << vec.tz << "]";
        return os;
}

struct fitting_data
{
        fitting_data(std::vector<e8::point_corr> const& corrs,
                     cv::Vec2i const& center):
                corrs(corrs), center(center)
        {
        }

        std::vector<e8::point_corr> const&      corrs;
        cv::Vec2i                               center;
};

static void
reprojection_error(double const* p, int, void const* data, double* f, int*)
{
        param_vec const* vec = (param_vec const*) p;
        fitting_data const* fdata = (fitting_data const*) data;

        e8::camera cam(vec->f, fdata->center,
                       cv::Vec3f(vec->tx, vec->ty, vec->tz),
                       e8::rotation_xyz_transform(vec->thx, 0.0f, vec->thz));

        cam.proj_sqerr(f, fdata->corrs);
}

static e8::camera
fit_camera(std::vector<e8::point_corr> const& corrs, cv::Vec2f const& center, float grid_size)
{
        // fitting data.
        fitting_data data(corrs, center);

        // find the best-fit parameterization.
        param_vec       best_vec;
        double          e[corrs.size()];
        double          min_error = INFINITY;
        unsigned const  max_iters = 40000;
        e8util::rng     rng;

        for (unsigned i = 0; i < max_iters; i ++) {
                // initial values.
                param_vec vec;
                vec.f = rng.draw()*(5000 - 100) + 100;
                vec.thx = rng.draw()*M_PI/2;
                vec.thz = -rng.draw()*M_PI/2;
                vec.tx = rng.draw()*(5*grid_size - grid_size) + grid_size;
                vec.ty = rng.draw()*(5*grid_size - grid_size) + grid_size;
                vec.tz = rng.draw()*(5*grid_size - grid_size) + grid_size;

                // fit parameters.
                lm_control_struct control = lm_control_double;
                lm_status_struct  status;
                lmmin(6, vec.params, corrs.size(), static_cast<void*>(&data), reprojection_error, &control, &status);

                // compare the reprojection error with the previously best guess.
                reprojection_error(vec.params, 6, static_cast<void*>(&data), e, nullptr);

                double se = 0.0f;
                for (unsigned i = 0; i < corrs.size(); i ++)
                        se += e[i];

                if (se < min_error) {
                        std::cout << i <<  "th trial has the best error: " << se/corrs.size() << std::endl;
                        std::cout << i << "th parameterization: " << vec << std::endl << std::endl;
                        min_error = se;
                        best_vec = vec;
                }
        }

        // load the optimized results to the camera.
        return e8::camera(best_vec.f, center,
                          cv::Vec3f(best_vec.tx, best_vec.ty, best_vec.tz),
                          e8::rotation_xyz_transform(best_vec.thx, 0.0f, best_vec.thz));
}

bool
e8::checker_calibrator::calibrate(camera& cam, cv::Mat& project_map) const
{
        project_map = cv::Mat3i::zeros(m_checker.size());
        draw_principle_region(project_map, m_axes, m_planes, m_thickness);

        // extract correspondences from plane keypoints.
        cv::Scalar colors[3] = {cv::Scalar(0, 0, 255), cv::Scalar(0, 255, 0), cv::Scalar(255, 0, 0)};

        std::vector<point_corr> corrs;
        std::vector<cv::Vec3f> pts_3d;
        for (unsigned i = 0; i < 3; i ++) {
                std::vector<point_corr> const& plane_corrs = correspondences(m_planes[i], 10, m_width, i);

                // Aggregate the correspondences and visualize them.
                std::vector<cv::Vec2f> pts_2d(plane_corrs.size());
                for (unsigned j = 0; j < plane_corrs.size(); j ++) {
                        pts_2d[j] = cv::Vec2f(plane_corrs[j].a[0], plane_corrs[j].a[1]);
                        pts_3d.push_back(plane_corrs[j].b);
                        corrs.push_back(plane_corrs[j]);
                }
                draw_points(project_map, pts_2d, m_thickness, colors[i]);
        }

        // fit camera parameters from correspondences.
        cam = fit_camera(corrs, cv::Vec2f(m_checker.cols/2, m_checker.rows/2), m_width);

        // plot the estimated projection using the fitted camera.
        std::vector<cv::Vec2f> const& projs = cam.proj(pts_3d);
        draw_points(project_map, projs, m_thickness, cv::Scalar(0, 255, 255));
        return true;
}
