#include "morphology.h"


e8util::island::island(unsigned id):
        id(id), area(0)
{
}


static void
islands_connected_components(e8util::island& island, cv::Mat1b const& bw, cv::Mat1b& id_map, unsigned target,  unsigned i, unsigned j)
{
        std::vector<cv::Point2i> candids({cv::Point2i(j, i)});

        while (!candids.empty()) {
                cv::Point2i const& p = candids.back();
                j = p.x;
                i = p.y;
                candids.pop_back();

                if (bw.at<unsigned char>(j, i) != target || id_map.at<unsigned char>(j, i) != 0)
                        continue;
                island.area ++;
                island.pixel_locs.push_back(cv::Point2i(j, i));
                id_map.at<unsigned char>(j, i) = 0xFF;

                if (i != 0)
                        candids.push_back(cv::Point2i(j, i - 1));
                if (i != static_cast<unsigned>(bw.size().width) - 1)
                        candids.push_back(cv::Point2i(j, i + 1));
                if (j != 0)
                        candids.push_back(cv::Point2i(j - 1, i));
                if (j != static_cast<unsigned>(bw.size().height) - 1)
                        candids.push_back(cv::Point2i(j + 1, i));
        }
}

std::vector<e8util::island>
e8util::islands(cv::Mat1b const& bw, cv::Mat1b& id_map, unsigned target)
{
        std::vector<island> is;

        id_map = cv::Mat1b::zeros(bw.size());
        unsigned char id = 0;
        for (unsigned j = 0; j < static_cast<unsigned>(bw.size().height); j ++) {
                for (unsigned i = 0; i < static_cast<unsigned>(bw.size().width); i ++) {
                        if (bw.at<unsigned char>(j, i) == target && id_map.at<unsigned char>(j, i) == 0) {
                                // found a seeding element
                                // perform graph traversal to find all connected components.
                                e8util::island a(++ id);
                                islands_connected_components(a, bw, id_map, target, i, j);
                                is.push_back(a);
                        }
                }
        }

        std::sort(is.begin(), is.end(), [](island const& a, island const& b) -> bool {
                return a.area > b.area;
        });
        return is;
}
