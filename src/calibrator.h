#ifndef CALIBRATOR_H
#define CALIBRATOR_H

#include "camera.h"

namespace e8
{

class if_calibrator
{
public:
        if_calibrator();
        virtual ~if_calibrator();

        virtual camera  calibrate() const = 0;
};

class epipolar_calibrator: public if_calibrator
{
public:
        epipolar_calibrator();
        virtual ~epipolar_calibrator();

        camera  calibrate() const override;
};


class checker_calibrator: public if_calibrator
{
public:
        checker_calibrator();
        virtual ~checker_calibrator();

        camera calibrate() const override;
};

}

#endif // CALIBRATOR_H
