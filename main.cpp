#include <iostream>
#include <random>
#include <vector>
#include <algorithm>
#include <chrono>

using namespace std;

template<typename RNG> std::vector<int> create_drawer(int num, RNG&& rng) {
    std::vector<int> v(num);
    std::iota(v.begin(), v.end(), 1);
    std::shuffle(v.begin(), v.end(), rng);
    return std::move(v);
}

template<typename RNG> bool prisoner_rnd(const std::vector<int>& drawer, int id, RNG&& rng) {
    auto tries = drawer;
    std::shuffle(tries.begin(), tries.end(), rng);
    for (size_t i = 0; i < drawer.size()/2; i++) {
        if(drawer[tries[i]] == id)
            return true;
    }
    return false;
}

template<typename RNG> bool prisoner_best(const std::vector<int>& drawer, int id, RNG&& rng) {
    (void)rng;
    int cur = id;
    int tries = 0;
    while(id != drawer[cur]) {
       cur = drawer[cur];
       tries++;
       if(tries >= 50) {
           return false;
       }
   }
   return true;
}

template<typename RNG> int test_strategy(const std::vector<int>& drawer, bool (*strategy)(const std::vector<int>&, int, RNG), RNG&& rng)
{
    int counter = 0;
    for (size_t i = 0; i < drawer.size(); i++) {
        if (strategy(drawer, i, rng))
            counter++;
    }
    return counter;
}

int main()
{
    const int NUM = 100;
    const int COUNT = 10000;

    int count_best = 0;
    int count_rnd = 0;
    int count_100_best = 0;
    int count_100_rnd = 0;
    std::vector<int> hist_best(NUM);

    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    auto rng = std::mt19937(seed);

    for(int i = 0;i < COUNT; i++) {
        auto drawer = create_drawer(NUM, rng);
        int best = test_strategy(drawer, prisoner_best, rng);
        count_best += best;
        hist_best[best]++;
        if(best == NUM) {
            count_100_best++;
        }
        int rnd = test_strategy(drawer, prisoner_rnd, rng);
        count_rnd += rnd;
        if(rnd == 100)
            count_100_rnd++;
    }

    std::cout << "Best 100: " << (double)count_100_best/(COUNT) << "\n";
    std::cout << "Rnd 100: " << (double)count_100_rnd/(COUNT) << "\n";

    std::cout << "Best: " << (double)count_best/(NUM*COUNT) << "\n";
    std::cout << "Rnd: " << (double)count_rnd/(NUM*COUNT) << "\n";

    std::cout << "Hist: ";

    std::for_each(hist_best.begin(), hist_best.end(), [](int i) {std::cout << i << ", ";});

    std::cout << "\n";

    return 0;
}

