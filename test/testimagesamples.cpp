#include "src/imagesamples.h"
#include "testimagesamples.h"


test::test_image_samples::test_image_samples()
{
}

test::test_image_samples::~test_image_samples()
{
}

void
test::test_image_samples::run() const
{
        e8util::fddb_samples samples("res/FDDB_annotations", "res/FDDB_imgs", "res/FDDB_normalized");
        e8util::fddb_samples samples2("res/FDDB_annotations", "res/FDDB_normalized");
}
