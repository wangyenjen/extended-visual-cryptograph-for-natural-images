#include <cstdio>
#include <random>
#include <vector>
#include <utility>
#include <cassert>
#include <algorithm>

unsigned char A2[SIZE * K][SIZE * K];
unsigned char B2[SIZE * K][SIZE * K];
unsigned char C2[SIZE * K][SIZE * K];

void greedy_fill(unsigned char A[SIZE][SIZE],
                 unsigned char B[SIZE][SIZE],
                 unsigned char C[SIZE][SIZE]) {
    static constexpr int RAND_SEED = 880301;
    static std::mt19937 gen(RAND_SEED);

    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            int a = A[i][j] / (256 / (K * K));
            int b = B[i][j] / (256 / (K * K));
            int c = C[i][j] / (256 / (K * K));
            
            std::vector<std::pair<int, int> > vs;
            
            while (c) {
                assert(a > 0 && b > 0);
                c--, a--, b--;
                vs.emplace_back(1, 1);
            }
            
            while (a > 0) {
                a--;
                vs.emplace_back(1, 0);
            }
            
            while (b > 0) {
                b--;
                vs.emplace_back(0, 1);
            }
            
            assert(a == 0 && b == 0);
            
            while ((int)vs.size() < K * K) {
                vs.emplace_back(0, 0);
            }
            
            assert((int)vs.size() == K * K);
            
            std::shuffle(vs.begin(), vs.end(), gen);
            
            for (int x = 0; x < K; x++) {
                for (int y = 0; y < K; y++) {
                    A2[K * i + x][K * j + y] = vs[x * K + y].first;
                    B2[K * i + x][K * j + y] = vs[x * K + y].second;
                }
            }
        }
    }

    for (int i = 0; i < SIZE * K; i++) {
        for (int j = 0; j < SIZE * K; j++) {
            if (A2[i][j]) {
                A2[i][j] = 255;
            }
            
            if (B2[i][j]) {
                B2[i][j] = 255;
            }
            
            if (A2[i][j] && B2[i][j]) {
                C2[i][j] = 255;
            } else {
                C2[i][j] = 0;
            }
        }
    }
}

int main(int argc, char *argv[]) {
    if (argc < 7) {
        fprintf(stderr, "usage: ./this_program image_A image_B image_C output_A2 output_B2 output_C2\n");
        return 0;
    }
    unsigned char A[SIZE][SIZE];
    unsigned char B[SIZE][SIZE];
    unsigned char C[SIZE][SIZE];

    FILE *fp = fopen(argv[1], "rb");
    fread(A, sizeof(unsigned char), SIZE * SIZE, fp);
    fclose(fp);

    fp = fopen(argv[2], "rb");
    fread(B, sizeof(unsigned char), SIZE * SIZE, fp);
    fclose(fp);

    fp = fopen(argv[3], "rb");
    fread(C, sizeof(unsigned char), SIZE * SIZE, fp);
    fclose(fp);

    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            assert(std::max(0, (int)A[i][j] + (int)B[i][j] - 255) <= C[i][j] &&
                               C[i][j] <= std::min(A[i][j], B[i][j]));
        }
    }

    greedy_fill(A, B, C);

    fp = fopen(argv[4], "wb");
    fwrite(A2, sizeof(unsigned char), SIZE * SIZE * K * K, fp);
    fclose(fp);

    fp = fopen(argv[5], "wb");
    fwrite(B2, sizeof(unsigned char), SIZE * SIZE * K * K, fp);
    fclose(fp);

    fp = fopen(argv[6], "wb");
    fwrite(C2, sizeof(unsigned char), SIZE * SIZE * K * K, fp);
    fclose(fp);

    return 0;
}
