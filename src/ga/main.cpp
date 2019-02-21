#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <cassert>

#define POP_SIZE 100

using Graph = std::vector<std::vector<double>>;

struct Solution {
    std::vector<int> sequence; // size = n
    std::vector<int> delimiters; // size = m-1
};

Solution random_solution(int n, int m) {
    Solution s = { std::vector<int>(n-1), std::vector<int>(m-1) };

    // Generate random sequence
    for (int i = 0; i < int(s.sequence.size()); ++i)
        s.sequence[i] = i+1;
    std::random_shuffle(s.sequence.begin(), s.sequence.end());

    // Set delimiters randomly
    for (int i = 0; i < int(s.delimiters.size()); ++i)
        s.delimiters[i] = rand() % n;
    std::sort(s.delimiters.begin(), s.delimiters.end());

    return s;
}

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

void mutation_swap(Solution &s) {
    int i = rand() % s.sequence.size();
    int j = i;
    while (j == i)
        j = rand() % s.sequence.size();
    std::swap(s.sequence[i], s.sequence[j]);
}

bool move_delimiter_left(Solution &s, int i) {
    if (s.delimiters[i] > 0
            && (i == 0 || s.delimiters[i-1] < s.delimiters[i])) {
        --s.delimiters[i];
        return true;
    }
    return false;
}

bool move_delimiter_right(Solution &s, int i) {
    if (s.delimiters[i] < int(s.sequence.size())
            && (i == int(s.delimiters.size())-1 || s.delimiters[i+1] > s.delimiters[i])) {
        ++s.delimiters[i];
        return true;
    }
    return false;
}

// Mutates the solution by moving a random delimiter right or left
void mutation_delimiter(Solution &s) {
    bool flipped = false; // sanity check
    int start = rand() % s.delimiters.size();
    int i = start;
    int direction = rand() % 2 == 0 ? -1 : 1;
    while (true) {
        if (direction == -1) {
            if (move_delimiter_left(s, i)) return;
            if (move_delimiter_right(s, i)) return;
        } else {
            if (move_delimiter_right(s, i)) return;
            if (move_delimiter_left(s, i)) return;
        }
        i += direction;
        if (i < 0 || i >= int(s.delimiters.size())) {
            assert(!flipped); // can not double flip
            i = start;
            direction *= -1;
            flipped = true;
        }
    }
}

void print_solution(Solution &s) {
    printf("Vehicle 0: ");
    for (int j = 0; j < s.delimiters[0]; ++j)
        printf("%d ", s.sequence[j]);
    printf("\n");

    for (int i = 1; i < int(s.delimiters.size()); ++i) {
        printf("Vehicle %d: ", i);
        for (int j = s.delimiters[i-1]; j < s.delimiters[i]; ++j)
            printf("%d ", s.sequence[j]);
        printf("\n");
    }

    printf("Vehicle %d: ", int(s.delimiters.size()));
    for (int j = s.delimiters.back(); j < int(s.sequence.size()); ++j)
        printf("%d ", s.sequence[j]);
    printf("\n\n");
}

int main() {
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
        s = random_solution(n, m);

    for (int i = 0; i < 10; ++i) {
        print_solution(population[i]);
        mutation_delimiter(population[i]);
        print_solution(population[i]);
        printf("\n\n");
    }
}