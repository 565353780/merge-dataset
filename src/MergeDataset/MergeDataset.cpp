#include "MergeDataset.h"

MergeDataset::MergeDataset()
{
    source_dataset_path_ = "";
    target_dataset_path_ = "";

    merge_image_num_ = 1;
    target_dataset_num_ = 10;
}

MergeDataset::~MergeDataset()
{

}

void MergeDataset::init(QString source_dataset_path,
                        QString target_dataset_path,
                        int merge_image_num,
                        int row_image_num,
                        int col_image_num,
                        bool row_first,
                        int target_dataset_num,
                        int target_image_width,
                        int target_image_height
                        )
{
    source_dataset_path_ = source_dataset_path;
    if(source_dataset_path_.lastIndexOf("/") != source_dataset_path_.size() - 1)
    {
        source_dataset_path_ += "/";
    }

    target_dataset_path_ = target_dataset_path;
    if(target_dataset_path_.lastIndexOf("/") != target_dataset_path_.size() - 1)
    {
        target_dataset_path_ += "/";
    }
    QDir dir;
    if(!dir.exists(target_dataset_path_))
    {
        dir.mkpath(target_dataset_path_);
    }

    merge_image_num_ = merge_image_num;
    row_image_num_ = row_image_num;
    col_image_num_ = col_image_num;
    row_first_ = row_first;

    target_dataset_num_ = target_dataset_num;
    target_image_width_ = target_image_width;
    target_image_height_ = target_image_height;

    source_image_path_list_.clear();
    valid_source_basename_list_.clear();

    json_load_vec_.clear();
    json_merge_ = QJsonObject();

    loadImagePath();
    loadValidImagePath();
    loadValidJson();
}

void MergeDataset::merge_dataset()
{
    if(merge_image_num_ < 1)
    {
        cout << "MergeDataset::merge_dataset : merge_image_num not positive." << endl;
        return;
    }
    if(target_dataset_num_ < 1)
    {
        cout << "MergeDataset::merge_dataset : target_dataset_num not positive." << endl;
        return;
    }

    for(int i = 0; i < target_dataset_num_; ++i)
    {
        vector<int> random_idx_vec;
        random_idx_vec.resize(merge_image_num_);
        for(int j = 0; j < random_idx_vec.size(); ++j)
        {
            random_idx_vec[j] = rand() % valid_source_basename_list_.size();
        }

        mergeImageWithIdx(random_idx_vec);
        mergeJsonWithIdx(random_idx_vec);

        imwrite((target_dataset_path_ + "merge_" + QString::number(i) + ".jpg").toStdString(), merge_image_);
        saveJson(json_merge_, target_dataset_path_ + "merge_" + QString::number(i) + ".json");

        cout << "MergeDataset::merge_dataset : Merge image : " << i + 1 << " / " << target_dataset_num_ << " ..." << endl;
    }
}

void MergeDataset::loadImagePath()
{
    QStringList filters;
    filters << "*.jpg";

    QDir dir;
    dir.setPath(source_dataset_path_);
    dir.setFilter(QDir::Files);
    dir.setNameFilters(filters);
    source_image_path_list_ = dir.entryList();
}

void MergeDataset::loadValidImagePath()
{
    QDir dir;

    for(int i = 0; i < source_image_path_list_.size(); ++i)
    {
        QString current_basename = source_image_path_list_[i].left(source_image_path_list_[i].size() - 4);

        QString current_json_file_name = current_basename + ".json";

        if(dir.exists(source_dataset_path_ + current_json_file_name))
        {
            valid_source_basename_list_.append(current_basename);
        }
    }
}

void MergeDataset::loadValidJson()
{
    if(valid_source_basename_list_.size() > 0)
    {
        json_load_vec_.resize(valid_source_basename_list_.size());

        for(int i = 0; i < valid_source_basename_list_.size(); ++i)
        {
            QJsonObject json_obj = loadJson(source_dataset_path_ + valid_source_basename_list_[i] + ".json");
            json_load_vec_[i] = json_obj;
        }
    }
}

void MergeDataset::mergeImageWithIdx(vector<int> image_idx_vec)
{
    vector<Mat> merge_image_vec;
    merge_image_vec.resize(image_idx_vec.size());

    for(int i = 0; i < image_idx_vec.size(); ++i)
    {
        merge_image_vec[i] = imread((source_dataset_path_ + valid_source_basename_list_[image_idx_vec[i]] + ".jpg").toStdString());
    }

    merge_image_ = merge_cvmat_.mergeImage(merge_image_vec, row_image_num_, col_image_num_, target_image_height_, target_image_width_, row_first_);
}

