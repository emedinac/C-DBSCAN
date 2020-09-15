/**
 * https://en.wikipedia.org/wiki/DBSCAN
 * DBSCAN clustering manually implemented from scratch.
 *
 * */
#include <iostream>
#include <fstream>
#include <Eigen/Dense>

#include <pcl/point_cloud.h>
#include <pcl/kdtree/kdtree_flann.h>
#include <Eigen/Dense>


namespace Clustering{
// clustering algorimths
    static std::vector<int> labels;
    void SaveLabelsFile(std::string filename);
    class DBSCAN{
        // Density-based spatial clustering of applications with noise (DBSCAN)
        public:
            void Cluster(Eigen::MatrixXd points, double eps, int minPts, std::string distFunc);
            void Cluster(pcl::PointCloud<pcl::PointXYZ>::Ptr points, double eps, int minPts, std::string distFunc);
            void RangeQuery(pcl::PointCloud<pcl::PointXYZ>::Ptr points, pcl::PointXYZ pointP, int K_neighbors, double eps, std::string distFunc);
        private:
            std::vector<int> pointIdxRadiusSearch;
    };
}
