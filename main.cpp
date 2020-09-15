#include <iostream>
#include "reader.hpp"
#include "clustering.hpp"

int main(int argc, char **argv) {
    if (argc < 2) {
        std::cerr << "folder path was not set";
        return EXIT_FAILURE;
    }
    Manager::Reader R;
    Clustering::DBSCAN Algo;

    R.GetFilesFromFolder(argv[1], ".txt"); // txt extensions
    // Eigen::MatrixXd Points = R.ReadFileToEigen(R.paths[0]);
    pcl::PointCloud<pcl::PointXYZ>::Ptr Points = R.ReadFileToPCL(R.paths[0]);
    std::cout<<"good\n";
    Algo.Cluster(Points, 2, 20, "Euclidean");
    Clustering::SaveLabelsFile("../labels.txt");
}
