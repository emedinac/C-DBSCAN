
#include "reader.hpp"


// subfunction to find two spaces joined "  "
bool Manager::Reader::BothAreSpaces(char lhs, char rhs) { return (lhs == rhs) && (lhs == ' '); }
// subfunction to count number of ocurrences of a defined character.
int Manager::Reader::count_character(std::string s, char target_shar){
    int count = 0;
    for (int i = 0; i < s.size(); i++)
        if (s[i] == target_shar) count++;
    return count;
}

// Given a txt path, load and convert the string into a number PCL matrix
// return a PCL matrix of NxM, for point clouds: N is the number of points and M the number of dimensions
// interesting idea: https://stackoverflow.com/questions/45795526/generating-a-point-cloud-from-another-labeld-point-cloud
pcl::PointCloud<pcl::PointXYZ>::Ptr Manager::Reader::ReadFileToPCL(std::string file) {
    std::string str_vals = PreprocessString(file);

    pcl::PointCloud<pcl::PointXYZ>::Ptr Points (new pcl::PointCloud<pcl::PointXYZ>);
    Points->width = curr_rows;
    Points->height = curr_cols;  //makes it organized
    Points->resize(Points->height*Points->width);

    std::istringstream stm(str_vals);
    int iter=0;
    std::string str;
    while( stm >> str ){ // optimize this function.. not elegant!!!
        Points->points[iter].x = std::stod(str);
        stm >> str;
        Points->points[iter].y = std::stod(str);
        stm >> str;
        Points->points[iter].z = std::stod(str);
        iter++;
    }
    return Points;
};

// Given a txt path, load and convert the string into a number Eigen matrix
// return an Eigen matrix of NxM, for point clouds: N is the number of points and M the number of dimensions
Eigen::MatrixXd Manager::Reader::ReadFileToEigen(std::string file) {
    std::string str_vals = PreprocessString(file);

    Eigen::MatrixXd Points(curr_rows,curr_cols);

    std::istringstream stm(str_vals);
    int iter=0;
    std::string str;
    while( stm >> str ){
        Points(iter/curr_cols, iter%curr_cols) = std::stod(str);
        iter++;
    }
    return Points;
};

// Pre process the text input, removing spaces and commas, and curr_rows and curr_cols are updated
// to the current matrix size. (not sure if this is good yet)
std::string Manager::Reader::PreprocessString(std::string file){
    std::ifstream inFile;
    std::stringstream buffer;
    std::string str_vals;

    inFile.open(file); //open the input file
    buffer << inFile.rdbuf();
    str_vals = buffer.str(); // convert to string
    if(str_vals.find_first_of(",")){ // remove commas and spaces
        std::replace(str_vals.begin(), str_vals.end(), ',', ' ');
        std::string::iterator new_end = std::unique(str_vals.begin(), str_vals.end(), BothAreSpaces);
        str_vals.erase(new_end, str_vals.end());
    }
    curr_rows = count_character(str_vals, '\n');
    curr_cols = count_character(str_vals.substr(0, str_vals.find_first_of("\n")), ' ')+1;
    std::cout<<"cols: "<<curr_cols<<" rows: "<<curr_rows<<"\n";
    return str_vals;
}

// set a folder that contains the files and set the extension (Ex: ".txt").
// return a path vector in the public class.
void Manager::Reader::GetFilesFromFolder(std::string root, std::string ext) {
    if (fs::exists(root) && fs::is_directory(root))
    {
        for (const auto & entry : fs::directory_iterator(root))
            if (fs::is_regular_file(entry) && entry.path().extension() == ext)
                paths.emplace_back( entry.path() );
    }
};
