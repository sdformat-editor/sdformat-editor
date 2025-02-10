# Build SDFormat14
cd libs/sdformat
apt -y install \
  $(sort -u $(find . -iname 'packages-'`lsb_release -cs`'.apt' -o -iname 'packages.apt' | tr '\n' ' '))
mkdir build
cd build
cmake ..
make install

# Build the main project
cd ../../..
mkdir build
cd build
cmake ..
make