TOPDIR=/home/docker

PROBLEMS="arch8.dat-s control11.dat-s equalG51.dat-s gpp500-1.dat-s gpp500-4.dat-s maxG11.dat-s maxG32.dat-s maxG51.dat-s maxG55.dat-s \
mcp500-1.dat-s mcp500-4.dat-s qpG11.dat-s qpG51.dat-s ss30.dat-s theta5.dat-s theta6.dat-s thetaG11.dat-s thetaG51.dat-s truss8.dat-s"
for _problem in $PROBLEMS; do
    cd ${TOPDIR}/SDPA-BINARY128_BENCH/sdpa-dd-cuda
    _resultfile=`basename $_problem`
    resultfile="${_resultfile%.*}.result"
    ./sdpa_dd -ds $TOPDIR/SDPA-BINARY128_BENCH/SDPLIB/data/${_problem} -o $resultfile
done

