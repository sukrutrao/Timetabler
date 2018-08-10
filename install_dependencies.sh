#! /bin/bash
set -e

echo "Getting dependencies..."
if [ ! -d dependencies ] ; then
  mkdir dependencies
fi
cd dependencies

echo "Getting Open WBO..."
if [ ! -d open-wbo ] ; then
  git clone https://github.com/GoodDeeds/open-wbo.git
fi
cd open-wbo
echo "Building Open WBO..."
LIB=open-wbo make libr
cd ..

echo "Getting YAML-CPP..."
if [ ! -d yaml-cpp-yaml-cpp-0.5.3 ] ; then
  wget https://github.com/jbeder/yaml-cpp/archive/yaml-cpp-0.5.3.tar.gz
  tar -xf yaml-cpp-0.5.3.tar.gz
fi
cd yaml-cpp-yaml-cpp-0.5.3
if [ ! -d build ] ; then
  mkdir build
fi
cd build
echo "Building YAML-CPP..."
cmake -DBUILD_SHARED_LIBS=ON ..
make -j4
cd ../..

echo "Getting CSVparser..."
if [ ! -d CSVparser ] ; then
  git clone https://github.com/MyBoon/CSVparser.git
fi
cd CSVparser
git checkout 540e3e2
cd ..

echo "Getting PEGTL..."
if [ ! -d PEGTL-2.2.0 ] ; then
  wget https://github.com/taocpp/PEGTL/archive/2.2.0.tar.gz
  tar -xf 2.2.0.tar.gz
fi
cd PEGTL-2.2.0
if [ ! -d build ] ; then
  mkdir build
fi
cd build
echo "Building PEGTL..."
cmake .. -DBUILD_SHARED_LIBS=ON
make -j4
cd ../..
echo "All dependencies installed."
cd ..

echo "Done."
