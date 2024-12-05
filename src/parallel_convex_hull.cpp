#include "parallel_convex_hull.h"
#include <algorithm>
#include <random>
#include <vector>
#include <set>
#include <map>
#include <pthread.h>

// Constante para el número de hilos
const int NUM_THREADS = 8;

// Variables globales compartidas entre hilos
std::set<facet> global_facets;
std::map<facet, std::set<P>> global_C;
std::map<P, std::set<facet>> global_C_inv;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

// Operador producto cruzado
D operator*(P p1, P p2) {
    return p1.X * p2.Y - p1.Y * p2.X;
}

// Operador resta
P operator-(P p1, P p2) {
    return {p1.X - p2.X, p1.Y - p2.Y};
}

// Intercambia los componentes de una faceta
facet swapComponents(facet f) {
    return {f.second, f.first};
}

// Verifica si un punto es visible desde una faceta
bool isVisible(P v, facet t) {
    auto A = t.second - t.first;
    auto B = v - t.first;
    return A * B > 0;
}

// Estructura para los argumentos de los hilos
struct ThreadArgs {
    facet t;
    std::vector<P> points;
};

// Función que procesará las facetas en paralelo
void* processFacet(void* args) {
    ThreadArgs* thread_args = (ThreadArgs*)args;
    facet t = thread_args->t;
    std::vector<P>& points = thread_args->points;

    // Construye el mapa C para esta faceta
    std::set<P> visible_points;
    for (const auto& p : points) {
        if (isVisible(p, t)) {
            visible_points.insert(p);
        }
    }

    // Bloquea el mutex para modificar las estructuras globales
    pthread_mutex_lock(&mutex);
    global_C[t] = visible_points;
    for (const auto& p : visible_points) {
        global_C_inv[p].insert(t);
    }
    pthread_mutex_unlock(&mutex);

    pthread_exit(nullptr);
}

// Algoritmo paralelo de casco convexo incremental
std::vector<P> ParallelConvexHull(std::vector<P>& points) {
    size_t n = points.size();
    std::vector<P> hull;

    if (n <= 3) {
        return points; // Si hay 3 o menos puntos, el casco es trivial
    }

    // Baraja los puntos aleatoriamente
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(points.begin(), points.end(), g);

    // Inicializa las primeras facetas con los primeros 3 puntos
    for (int i = 0; i < 3; i++) {
        facet f = {points[i], points[(i + 1) % 3]};
        if (isVisible(points[(i + 2) % 3], f)) {
            f = swapComponents(f);
        }
        global_facets.insert(f);
    }

    // Inicializa las estructuras C y C_inv en paralelo
    std::vector<pthread_t> threads(global_facets.size());
    std::vector<ThreadArgs> thread_args(global_facets.size());
    int idx = 0;
    for (const auto& f : global_facets) {
        thread_args[idx] = {f, points};
        pthread_create(&threads[idx], nullptr, processFacet, &thread_args[idx]);
        idx++;
    }

    // Espera a que terminen todos los hilos
    for (pthread_t& thread : threads) {
        pthread_join(thread, nullptr);
    }

    // Itera sobre los puntos restantes
    for (size_t i = 3; i < n; ++i) {
        P vi = points[i];
        auto R = global_C_inv[vi]; // Facetas visibles desde el punto vi

        // Encuentra las aristas límite (boundary)
        std::set<ridge> boundary;
        for (const auto& t : R) {
            ridge r1 = t.first;
            ridge r2 = t.second;
            if (boundary.find(r1) != boundary.end()) {
                boundary.erase(r1);
            } else {
                boundary.insert(r1);
            }
            if (boundary.find(r2) != boundary.end()) {
                boundary.erase(r2);
            } else {
                boundary.insert(r2);
            }
        }

        // Procesa cada arista límite en paralelo
        std::vector<pthread_t> boundary_threads(boundary.size());
        idx = 0;
        for (const auto& r : boundary) {
            // Aquí puedes implementar lógica adicional si quieres procesar cada límite en paralelo
            idx++;
        }

        // Actualiza las facetas visibles y reconstruye los mapas C y C_inv
        for (const auto& t : R) {
            global_facets.erase(t);
            for (const auto& vis_p : global_C[t]) {
                global_C_inv[vis_p].erase(t);
            }
            global_C.erase(t);
        }
    }

    // Convierte las facetas finales en un vector de puntos
    for (const auto& f : global_facets) {
        if (hull.empty() || hull.back() != f.first) {
            hull.push_back(f.first);
        }
        hull.push_back(f.second);
    }

    return hull;
}