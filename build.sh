TOPDIR=/home/docker

#### build process ####
cd ${TOPDIR}/SDPA-BINARY128_BENCH
rm -rf sdpa-dd-cuda
git clone https://github.com/nakatamaho/sdpa-dd-cuda.git
cd sdpa-dd-cuda
aclocal ; autoconf ; automake --add-missing
autoreconf --force --install
./configure --enable-openmp=yes --enable-shared=yes
make -j4


cd ${TOPDIR}/SDPA-BINARY128_BENCH
rm -rf sdpa-dd
git clone https://github.com/nakatamaho/sdpa-dd.git
cd sdpa-dd
aclocal ; autoconf ; automake --add-missing
autoreconf --force --install
./configure --enable-openmp=yes --enable-shared=yes
make -j4

#### download SDPLIB ####
cd ${TOPDIR}/SDPA-BINARY128_BENCH
rm -rf SDPLIB
git clone https://github.com/vsdp/SDPLIB.git

PROBLEMS="gpp500-4.dat-s"

for problem in $PROBLMES; do
    cd ${TOPDIR}/SDPA-BINARY128_BENCH/sdpa-dd-cuda
    _outfile=`basename $problem`
     outfile="${_outfile%.*}.out"
    echo "./sdpa_dd -ds $problem -o $out"
done

