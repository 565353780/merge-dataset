#include "Example.h"
#include <ctime>

Example::Example()
{

}

Example::~Example()
{

}

void Example::run_example()
{
    vector<Mat> image_vec;
    image_vec.emplace_back(imread("/home/chli/public/20201222164429.jpg"));
    image_vec.emplace_back(imread("/home/chli/public/20201221194200.jpg"));
    image_vec.emplace_back(imread("/home/chli/public/20201222164429.jpg"));
    image_vec.emplace_back(imread("/home/chli/public/20201221194200.jpg"));
    image_vec.emplace_back(imread("/home/chli/public/20201222164429.jpg"));
    image_vec.emplace_back(imread("/home/chli/public/20201221194200.jpg"));
    image_vec.emplace_back(imread("/home/chli/public/20201222164429.jpg"));
    image_vec.emplace_back(imread("/home/chli/public/20201221194200.jpg"));
    image_vec.emplace_back(imread("/home/chli/public/20201222164429.jpg"));
    image_vec.emplace_back(imread("/home/chli/public/20201221194200.jpg"));
    image_vec.emplace_back(imread("/home/chli/public/20201222164429.jpg"));
    image_vec.emplace_back(imread("/home/chli/public/20201221194200.jpg"));
    image_vec.emplace_back(imread("/home/chli/public/20201222164429.jpg"));
    image_vec.emplace_back(imread("/home/chli/public/20201221194200.jpg"));
    image_vec.emplace_back(imread("/home/chli/public/20201222164429.jpg"));
    image_vec.emplace_back(imread("/home/chli/public/20201221194200.jpg"));

    MergeCVMat merge_cvmat;

    clock_t start = clock();
    Mat merged_image = merge_cvmat.mergeImage(image_vec, 3, 6, 1000, 1000, false);
    clock_t end = clock() - start;

    cout << "time:" << endl << end << endl << endl;

    for(int i = 0; i < 10; ++i)
    {
        cout << "source point " << i << " :" << endl;
        vector<double> unit_point_with_id = merge_cvmat.getUnitImagePositionAndID(1.0 * i / 10.0, 0.0);
        cout << "(" << 1.0 * i / 10.0 << "," << 0.0 << ") ->";
        cout << "(" << unit_point_with_id[0] << "," << unit_point_with_id[1] << ") : on image " << unit_point_with_id[2] << endl;
    }

    for(int i = 0; i < image_vec.size(); ++i)
    {
        cout << "point on single image " << i << " :" << endl;
        vector<double> unit_point_on_merge_image = merge_cvmat.getUnitMergeImagePositionByID(1.0, 1.0, i);
        cout << "(" << 1.0 << "," << 1.0 << ")<id:" << i << "> ->";
        cout << "(" << unit_point_on_merge_image[0] << "," << unit_point_on_merge_image[1] << ") : on merge image " << endl;
    }

    imshow("merged_image", merged_image);
    waitKey(0);
}