void MergeDataset::mergeJsonWithIdx(vector<int> image_idx_vec)
{
    int merge_image_width = merge_image_.cols;
    int merge_image_height = merge_image_.rows;

    int track_array_id = 0;

    QJsonObject Area_Obj;
    QJsonArray Area_labels_array;
    QJsonArray Area_polygons_array;
    QJsonArray Area_shape_Array;
    QJsonObject Area_track_Obj;
    QJsonArray Area_track_Array_array;

    Area_track_Obj["NextId"] = "0";

    for(int i = 0; i < image_idx_vec.size(); ++i)
    {
        QJsonArray labels = json_load_vec_[image_idx_vec[i]].value("Area").toObject().value("labels").toArray();
        QJsonArray polygons = json_load_vec_[image_idx_vec[i]].value("Area").toObject().value("polygons").toArray();
        QJsonArray shape = json_load_vec_[image_idx_vec[i]].value("Area").toObject().value("shape").toArray();

        int current_image_width = shape[0].toInt();
        int current_image_height = shape[1].toInt();

        for(int j = 0; j < labels.size(); ++j)
        {
            QString current_label = labels[j].toArray()[0].toString();

            int x_min = polygons[j].toArray()[0].toArray()[0].toInt();
            int y_min = polygons[j].toArray()[0].toArray()[1].toInt();
            int x_max = polygons[j].toArray()[2].toArray()[0].toInt();
            int y_max = polygons[j].toArray()[2].toArray()[1].toInt();

            double unit_x_min = 1.0 * x_min / current_image_width;
            double unit_y_min = 1.0 * y_min / current_image_height;
            double unit_x_max = 1.0 * x_max / current_image_width;
            double unit_y_max = 1.0 * y_max / current_image_height;

            vector<double> unit_pointmin_in_merge_image = merge_cvmat_.getUnitMergeImagePositionByID(unit_x_min, unit_y_min, i);
            vector<double> unit_pointmax_in_merge_image = merge_cvmat_.getUnitMergeImagePositionByID(unit_x_max, unit_y_max, i);

            QJsonArray Area_labels_label_array;
            Area_labels_label_array.append(current_label);
            Area_labels_array.append(Area_labels_label_array);

            QJsonArray Area_polygons_polygon_array;
            QJsonArray Area_polygons_polygon_point1_array;
            Area_polygons_polygon_point1_array.append(int(unit_pointmin_in_merge_image[0] * merge_image_width));
            Area_polygons_polygon_point1_array.append(int(unit_pointmin_in_merge_image[1] * merge_image_height));
            Area_polygons_polygon_array.append(Area_polygons_polygon_point1_array);
            QJsonArray Area_polygons_polygon_point2_array;
            Area_polygons_polygon_point2_array.append(int(unit_pointmin_in_merge_image[0] * merge_image_width));
            Area_polygons_polygon_point2_array.append(int(unit_pointmax_in_merge_image[1] * merge_image_height));
            Area_polygons_polygon_array.append(Area_polygons_polygon_point2_array);
            QJsonArray Area_polygons_polygon_point3_array;
            Area_polygons_polygon_point3_array.append(int(unit_pointmax_in_merge_image[0] * merge_image_width));
            Area_polygons_polygon_point3_array.append(int(unit_pointmax_in_merge_image[1] * merge_image_height));
            Area_polygons_polygon_array.append(Area_polygons_polygon_point3_array);
            QJsonArray Area_polygons_polygon_point4_array;
            Area_polygons_polygon_point4_array.append(int(unit_pointmax_in_merge_image[0] * merge_image_width));
            Area_polygons_polygon_point4_array.append(int(unit_pointmin_in_merge_image[1] * merge_image_height));
            Area_polygons_polygon_array.append(Area_polygons_polygon_point4_array);
            Area_polygons_polygon_array.append(Area_polygons_polygon_point1_array);
            Area_polygons_array.append(Area_polygons_polygon_array);

            Area_track_Array_array.append(track_array_id);

            Area_track_Obj["NextId"] = track_array_id;

            ++track_array_id;
        }
    }

    Area_shape_Array.append(merge_image_width);
    Area_shape_Array.append(merge_image_height);

    Area_track_Obj.insert("Array", Area_track_Array_array);

    Area_Obj.insert("labels", Area_labels_array);
    Area_Obj.insert("polygons", Area_polygons_array);
    Area_Obj.insert("shape", Area_shape_Array);
    Area_Obj.insert("track", Area_track_Obj);
    Area_Obj.insert("type", "bbox");

    json_merge_.insert("Area", Area_Obj);
}

QJsonObject MergeDataset::loadJson(QString json_path)
{
    QJsonObject json_load;

    QFile json_file(json_path);

    if(!json_file.open(QIODevice::ReadOnly))
    {
        cout << "MergeDataset::loadJson : Can't open json file :" << endl;
        cout << json_path.toStdString() << endl;
        return json_load;
    }

    QByteArray bytes = json_file.readAll();

    QJsonDocument json_doc = QJsonDocument::fromJson(bytes);

    json_load = json_doc.object();

    return json_load;
}

void MergeDataset::saveJson(QJsonObject json, QString file_path)
{
    QJsonDocument json_doc(json);

    QByteArray bytes = json_doc.toJson();

    QFile json_file(file_path);

    if(!json_file.open(QIODevice::WriteOnly))
    {
        cout << "MergeDataset::saveJson : Can't open json file." << endl;
        return;
    }

    json_file.write(bytes);

    json_file.close();
}
