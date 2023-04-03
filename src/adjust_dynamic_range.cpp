// compile: g++ -std=c++11 -pthread
#include <tuple>
#include <cstdio>
#include <thread>
#include <vector>
#include <cstring>
#include <utility>
#include <algorithm>

struct KDTree {
    static constexpr int INF = 0x3f3f3f3f;
    static constexpr int MAX_N = 256 * 256 * 256 + 7;

    struct Node {
        Node *lc, *rc;
        int x, y, z, x1, y1, z1, x2, y2, z2;

        Node() : lc(0) , rc(0) {}

        int min_cost(int xx, int yy, int zz) {
            int dx = std::min(std::abs(xx - x1), std::abs(xx - x2));
            if (x1 <= xx && xx <= x2) {
                dx = 0;
            }
            
            int dy = std::min(std::abs(yy - y1), std::abs(yy - y2));
            if (y1 <= yy && yy <= y2) {
                dy = 0;
            }
            int dz = std::min(std::abs(zz - z1), std::abs(zz - z2));
            if (z1 <= zz && zz <= z2) {
                dy = 0;
            }
            
            return abs(dx) + abs(dy) + abs(dz);
        }
    };

    Node *tr;
    int ans_val;
    int qx , qy, qz;
    int ans_x , ans_y, ans_z;
    std::tuple<int, int, int> ps[MAX_N];
    

    void pull(Node *o) {
        o->x1 = std::min({o->x,
                         (o->lc ? o->lc->x1 : INF),
                         (o->rc ? o->rc->x1 : INF)});
        o->x2 = std::max({o->x,
                         (o->lc ? o->lc->x2 : -INF),
                         (o->rc ? o->rc->x2 : -INF)});

        o->y1 = std::min({o->y,
                         (o->lc ? o->lc->y1 : INF),
                         (o->rc ? o->rc->y1 : INF)});
        o->y2 = std::max({o->y,
                         (o->lc ? o->lc->y2 : -INF),
                         (o->rc ? o->rc->y2 : -INF)});

        o->z1 = std::min({o->z,
                         (o->lc ? o->lc->z1 : INF),
                         (o->rc ? o->rc->z1 : INF)});
        o->z2 = std::max({o->z,
                         (o->lc ? o->lc->z2 : -INF),
                         (o->rc ? o->rc->z2 : -INF)});
    }

    Node* build(int lb , int rb , int d = 0) {
        Node *o = new Node();
        if (lb == rb) {
            o->x = o->x1 = o->x2 = std::get<0>(ps[lb]);
            o->y = o->y1 = o->y2 = std::get<1>(ps[lb]);
            o->z = o->z1 = o->z2 = std::get<2>(ps[lb]);
            return o;
        }
        int mid = (lb + rb) / 2;
        std::nth_element(ps + lb ,
                         ps + mid ,
                         ps + rb + 1 ,
                         [&](const std::tuple<int , int , int> &a ,
                             const std::tuple<int , int , int> &b) {
            if (d == 2) {
                if (std::get<2>(a) != std::get<2>(b)) {
                    return std::get<2>(a) < std::get<2>(b);
                } else if (std::get<1>(a) != std::get<1>(b)) {
                    return std::get<1>(a) < std::get<1>(b);
                } else {
                    return std::get<0>(a) < std::get<0>(b);
                }
            } else if (d == 1) {
                if (std::get<1>(a) != std::get<1>(b)) {
                    return std::get<1>(a) < std::get<1>(b);
                } else if (std::get<2>(a) != std::get<2>(b)) {
                    return std::get<2>(a) < std::get<2>(b);
                } else {
                    return std::get<0>(a) < std::get<0>(b);
                }
            } else {
                if (std::get<0>(a) != std::get<0>(b)) {
                    return std::get<0>(a) < std::get<0>(b);
                } else if (std::get<1>(a) != std::get<1>(b)) {
                    return std::get<1>(a) < std::get<1>(b);
                } else {
                    return std::get<2>(a) < std::get<2>(b);
                }
            }
        });

        o->x = o->x1 = o->x2 = std::get<0>(ps[mid]);
        o->y = o->y1 = o->y2 = std::get<1>(ps[mid]);
        o->z = o->z1 = o->z2 = std::get<2>(ps[mid]);
        
        if (lb <= mid - 1) {
            o->lc = build(lb, mid - 1, (d + 1) % 3);
        }
        
        if (mid + 1 <= rb) {
            o->rc = build(mid + 1, rb, (d + 1) % 3);
        }
        
        pull(o);
        
        return o;
    }

