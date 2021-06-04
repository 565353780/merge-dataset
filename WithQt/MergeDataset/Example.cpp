#include "Example.h"

Example::Example()
{

}

Example::~Example()
{

}

void Example::run_example()
{
    QString source_dataset_path = "/home/chli/chLi/Download/DeepLearning/Dataset/WaterDrop/20210412-hospital-cap/2021-04-12/MaskSpace/Yolov3Region/train_dataset";
    QString target_dataset_path = source_dataset_path + "_merge";
    int merge_image_num = 16;
    int row_image_num = 4;
    int col_image_num = 4;
    int row_first = true;
    int max_dataset_num = 10000;
    int target_image_width = 416 * 4;
    int target_image_height = 416 * 4;

    MergeDataset merge_dataset;

    merge_dataset.init(source_dataset_path,
                       target_dataset_path,
                       merge_image_num,
                       row_image_num,
                       col_image_num,
                       row_first,
                       max_dataset_num,
                       target_image_width,
                       target_image_height);

    merge_dataset.merge_dataset();
}
