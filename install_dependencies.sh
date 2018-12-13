#! /bin/bash
set -e

OPEN_WBO_COMMIT=12382e6
YAML_CPP_VERSION=0.6.2
CSVPARSER_COMMIT=540e3e2
PEGTL_VERSION=2.7.0
GTEST_VERSION=1.8.1

OPTIONS=`getopt -o p --long enable-tests,parallel --name "install_dependencies.sh" -- "$@"`
if [ $? -ne 0 ] ; then
  echo "Invalid arguments"
  exit 1
fi
eval set -- "$OPTIONS"

PARALLEL=""
ENABLE_TESTS=0

while true; do
  case "$1" in
    -p|--parallel) PARALLEL="-j" ; shift ;;
    --enable-tests) ENABLE_TESTS=1 ; shift ;;
    --) shift ; break ;;
    *) echo "Invalid arguments" ; exit 1 ;;
  esac
done

echo "Getting dependencies..."
if [ "$PARALLEL" = "-j" ] ; then
  echo "Running make jobs in parallel"
fi

mkdir -p dependencies
cd dependencies

echo "Getting Open WBO..."
if [ ! -d open-wbo ] ; then
  git clone https://github.com/sat-group/open-wbo.git
fi
cd open-wbo
git checkout $OPEN_WBO_COMMIT
echo "Building Open WBO..."
LIB=open-wbo make libr $PARALLEL
cd ..

echo "Getting YAML-CPP..."
if [ ! -d yaml-cpp ] ; then
  wget -O yaml-cpp.tar.gz https://github.com/jbeder/yaml-cpp/archive/yaml-cpp-$YAML_CPP_VERSION.tar.gz
  tar -xf yaml-cpp.tar.gz
  rm yaml-cpp.tar.gz
  mv yaml-cpp-yaml-cpp-$YAML_CPP_VERSION yaml-cpp
fi
cd yaml-cpp
mkdir -p build
cd build
echo "Building YAML-CPP..."
cmake -DYAML_CPP_BUILD_TESTS=OFF -DYAML_CPP_BUILD_TOOLS=OFF -DYAML_CPP_BUILD_CONTRIB=OFF ..
make $PARALLEL
cd ../..

echo "Getting CSVparser..."
if [ ! -d CSVparser ] ; then
  git clone https://github.com/MyBoon/CSVparser.git
fi
cd CSVparser
git checkout $CSVPARSER_COMMIT
cd ..

echo "Getting PEGTL..."
if [ ! -d PEGTL ] ; then
  wget -O pegtl.tar.gz https://github.com/taocpp/PEGTL/archive/$PEGTL_VERSION.tar.gz
  tar -xf pegtl.tar.gz
  rm pegtl.tar.gz
  mv PEGTL-$PEGTL_VERSION PEGTL
fi

if [ "$ENABLE_TESTS" = "1" ]; then
echo "Getting GoogleTest..."
if [ ! -d googletest-release-$GTEST_VERSION ] ; then
  wget -O gtest.tar.gz https://github.com/google/googletest/archive/release-$GTEST_VERSION.tar.gz
  tar -xf gtest.tar.gz
  rm gtest.tar.gz
fi
cd googletest-release-$GTEST_VERSION
mkdir -p build
cd build
echo "Building GooglTest..."
cmake ..
make $PARALLEL
cd ../..
fi

echo "All dependencies installed."
cd ..

echo "Done."
