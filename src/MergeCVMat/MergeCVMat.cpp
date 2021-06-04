#include "MergeCVMat.h"

MergeCVMat::MergeCVMat()
{

}

MergeCVMat::~MergeCVMat()
{

}

bool MergeCVMat::checkImageVecValid(vector<Mat>& image_vec)
{
    if(image_vec.size() == 0)
    {
        cout << "MergeCVMat::checkImageVecValid : No image input." << endl;
        return false;
    }

    int common_type = image_vec[0].type();

    if(image_vec.size() > 1)
    {
        for(int i = 1; i < image_vec.size(); ++i)
        {
            if(image_vec[i].type() != common_type)
            {
                cout << "MergeCVMat::checkImageVecValid : Image types mixed." << endl;
                cout << "MergeCVMat::checkImageVecValid : Image 0 and " << i << " have different types." << endl;
                cout << "MergeCVMat::checkImageVecValid : Image 0's type : " << image_vec[0].type() << " ; image " << i << "'s type : " << image_vec[i].type() << endl;
                return false;
            }
        }
    }

    return true;
}

Mat MergeCVMat::mergeImage(vector<Mat>& image_vec, int row_image_num, int col_image_num, int merge_image_rows, int merge_image_cols, bool row_first)
{
    if(!checkImageVecValid(image_vec))
    {
        return Mat();
    }

    row_image_num_ = row_image_num;
    col_image_num_ = col_image_num;
    merge_image_rows_ = merge_image_rows;
    merge_image_cols_ = merge_image_cols;
    row_first_ = row_first;

    updateImagePosition(image_vec);

    Mat merge_image = Mat::zeros(unit_image_rows_sum_, unit_image_cols_sum_, image_vec[0].type());

    for(int i = 0; i < image_position_vector_.size(); ++i)
    {
        Mat dstMat = merge_image(Rect(image_position_vector_[i].unit_col_min, image_position_vector_[i].unit_row_min, image_position_vector_[i].unit_cols, image_position_vector_[i].unit_rows));

        Mat scaled_image;

        resize(image_vec[i], scaled_image, Size(image_position_vector_[i].unit_cols, image_position_vector_[i].unit_rows));

        scaled_image.colRange(0, image_position_vector_[i].unit_cols).copyTo(dstMat);
    }

    return merge_image;

//    if(unit_image_rows_sum_ < image_rows_sum_ && unit_image_cols_sum_ < image_cols_sum_)
//    {
//        Mat merge_image = Mat::zeros(unit_image_rows_sum_, unit_image_cols_sum_, image_vec[0].type());

//        for(int i = 0; i < image_position_vector_.size(); ++i)
//        {
//            Mat dstMat = merge_image(Rect(image_position_vector_[i].unit_col_min, image_position_vector_[i].unit_row_min, image_position_vector_[i].unit_cols, image_position_vector_[i].unit_rows));

//            Mat scaled_image;

//            resize(image_vec[i],scaled_image, Size(image_position_vector_[i].unit_cols, image_position_vector_[i].unit_rows));

//            scaled_image.colRange(0, image_position_vector_[i].unit_cols).copyTo(dstMat);
//        }

//        return merge_image;
//    }
//    else
//    {
//        Mat merge_image = Mat::zeros(image_rows_sum_, image_cols_sum_, image_vec[0].type());

//        for(int i = 0; i < image_position_vector_.size(); ++i)
//        {
//            Mat dstMat = merge_image(Rect(image_position_vector_[i].col_min, image_position_vector_[i].row_min, image_position_vector_[i].cols, image_position_vector_[i].rows));

//            image_vec[i].colRange(0, image_position_vector_[i].cols).copyTo(dstMat);
//        }
//        resize(merge_image, merge_image, Size(unit_image_rows_sum_, unit_image_cols_sum_));

//        return merge_image;
//    }
}

vector<double> MergeCVMat::getUnitImagePositionAndID(double x, double y)
{
    vector<double> unit_image_position_with_id_vec;
    unit_image_position_with_id_vec.resize(3);
    for(int i = 0; i < 3; ++i)
    {
        unit_image_position_with_id_vec[i] = -1;
    }

    if(x < 0 || x > 1 || y < 0 || y > 1)
    {
        cout << "MergeCVMat::getUnitImagePositionAndID : x or y out of range." << endl;
        return unit_image_position_with_id_vec;
    }

    int row = int(y * unit_image_rows_sum_);
    int col = int(x * unit_image_cols_sum_);

    for(int i = 0; i < image_position_vector_.size(); ++i)
    {
        if(isInImageWithID(row, col, i))
        {
            int row_in_image = row - image_position_vector_[i].unit_row_min;
            int col_in_image = col - image_position_vector_[i].unit_col_min;

            double unit_x_in_image = 1.0 * col_in_image / image_position_vector_[i].unit_cols;
            double unit_y_in_image = 1.0 * row_in_image / image_position_vector_[i].unit_rows;

            unit_image_position_with_id_vec[0] = unit_x_in_image;
            unit_image_position_with_id_vec[1] = unit_y_in_image;
            unit_image_position_with_id_vec[2] = i;

            return unit_image_position_with_id_vec;
        }
    }

    cout << "MergeCVMat::getUnitImagePositionAndID : Don't find this point in image." << endl;
    return unit_image_position_with_id_vec;
}

