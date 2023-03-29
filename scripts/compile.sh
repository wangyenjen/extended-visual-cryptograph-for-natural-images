mkdir -p tmp
g++ -std=c++11 -pthread ./src/adjust_dynamic_range.cpp -o ./tmp/adjust_dynamic_range -DSIZE=$1 -DK=$3
g++ -std=c++11 -pthread ./src/random_filling.cpp -o ./tmp/random_filling -DSIZE=$1 -DK=$2