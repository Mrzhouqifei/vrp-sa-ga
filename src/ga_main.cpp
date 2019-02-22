#include "common.hpp"

#define POP_SIZE 100

Solution aex(Solution &p1, Solution &p2) {
    // Create permutation mappings, from (index) -> to (value)
    std::vector<int> map1(p1.sequence.size()+1);
    std::vector<int> map2(p2.sequence.size()+1);
    map1[p1.sequence.back()] = p1.sequence[0];
    map2[p2.sequence.back()] = p2.sequence[0];
    for (int i = 1; i < int(map1.size())-1; ++i) {
        map1[p1.sequence[i-1]] = p1.sequence[i];
        map2[p2.sequence[i-1]] = p2.sequence[i];
    }

    // Perform AEX (indicies 0 and 1 are ok)
    Solution c = p1;
    std::vector<bool> added(map1.size(), false);
    added[c.sequence[0]] = added[c.sequence[1]] = true;
    bool p1_turn = false;
    for (int i = 2; i < int(p1.sequence.size()); ++i) {
        int candidate = p1_turn ? map1[c.sequence[i-1]] : map2[c.sequence[i-1]];
        if (added[candidate]) {
            // Pick random non-added node
            std::vector<int> remaining;
            remaining.reserve(c.sequence.size());
            for (int i = 1; i < int(added.size()); ++i)
                if (!added[i]) remaining.push_back(i);
            candidate = remaining[rand() % remaining.size()];
        }
        c.sequence[i] = candidate;
        added[candidate] = true;
        p1_turn = !p1_turn;
    }

    // Sanity check
    assert(added[0] == false);
    for (int i = 1; i < int(added.size()); ++i)
        assert(added[i] == true);

    return c;
}

int main() {
    srand(time(NULL));
    /*
    Graph g = {
        {0, 1, sqrt(2), 1},
        {1, 0, 1, sqrt(2)},
        {sqrt(2), 1, 0, 1},
        {1, sqrt(2), 1, 0}
    };
    */
    Graph g = {
        {0, 1, sqrt(2), 1, sqrt(2), 1},
        {1, 0, 1, sqrt(2), 1, sqrt(2)},
        {sqrt(2), 1, 0, 1, 2, sqrt(5)},
        {1, sqrt(2), 1, 0, sqrt(5), 2},
        {sqrt(2), 1, 2, sqrt(5), 0, 1},
        {1, sqrt(2), sqrt(5), 2, 1, 0}
    };

    int n = g.size(); // nodes
    int m = 3; // vehicles
    assert(n > 2);
    assert(m > 1);
    assert(m < n);

    // Generate a random population
    std::vector<Solution> population(POP_SIZE);
    for (auto &s : population)
        s.randomize(n, m);

    for (int i = 0; i < 10; ++i) {
        printf("--- Solution %d ---\n", i);
        population[i].print();
        population[i].mutate();
        population[i].print();
        printf("\n");
    }
}