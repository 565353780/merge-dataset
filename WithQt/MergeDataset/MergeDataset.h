#ifndef MERGEDATASET_H
#define MERGEDATASET_H

#include <QString>
#include <QStringList>
#include <QDir>
#include <QFile>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>
#include <QDebug>

#include "MergeCVMat/MergeCVMat.h"

class MergeDataset
{
public:
    MergeDataset();
    ~MergeDataset();

    void init(QString source_dataset_path,
              QString target_dataset_path,
              int merge_image_num,
              int row_image_num,
              int col_image_num,
              bool row_first,
              int target_dataset_num,
              int target_image_width,
              int target_image_height
              );

    void merge_dataset();

private:
    void loadImagePath();
    void loadValidImagePath();
    void loadValidJson();

    void mergeImageWithIdx(vector<int> image_idx_vec);
    void mergeJsonWithIdx(vector<int> image_idx_vec);

    QJsonObject loadJson(QString json_path);
    void saveJson(QJsonObject json, QString file_path);

private:
    MergeCVMat merge_cvmat_;

    QString source_dataset_path_;
    QString target_dataset_path_;

    int merge_image_num_;
    int row_image_num_;
    int col_image_num_;
    bool row_first_;

    int target_dataset_num_;
    int target_image_width_;
    int target_image_height_;

    QStringList source_image_path_list_;
    QStringList valid_source_basename_list_;

    vector<QJsonObject> json_load_vec_;

    Mat merge_image_;
    QJsonObject json_merge_;
};

#endif // MERGEDATASET_H
