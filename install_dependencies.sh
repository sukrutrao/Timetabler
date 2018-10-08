#! /bin/bash
set -e

echo "Getting dependencies..."
if [ ! -d dependencies ] ; then
  mkdir dependencies
fi
cd dependencies

echo "Getting Open WBO..."
if [ ! -d open-wbo ] ; then
  git clone https://github.com/sat-group/open-wbo.git
fi
cd open-wbo
git checkout 12382e6
echo "Building Open WBO..."
LIB=open-wbo make libr
cd ..

echo "Getting YAML-CPP..."
if [ ! -d yaml-cpp-yaml-cpp-0.6.2 ] ; then
  wget https://github.com/jbeder/yaml-cpp/archive/yaml-cpp-0.6.2.tar.gz
  tar -xf yaml-cpp-0.6.2.tar.gz
fi
cd yaml-cpp-yaml-cpp-0.6.2
if [ ! -d build ] ; then
  mkdir build
fi
cd build
echo "Building YAML-CPP..."
cmake ..
make
cd ../..

echo "Getting CSVparser..."
if [ ! -d CSVparser ] ; then
  git clone https://github.com/MyBoon/CSVparser.git
fi
cd CSVparser
git checkout 540e3e2
cd ..

echo "Getting PEGTL..."
if [ ! -d PEGTL-2.7.0 ] ; then
  wget https://github.com/taocpp/PEGTL/archive/2.7.0.tar.gz
  tar -xf 2.7.0.tar.gz
fi
cd PEGTL-2.7.0
if [ ! -d build ] ; then
  mkdir build
fi
cd build
echo "Building PEGTL..."
cmake ..
make
cd ../..

if [ "$1" = "--enable-tests" ]; then
echo "Getting GoogleTest..."
if [ ! -d googletest-release-1.8.1 ] ; then
  wget https://github.com/google/googletest/archive/release-1.8.1.tar.gz
  tar -xf release-1.8.1.tar.gz
fi
cd googletest-release-1.8.1
if [ ! -d build ] ; then
  mkdir build
fi
cd build
echo "Building GooglTest..."
cmake ..
make
cd ../..
fi

echo "All dependencies installed."
cd ..

echo "Done."
