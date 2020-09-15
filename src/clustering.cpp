/**
 * https://en.wikipedia.org/wiki/DBSCAN
 * DBSCAN clustering manually implemented from scratch.
 *
 * */
#include "clustering.hpp"

void Clustering::DBSCAN::Cluster(Eigen::MatrixXd points, double eps, int minPts, std::string distFunc){
    pcl::PointCloud<pcl::PointXYZ>::Ptr points_pcl;

    points_pcl->width = points.cols();
    points_pcl->height = points.rows();  //makes it organized
    points_pcl->resize(points_pcl->height*points_pcl->width);

    for(int p=0;p<points.rows();p++){
        points_pcl->points[p].x = points.row(p)[0];
        points_pcl->points[p].y = points.row(p)[1];
        points_pcl->points[p].z = points.row(p)[2];
    }
    std::cout<<"good1\n";
    Cluster(points_pcl, eps, minPts, distFunc);
    std::cout<<"good2\n";
}


void Clustering::DBSCAN::Cluster(pcl::PointCloud<pcl::PointXYZ>::Ptr points, double eps, int minPts, std::string distFunc){
//    extern std::vector<int> labels;

    int C = 0; // Cluster counter
    labels.resize(points->width);
    for (int p = 0; p < points->width; ++p){
        if (labels[p] != 0) {continue;} // Previously processed in inner loop
        RangeQuery(points, points->at(p), minPts, eps, distFunc); /* Find neighbors */
        if (pointIdxRadiusSearch.size() < minPts){
            labels[p] = -1; // Label as Noise
            continue;
        }
        C++; // next cluster label

        labels[p] = C;
        std::vector<int> SeedSet(pointIdxRadiusSearch.begin(), pointIdxRadiusSearch.end()); // Neighbors to expand
        std::cout<<p<<" =>  C:"<<C<<" init:"<<SeedSet.size()<<" --- Final:";

        int i=0; // just a counter that follow the increasing size vector.
        while(i<SeedSet.size() || SeedSet.size()==0){
            int l = SeedSet[i]; // pointer used in the algorithm
            i++;
            if (labels[l] == -1) {labels[l] = C;}
            if (labels[l] != 0) {continue;}
            labels[l] = C;
            RangeQuery(points, points->at(l), minPts, eps, distFunc); // Find neighbors
            if (pointIdxRadiusSearch.size() >= minPts){
                for(int n: pointIdxRadiusSearch) // Add new neighbors to seed set
                    if (std::find(SeedSet.begin(), SeedSet.end(),n)==SeedSet.end())
                        SeedSet.push_back(n);
            }
        }
        int mycount = std::count(labels.begin(), labels.end(), 0);
        std::cout<<SeedSet.size()<<" times:"<<mycount<<"\n";
    }
//    std::cout<<"\n\n\n\n";
//    for(int l: labels){std::cout<<l<<" ";}
//    std::cout<<C<<"\n\n\n\n";
}

// Save the labels in a Txt file using the output of the clustering algorithm
void Clustering::SaveLabelsFile(std::string filename){
    std::ofstream outFile(filename);
//    std::ostream_iterator<std::string> output_iterator(output_file, "\n");
//    std::copy(example.begin(), example.end(), output_iterator);
    outFile << "labels\n";
    for (auto &l : labels){
        if(l>0) {l--;} // simple correction for point plotting and to compare with sklearn (python) results.
        outFile << std::to_string(l) << "\n";
    }
}

// Find the closest points within a radius using KDtree.
// https://pcl.readthedocs.io/projects/tutorials/en/latest/kdtree_search.html#kdtree-search
void Clustering::DBSCAN::RangeQuery(pcl::PointCloud<pcl::PointXYZ>::Ptr points, pcl::PointXYZ pointP, int K_neighbors, double eps, std::string distFunc){
    // measure distance points and pointsQ
    pcl::KdTreeFLANN<pcl::PointXYZ> kdtree;
    kdtree.setInputCloud(points);
    // Neighbors within radius search
    std::vector<float> pointRadiusSquaredDistance;
    // distFunc WILL BE USED TO IMPLEMENT OTHER SEARCH ALGORITHMS
    kdtree.radiusSearch (pointP, eps, pointIdxRadiusSearch, pointRadiusSquaredDistance);
}
