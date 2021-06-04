#ifndef MERGECVMAT_H
#define MERGECVMAT_H

#include <iostream>
#include <fstream>

#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

class MergeCVMat
{
public:
    struct ImagePosition
    {
        int row_idx;
        int col_idx;

        int row_min;
        int col_min;
        int row_max;
        int col_max;

        int unit_row_min;
        int unit_col_min;
        int unit_row_max;
        int unit_col_max;

        int rows;
        int cols;
        int unit_rows;
        int unit_cols;

        void updateUnitPosition(double scale)
        {
            unit_row_min = int(scale * row_min);
            unit_row_max = int(scale * row_max);
            unit_col_min = int(scale * col_min);
            unit_col_max = int(scale * col_max);

            rows = row_max - row_min;
            cols = col_max - col_min;
            unit_rows = unit_row_max - unit_row_min;
            unit_cols = unit_col_max - unit_col_min;
        }
    };

public:
    MergeCVMat();
    ~MergeCVMat();

    bool checkImageVecValid(vector<Mat>& image_vec);

    Mat mergeImage(vector<Mat>& image_vec, int row_image_num, int col_image_num, int merge_image_rows=-1, int merge_image_cols=-1, bool row_first=false);

    vector<double> getUnitImagePositionAndID(double x, double y);

    vector<double> getUnitMergeImagePositionByID(double x, double y, int id);

private:
    void updateImagePosition(vector<Mat>& image_vec);

    bool isInImageWithID(int row, int col, int id);

private:
    int row_image_num_;
    int col_image_num_;
    int merge_image_rows_;
    int merge_image_cols_;
    bool row_first_;

    int image_rows_sum_;
    int image_cols_sum_;
    int unit_image_rows_sum_;
    int unit_image_cols_sum_;

    vector<ImagePosition> image_position_vector_;
};

#endif // MERGECVMAT_H