vector<double> MergeCVMat::getUnitMergeImagePositionByID(double x, double y, int id)
{
    vector<double> unit_merge_image_position;
    unit_merge_image_position.resize(2);
    for(int i = 0; i < 2; ++i)
    {
        unit_merge_image_position[i] = -1;
    }

    if(x < 0 || x > 1 || y < 0 || y > 1)
    {
        cout << "MergeCVMat::getUnitMergeImagePositionByID : x or y out of range." << endl;
        return unit_merge_image_position;
    }

    if(id < 0 || id >= image_position_vector_.size())
    {
        cout << "MergeCVMat::getUnitMergeImagePositionByID : id out of range." << endl;
        return unit_merge_image_position;
    }

    int row = int(y * image_position_vector_[id].unit_rows);
    int col = int(x * image_position_vector_[id].unit_cols);

    int row_in_merge_image = row + image_position_vector_[id].unit_row_min;
    int col_in_merge_image = col + image_position_vector_[id].unit_col_min;

    double unit_x_in_merge_image = 1.0 * col_in_merge_image / unit_image_cols_sum_;
    double unit_y_in_merge_image = 1.0 * row_in_merge_image / unit_image_rows_sum_;

    unit_merge_image_position[0] = unit_x_in_merge_image;
    unit_merge_image_position[1] = unit_y_in_merge_image;

    return unit_merge_image_position;
}

void MergeCVMat::updateImagePosition(vector<Mat>& image_vec)
{
    image_rows_sum_ = 0;
    image_cols_sum_ = 0;
    unit_image_rows_sum_ = 0;
    unit_image_cols_sum_ = 0;

    image_position_vector_.resize(image_vec.size());

    int current_row_idx = 0;
    int current_col_idx = 0;

    int last_row_sum = 0;
    int last_col_sum = 0;
    int current_row_sum = 0;
    int current_col_sum = 0;

    for(int i = 0; i < image_vec.size(); ++i)
    {
        image_position_vector_[i].row_idx = current_row_idx;
        image_position_vector_[i].col_idx = current_col_idx;

        if(row_first_)
        {
            ++current_row_idx;

            last_row_sum = current_row_sum;

            current_row_sum += image_vec[i].rows;

            int current_col_add = last_col_sum + image_vec[i].cols;

            if(current_col_add > current_col_sum)
            {
                current_col_sum = current_col_add;
            }

            image_position_vector_[i].row_min = last_row_sum;
            image_position_vector_[i].col_min = last_col_sum;
            image_position_vector_[i].row_max = current_row_sum;
            image_position_vector_[i].col_max = current_col_add;

            if(current_row_sum > image_rows_sum_)
            {
                image_rows_sum_ = current_row_sum;
            }
            if(current_col_sum > image_cols_sum_)
            {
                image_cols_sum_ = current_col_sum;
            }

            if(current_row_idx % row_image_num_ == 0)
            {
                current_row_idx = 0;
                ++current_col_idx;

                current_row_sum = 0;
                last_col_sum = current_col_sum;
            }
        }
        else
        {
            ++current_col_idx;

            last_col_sum = current_col_sum;

            current_col_sum += image_vec[i].cols;

            int current_row_add = last_row_sum + image_vec[i].rows;

            if(current_row_add > current_row_sum)
            {
                current_row_sum = current_row_add;
            }

            image_position_vector_[i].row_min = last_row_sum;
            image_position_vector_[i].col_min = last_col_sum;
            image_position_vector_[i].row_max = current_row_add;
            image_position_vector_[i].col_max = current_col_sum;

            if(current_row_sum > image_rows_sum_)
            {
                image_rows_sum_ = current_row_sum;
            }
            if(current_col_sum > image_cols_sum_)
            {
                image_cols_sum_ = current_col_sum;
            }

            if(current_col_idx % col_image_num_ == 0)
            {
                current_col_idx = 0;
                ++current_row_idx;

                current_col_sum = 0;
                last_row_sum = current_row_sum;
            }
        }
    }

    double scale = 1.0;

    if(merge_image_rows_ > 0 && merge_image_cols_ > 0)
    {
        double row_scale = 1.0 * merge_image_rows_ / image_rows_sum_;
        double col_scale = 1.0 * merge_image_cols_ / image_cols_sum_;

        scale = row_scale;
        if(row_scale > col_scale)
        {
            scale = col_scale;
        }

        for(int i = 0; i < image_position_vector_.size(); ++i)
        {
            image_position_vector_[i].updateUnitPosition(scale);
        }
    }
    else
    {
        for(int i = 0; i < image_position_vector_.size(); ++i)
        {
            image_position_vector_[i].updateUnitPosition(scale);
        }
    }

    unit_image_rows_sum_ = int(scale * image_rows_sum_);
    unit_image_cols_sum_ = int(scale * image_cols_sum_);
}

bool MergeCVMat::isInImageWithID(int row, int col, int id)
{
    if(row >= image_position_vector_[id].unit_row_min
            && row <= image_position_vector_[id].unit_row_max
            && col >= image_position_vector_[id].unit_col_min
            && col <= image_position_vector_[id].unit_col_max)
    {
        return true;
    }

    return false;
}
