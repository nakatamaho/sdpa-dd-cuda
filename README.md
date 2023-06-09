# sdpa-dd-cuda
# how to build
I verified build on Ubuntu 22.04
```
rm -rf sdpa-dd-cuda
git clone https://github.com/nakatamaho/sdpa-dd-cuda.git
cd sdpa-dd-cuda
aclocal ; autoconf ; automake --add-missing
autoreconf --force --install
./configure --enable-openmp=yes --enable-shared=yes
make -j4
```
