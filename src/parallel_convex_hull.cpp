#include <pthread.h>
#include <vector>
#include <set>
#include <map>
#include <random>
#include <algorithm>
#include "visualizer_helper.h"
#include "parallel_convex_hull.h"
#include "convex_hull.h"



const int NUM_THREADS = 8;

// Thread arguments
struct ThreadArgs {
    std::vector<P> points_subset;
    std::vector<P> partial_hull;
};

D operator*(P p1, P p2) {
    auto [x1, y1] = p1;
    auto [x2, y2] = p2;
    return x1 * y2 - x2 * y1;
}

P operator-(P p1, P p2) {
    auto [x1, y1] = p1;
    auto [x2, y2] = p2;
    return {x1 - x2, y1 - y2};
}

facet swapComponents(facet f) {
    return {f.second, f.first};
}

bool isVisible(P v, facet t) {
    auto A = t.second - t.first;
    auto B = v - t.first;
    return A * B > 0;
}

// Thread function: Compute partial convex hull for a subset of points
void* computePartialHull(void* args) {
    ThreadArgs* thread_args = (ThreadArgs*)args;
    thread_args->partial_hull = ConvexHull(thread_args->points_subset);
    pthread_exit(nullptr);
}

// Merge two convex hulls into a single hull
std::vector<P> mergeHulls(const std::vector<P>& hull1, const std::vector<P>& hull2) {
    // Combine points from both hulls and compute the new convex hull
    std::vector<P> combined_points = hull1;
    combined_points.insert(combined_points.end(), hull2.begin(), hull2.end());
    return ConvexHull(combined_points);
}

// Parallelized Convex Hull
std::vector<P> ParallelConvexHull(std::vector<P>& points) {
    size_t n = points.size();
    if (n <= NUM_THREADS) {
        return ConvexHull(points); // Fall back to sequential if small size
    }

    // Shuffle points randomly
    std::ranges::shuffle(points, g);

    // Split points into NUM_THREADS subsets
    std::vector<std::vector<P>> subsets(NUM_THREADS);
    for (size_t i = 0; i < n; ++i) {
        subsets[i % NUM_THREADS].push_back(points[i]);
    }

    // Create threads to compute partial hulls
    pthread_t threads[NUM_THREADS];
    ThreadArgs thread_args[NUM_THREADS];
    for (int i = 0; i < NUM_THREADS; ++i) {
        thread_args[i].points_subset = subsets[i];
        pthread_create(&threads[i], nullptr, computePartialHull, &thread_args[i]);
    }

    // Wait for all threads to finish
    for (int i = 0; i < NUM_THREADS; ++i) {
        pthread_join(threads[i], nullptr);
    }

    // Collect partial hulls
    std::vector<std::vector<P>> partial_hulls(NUM_THREADS);
    for (int i = 0; i < NUM_THREADS; ++i) {
        partial_hulls[i] = thread_args[i].partial_hull;
    }

    // Merge all partial hulls into a single global hull
    std::vector<P> global_hull = partial_hulls[0];
    for (int i = 1; i < NUM_THREADS; ++i) {
        global_hull = mergeHulls(global_hull, partial_hulls[i]);

        // Visualize the current merge step
        std::vector<std::pair<P, P>> vis_edges;
        for (size_t j = 0; j < global_hull.size(); ++j) {
            vis_edges.emplace_back(global_hull[j], global_hull[(j + 1) % global_hull.size()]);
        }
        //send_update(points, vis_edges);
    }

    return global_hull;
}