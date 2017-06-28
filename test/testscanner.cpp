#include <vector>
#include "src/geometry.h"
#include "src/camera.h"
#include "src/scanner.h"
#include "testscanner.h"


test::test_scanner::test_scanner()
{
}

void
test::test_scanner::run() const
{
        e8::camera left(1665, cv::Vec2f(819.1000, 669.1000),
                        cv::Vec3f(-9.875, 37.3890, 28.8938), cv::Matx33f(-0.7506, -0.4176, 0.5121,
                                                                         -0.6608, 0.4737, -0.5822,
                                                                         0.0006, -0.7753, -0.6315));
        e8::camera right(1695, cv::Vec2f(823.7000, 656),
                         cv::Vec3f(-21.2850, 25.2265, 27.7429), cv::Matx33f(-0.5493, -0.4291, 0.7170,
                                                                            -0.8170, 0.4562, -0.3528,
                                                                            -0.1757, -0.7796, -0.6012));
        e8::struct_light_scanner scanner("res/mannequin/set_01", "jpg", 10);
        e8::point_cloud const& pts = scanner.scan(std::vector<e8::camera>({left, right}));
        e8::point_cloud_write("scan.ply", pts);
}
