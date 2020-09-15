#include <pcl/point_cloud.h>
#include <pcl/kdtree/kdtree_flann.h>

#include <vector>
#include <string>
#include <filesystem>
#include <fstream>
#include <streambuf>
#include <algorithm>
#include <iterator>

#include <Eigen/Dense>

namespace fs = std::filesystem;
namespace Manager{
// read file
    template<char delimiter>
        class WordDelimitedBy : public std::string {};
    class Reader{
        // read files in folder
        public:
            std::vector<fs::path> paths;
            int curr_rows = 0, curr_cols = 0;

            void GetFilesFromFolder(std::string root, std::string ext);
            Eigen::MatrixXd ReadFileToEigen(std::string file);
            pcl::PointCloud<pcl::PointXYZ>::Ptr ReadFileToPCL(std::string file);
        private:
            static bool BothAreSpaces(char lhs, char rhs);
            int count_character(std::string s, char target_shar);
            std::string PreprocessString(std::string file);
    };
}








