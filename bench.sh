TOPDIR=/home/docker

PROBLEMS="arch8.dat-s control11.dat-s equalG51.dat-s gpp500-1.dat-s gpp500-4.dat-s maxG11.dat-s maxG32.dat-s maxG51.dat-s maxG55.dat-s \
mcp500-1.dat-s mcp500-4.dat-s qpG11.dat-s qpG51.dat-s ss30.dat-s theta5.dat-s theta6.dat-s thetaG11.dat-s thetaG51.dat-s truss8.dat-s \
arch0.dat-s \
arch2.dat-s \
arch4.dat-s \
control1.dat-s \
control10.dat-s \
control2.dat-s \
control3.dat-s \
control4.dat-s \
control5.dat-s \
control6.dat-s \
control7.dat-s \
control8.dat-s \
control9.dat-s \
equalG51.dat-s \
gpp100.dat-s \
gpp124-1.dat-s \
gpp124-2.dat-s \
gpp124-3.dat-s \
gpp124-4.dat-s \
gpp250-1.dat-s \
gpp250-2.dat-s \
gpp250-3.dat-s \
gpp250-4.dat-s \
gpp500-2.dat-s \
gpp500-3.dat-s \
hinf1.dat-s \
hinf10.dat-s \
hinf11.dat-s \
hinf12.dat-s \
hinf13.dat-s \
hinf14.dat-s \
hinf15.dat-s \
hinf2.dat-s \
hinf3.dat-s \
hinf4.dat-s \
hinf5.dat-s \
hinf6.dat-s \
hinf7.dat-s \
hinf8.dat-s \
hinf9.dat-s \
infd1.dat-s \
infd2.dat-s \
infp1.dat-s \
infp2.dat-s \
maxG60.dat-s \
mcp100.dat-s \
mcp124-1.dat-s \
mcp124-2.dat-s \
mcp124-3.dat-s \
mcp124-4.dat-s \
mcp250-1.dat-s \
mcp250-2.dat-s \
mcp250-3.dat-s \
mcp250-4.dat-s \
mcp500-2.dat-s \
mcp500-3.dat-s \
qap10.dat-s \
qap5.dat-s \
qap6.dat-s \
qap7.dat-s \
qap8.dat-s \
qap9.dat-s \
theta1.dat-s \
theta2.dat-s \
theta3.dat-s \
theta4.dat-s \
truss1.dat-s \
truss2.dat-s \
truss3.dat-s \
truss4.dat-s \
truss5.dat-s \
truss6.dat-s \
truss7.dat-s"

for _problem in $PROBLEMS; do
    _resultfile=`basename $_problem`
    resultfile="${_resultfile%.*}.result"

    cd ${TOPDIR}/SDPA-BINARY128_BENCH/sdpa-dd-cuda
    ./sdpa_dd -ds $TOPDIR/SDPA-BINARY128_BENCH/SDPLIB/data/${_problem} -o $resultfile
    cd ${TOPDIR}/SDPA-BINARY128_BENCH/sdpa-dd
    ./sdpa_dd -ds $TOPDIR/SDPA-BINARY128_BENCH/SDPLIB/data/${_problem} -o $resultfile
    cd ${TOPDIR}/SDPA-BINARY128_BENCH/sdpa-binary128
    ./sdpa_binary128 -ds $TOPDIR/SDPA-BINARY128_BENCH/SDPLIB/data/${_problem} -o $resultfile
done

