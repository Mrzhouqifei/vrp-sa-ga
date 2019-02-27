#ifndef _SA_HPP_
#define _SA_HPP_

#include <cfloat>
#include <string>
#include "common.hpp"

#define COOL_RATE 0.9999
#define IMPROVE_LIMIT 10000
#define MS_PRINT_INTERVAL 500

/*
  Always accept equal or better solutions.
  If worse accept with some probability, dependent on
  how much worse and current temperature.
 */
bool accept(double new_s, double old_s, double temp) {
  if (new_s <= old_s) return true;
  return exp((old_s - new_s) / temp) > double(rand()) / RAND_MAX;
}

Solution simulated_annealing(const Graph &graph, int m, char graph_type) {
  char file_name[64];
  sprintf(file_name, "./out/sa_n%lu_m%d_%c.csv", graph.size(), m, graph_type);
  std::ofstream file(file_name);
  file << "score, time\n";

  auto before = high_resolution_clock::now();
  auto printed = before;

  Solution curr, best, next;
  curr.randomize(graph.size(), m);
  double curr_score = curr.score(graph);
  double best_score = DBL_MAX;

  int reheat = 5;
  while (reheat --> 0) {
    double temp = 5000;
    int since_improve = IMPROVE_LIMIT;
    while (since_improve --> 0) {
      if (ms_since(printed) > MS_PRINT_INTERVAL) {
        file << best_score << ", " << ms_since(before) << '\n';
        printed = high_resolution_clock::now();
      }
      temp *= COOL_RATE;
      next = curr;
      next.mutate();
      double new_score = next.score(graph);
      if (!accept(new_score, curr_score, temp))
        continue;
      curr_score = new_score;
      curr = next;
      if (curr_score >= best_score)
        continue;
      best_score = new_score;
      best = curr;
      since_improve = IMPROVE_LIMIT;
    }
  }
  return best;
}

#endif /* _SA_HPP_ */