    void query(Node *o) {
        int t = abs(qx - o->x) +
                abs(qy - o->y) +
                abs(qz - o->z);
        
        if (t < ans_val) {
            ans_x = o->x;
            ans_y = o->y;
            ans_z = o->z;
        }

        int t1 = INF;
        if (o->lc) {
            t1 = o->lc->min_cost(qx, qy, qz);
        }
        
        int t2 = INF;
        if (o->rc) {
            t2 = o->rc->min_cost(qx, qy, qz);
        }
        
        if (t1 < ans_val && t2 < ans_val) {
            if (t1 < t2) {
                query(o->lc);
                if (t2 < ans_val) {
                    query(o->rc);
                }
            } else {
                query(o->rc);
                if (t1 < ans_val) {
                    query(o->lc);
                }
            }
        } else if (t1 < ans_val) {
            query(o->lc);
        } else if (t2 < ans_val) {
            query(o->rc);
        }
    }

    void init(std::vector<std::tuple<int, int, int> > &vs) {
        int n = (int)vs.size();
        for (int i = 0; i < n; i++) {
            ps[i] = vs[i];
        }
        tr = build(0, n - 1);
    }

    std::tuple<int, int, int> query(int x, int y, int z) {
        std::tie(qx, qy, qz) = std::make_tuple(x, y, z);
        ans_val = INF;
        query(tr);
        return std::make_tuple(ans_x, ans_y, ans_z);
    }

    void release(Node *o) {
        if (!o) return;
        if (o->lc) release(o->lc);
        if (o->rc) release(o->rc);
        delete o;
    }

    void release() {
        release(tr);
    }
} kd_tree;

struct LinearTransform {
    double slope, inter;

    LinearTransform(double _slope, double _inter) :slope(_slope),
                                                   inter(_inter) {}

    double get_val(double x) {
        return slope * x + inter;
    }
};

LinearTransform get_linear_transform(double ori_mn, double ori_mx,
                                     double new_mn, double new_mx) {
    double slope = (new_mx - new_mn) / (ori_mx - ori_mn);
    double inter = new_mx - ori_mx * slope;
    return LinearTransform(slope, inter);
}

