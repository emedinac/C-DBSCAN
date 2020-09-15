mkdir -p bin
pushd bin
make clean
cmake .. > /dev/null 
make && printf "\n\n" && ./clustering* "../../camera_lidar/" && printf "\n\n" 
popd > /dev/null
