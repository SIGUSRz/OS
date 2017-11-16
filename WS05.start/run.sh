#!/bin/bash
FAIL=0
echo "###############"
echo "make run_client"
echo "###############"
make run_client
echo "Done make"
echo "###############"
echo "Concurrency test"
echo "###############"
./bin/conversion_client 404 1 EUR 100.0 &
./bin/conversion_client 404 2 EUR 101.0 &
./bin/conversion_client 404 3 EUR 102.0 &
./bin/conversion_client 404 4 EUR 103.0 &
./bin/conversion_client 404 5 EUR 104.0

for job in `jobs -p`
do
    wait $job || let "FAIL+=1"
done

if [ "$FAIL" == "0" ];
then
    echo "Done"
else
    echo "FAIL! ($FAIL)"
fi