int find_best_L(unsigned char A[SIZE][SIZE],
                unsigned char B[SIZE][SIZE],
                unsigned char C[SIZE][SIZE]) {
    double mn1 = 255, mx1 = 0;
    double mn2 = 255, mx2 = 0;
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            mn1 = std::min({mn1, (double)A[i][j], (double)B[i][j]});
            mx1 = std::max({mx1, (double)A[i][j], (double)B[i][j]});
            mn2 = std::min(mn2, (double)C[i][j]);
            mx2 = std::max(mx2, (double)C[i][j]);
        }
    }

    double pdf_A[256], pdf_B[256], pdf_C[256];
    double cdf_C[256];

    double best_PKL = 0;
    int best_L = 0;

    for (int L = 0; L + K <= 255; L++) {
        LinearTransform l1 = get_linear_transform(mn1, mx1, L, L + K);
        LinearTransform l2 = get_linear_transform(mn2, mx2, 0, K);

        memset(pdf_A, 0, sizeof(pdf_A));
        memset(pdf_B, 0, sizeof(pdf_B));
        memset(pdf_C, 0, sizeof(pdf_C));
        
        memset(cdf_C, 0, sizeof(cdf_C));

        for (int i = 0; i < SIZE; i++) {
            for (int j = 0; j < SIZE; j++) {
                pdf_A[(int)(l1.get_val(A[i][j]) + 0.5)] += 1.0;
                pdf_B[(int)(l1.get_val(B[i][j]) + 0.5)] += 1.0;
                pdf_C[(int)(l2.get_val(C[i][j]) + 0.5)] += 1.0;
            }
        }
        
        for (int i = 0; i <= 255; i++) {
            pdf_A[i] /= (SIZE * SIZE);
            pdf_B[i] /= (SIZE * SIZE);
            pdf_C[i] /= (SIZE * SIZE);
        }
        
        cdf_C[0] = pdf_C[0];
        for (int i = 1; i <= 255; i++) {
            cdf_C[i] = cdf_C[i - 1] + pdf_C[i];
        }

        double PKL = 0;
        for (int t1 = L; t1 <= L + K; t1++) {
            for (int t2 = L; t2 <= L + K; t2++) {
                int lb = std::max(0, t1 + t2 - 255);
                int rb = std::min(K, std::min(t1, t2));
                
                if (lb > rb) {
                    continue;
                }
                
                double p = cdf_C[rb];
                if (lb > 0) {
                    p -= cdf_C[lb - 1];
                }

                PKL += p * pdf_A[t1] * pdf_B[t2];
            }
        }

        if (PKL > best_PKL) {
            best_PKL = PKL;
            best_L = L;
        }
    }

    return best_L;
}

int adjust_dynamic_range(unsigned char A[SIZE][SIZE],
                         unsigned char B[SIZE][SIZE],
                         unsigned char C[SIZE][SIZE]) {
    int L = find_best_L(A, B, C);

    printf("K = %d, L = %d\n", K, L);
    fflush(stdout);

    double mn1 = 255, mx1 = 0;
    double mn2 = 255, mx2 = 0;
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            mn1 = std::min({mn1, (double)A[i][j], (double)B[i][j]});
            mx1 = std::max({mx1, (double)A[i][j], (double)B[i][j]});
            mn2 = std::min(mn2, (double)C[i][j]);
            mx2 = std::max(mx2, (double)C[i][j]);
        }
    }

    LinearTransform l1 = get_linear_transform(mn1, mx1, L, L + K);
    LinearTransform l2 = get_linear_transform(mn2, mx2, 0, K);
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            A[i][j] = (unsigned char)(l1.get_val(A[i][j]) + 0.5);
            B[i][j] = (unsigned char)(l1.get_val(B[i][j]) + 0.5);
            C[i][j] = (unsigned char)(l2.get_val(C[i][j]) + 0.5);
        }
    }

    return L;
}

void build_kd_tree(int L) {
    std::vector<std::tuple<int, int, int> > ps;
    for (int t1 = L; t1 <= L + K; t1++) {
        for (int t2 = L; t2 <= L + K; t2++) {
            int lb = std::max(0, t1 + t2 - 255);
            int rb = std::min(K, std::min(t1, t2));
            
            for (int tT = lb; tT <= rb; tT++) {
                ps.emplace_back(t1, t2, tT);
            }
        }
    }

    kd_tree.init(ps);
}

void error_diffusion(unsigned char A[SIZE][SIZE], int err[SIZE][SIZE]) {
    constexpr double LEFT_TO_RIGHT_FILTER[2][3] = {{0         , 0         , 7.0 / 16.0},
                                                   {3.0 / 16.0, 5.0 / 16.0, 1.0 / 16.0}};

    constexpr double RIGHT_TO_LEFT_FILTER[2][3] = {{7.0 / 16.0, 0         , 0         },
                                                   {1.0 / 16.0, 5.0 / 16.0, 3.0 / 16.0}};

    for (int i = 0; i + 1 < SIZE; i++) {
        if (~i & 1) { // from left to right
            for (int j = 1; j + 1 < SIZE; j++) {
                for (int dx = 0; dx <= 1; dx++) {
                    for (int dy = -1; dy <= 1; dy++) {
                        A[i + dx][j + dy] +=
                            LEFT_TO_RIGHT_FILTER[dx][dy + 1] * err[i][j];
                    }
                }
            }
        } else { // from right to left
            for (int j = SIZE - 2; j >= 1; j--) {
                for (int dx = 0; dx <= 1; dx++) {
                    for (int dy = -1; dy <= 1; dy++) {
                        A[i + dx][j + dy] += 
                            RIGHT_TO_LEFT_FILTER[dx][dy + 1] * err[i][j];
                    }
                }
            }
        }
    }
}

