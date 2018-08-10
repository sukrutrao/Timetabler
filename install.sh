#! /bin/bash
set -e

echo "Getting dependencies..."
mkdir dependencies
cd dependencies

echo "Getting Open WBO..."
git clone https://github.com/sat-group/open-wbo.git
cd open-wbo
git checkout f193a3b
echo "Building Open WBO..."
LIB=open-wbo make libr
cd ..

echo "Getting YAML-CPP..."
wget https://github.com/jbeder/yaml-cpp/archive/yaml-cpp-0.5.3.tar.gz
tar -xf yaml-cpp-0.5.3.tar.gz
cd yaml-cpp-yaml-cpp-0.5.3
mkdir build
cd build
echo "Building YAML-CPP..."
cmake -G "Unix Makefiles" -DBUILD_SHARED_LIBS=ON ..
make -j4
sudo make install
cd ../..

echo "Getting CSVparser..."
git clone https://github.com/MyBoon/CSVparser.git
cd CSVparser
git checkout 540e3e2
cd ..

echo "Getting PEGTL..."
wget https://github.com/taocpp/PEGTL/archive/2.2.0.tar.gz
tar -xf 2.2.0.tar.gz
cd PEGTL-2.2.0
mkdir build
cd build
echo "Building PEGTL..."
cmake .. -DBUILD_SHARED_LIBS=ON
make -j4
sudo make install
cd ../..
echo "All dependencies installed."
cd ..

echo "Building Timetabler..."
export TIMETABLER_ROOT=$(pwd)
mkdir build
cd build
cmake .. -DOPEN_WBO_PATH="${TIMETABLER_ROOT}/open-wbo" -DCSVPARSER_PATH="${TIMETABLER_ROOT}/CSVparser"
make -j4
sudo make install
echo "Done."