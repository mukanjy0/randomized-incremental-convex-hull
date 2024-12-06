#include <pthread.h>
#include <vector>
#include <random>
#include <algorithm>
#include "visualizer_helper.h"
#include "parallel_convex_hull.h"
#include "convex_hull.h"

const int NUM_THREADS = 8;

struct ThreadArgs {
    std::vector<P> points_subset;
    std::vector<P> partial_hull;
};

void* computePartialHull(void* args) {
    ThreadArgs* thread_args = (ThreadArgs*)args;
    thread_args->partial_hull = ConvexHull(thread_args->points_subset);
    pthread_exit(nullptr);
}

std::vector<P> mergeHulls(const std::vector<P>& hull1, const std::vector<P>& hull2) {
    // Combine points from both hulls and compute the new convex hull
    std::vector<P> combined_points = hull1;
    combined_points.insert(combined_points.end(), hull2.begin(), hull2.end());
    return ConvexHull(combined_points);
}

std::vector<P> ParallelConvexHull(std::vector<P>& points) {
    std::random_device rd3;
    std::mt19937 gen3(rd3());
    size_t n = points.size();
    if (n <= NUM_THREADS) {
        return ConvexHull(points); // Fall back to sequential if small size
    }

    std::ranges::shuffle(points, gen3);

    std::vector<std::vector<P>> subsets(NUM_THREADS);
    for (size_t i = 0; i < n; ++i) {
        subsets[i % NUM_THREADS].push_back(points[i]);
    }

    pthread_t threads[NUM_THREADS];
    ThreadArgs thread_args[NUM_THREADS];
    for (int i = 0; i < NUM_THREADS; ++i) {
        thread_args[i].points_subset = subsets[i];
        pthread_create(&threads[i], nullptr, computePartialHull, &thread_args[i]);
    }

    for (int i = 0; i < NUM_THREADS; ++i) {
        pthread_join(threads[i], nullptr);
    }

    std::vector<std::vector<P>> partial_hulls(NUM_THREADS);
    for (int i = 0; i < NUM_THREADS; ++i) {
        partial_hulls[i] = thread_args[i].partial_hull;
    }

    std::vector<P> global_hull = partial_hulls[0];
    for (int i = 1; i < NUM_THREADS; ++i) {
        global_hull = mergeHulls(global_hull, partial_hulls[i]);

        std::vector<std::pair<P, P>> vis_edges;
        for (size_t j = 0; j < global_hull.size(); ++j) {
            vis_edges.emplace_back(global_hull[j], global_hull[(j + 1) % global_hull.size()]);
        }
        //send_update(points, vis_edges);
    }

    return global_hull;
}