void find_nearest(int i, int j,
                  unsigned char A[SIZE][SIZE],
                  unsigned char B[SIZE][SIZE],
                  unsigned char C[SIZE][SIZE]) {
    std::tuple<int, int, int> best = kd_tree.query(A[i][j], B[i][j], C[i][j]);
    A[i][j] = (unsigned char)std::get<0>(best);
    B[i][j] = (unsigned char)std::get<1>(best);
    C[i][j] = (unsigned char)std::get<2>(best);
}

void find_row_nearest(int i1, int i2,
                      unsigned char A[SIZE][SIZE],
                      unsigned char B[SIZE][SIZE],
                      unsigned char C[SIZE][SIZE]) {
    for (int i = i1; i < i2; i++) {
        for (int j = 0; j < SIZE; j++) {
            if (!(std::max(0, (int)A[i][j] + (int)B[i][j] - 255) <= C[i][j] && 
                              C[i][j] <= std::min(A[i][j], B[i][j]))) {
                find_nearest(i, j, A, B, C);
            }
        }
    }
}

void process(unsigned char A[SIZE][SIZE],
             unsigned char B[SIZE][SIZE],
             unsigned char C[SIZE][SIZE]) {
    static unsigned char ori_A[SIZE][SIZE];
    static unsigned char ori_B[SIZE][SIZE];
    static unsigned char ori_C[SIZE][SIZE];

    static int err_A[SIZE][SIZE];
    static int err_B[SIZE][SIZE];
    static int err_C[SIZE][SIZE];

    int L = adjust_dynamic_range(A, B, C);
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            ori_A[i][j] = A[i][j];
            ori_B[i][j] = B[i][j];
            ori_C[i][j] = C[i][j];
        }
    }

    build_kd_tree(L);

    for (int times = 0; times < 2; times++) {
        printf("times = %d\n", times);
        fflush(stdout);

        if (times) {
            error_diffusion(A, err_A);
            error_diffusion(B, err_B);
            error_diffusion(C, err_C);
        }

        std::vector<std::thread> threads;

        for (int i = 0; i < SIZE; i += 16) {
            threads.push_back(std::thread(find_row_nearest, i, i + 16, A, B, C));
        }

        for (int i = 0; i < (int)threads.size(); i++)
            threads[i].join();

        for (int i = 0; i < SIZE; i++) {
            for (int j = 0; j < SIZE; j++) {
                err_A[i][j] = A[i][j] - ori_A[i][j];
                err_B[i][j] = B[i][j] - ori_B[i][j];
                err_C[i][j] = C[i][j] - ori_C[i][j];
            }
        }
    }

    kd_tree.release();
}

int main(int argc, char *argv[]) {
    if (argc < 7) {
        fprintf(stderr, "usage: ./this_program image_A image_B image_C output_A output_B output_C\n");
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

    process(A, B, C);

    fp = fopen(argv[4], "wb");
    fwrite(A, sizeof(unsigned char), SIZE * SIZE, fp);
    fclose(fp);

    fp = fopen(argv[5], "wb");
    fwrite(B, sizeof(unsigned char), SIZE * SIZE, fp);
    fclose(fp);

    fp = fopen(argv[6], "wb");
    fwrite(C, sizeof(unsigned char), SIZE * SIZE, fp);
    fclose(fp);

    return 0;
}

