bash ./scripts/compile.sh $1 $2 $3
mkdir -p tmp
mkdir -p res
./tmp/adjust_dynamic_range input/{a,b,c}.raw tmp/{a,b,c}.raw
./tmp/random_filling tmp/{a,b,c}.raw res/{a,b,c}.raw