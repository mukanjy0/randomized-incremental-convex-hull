from flask import Flask, request, jsonify
import matplotlib.pyplot as plt
import matplotlib.animation as animation
import threading
import numpy as np

app = Flask(__name__)

points = []
hull_edges = []

lock = threading.Lock()


@app.route('/update', methods=['POST'])
def update_plot():
    global points, hull_edges
    data = request.get_json()

    with lock:
        if "points" in data:
            points = np.array(data["points"])
        if "hull_edges" in data:
            hull_edges = np.array(data["hull_edges"]).reshape(-1, 2, 2)

    return jsonify({"status": "updated"}), 200


def plot_hull():
    global points, hull_edges

    fig, ax = plt.subplots()

    def animate(_):
        ax.clear()
        with lock:
            if len(points) > 0:
                ax.scatter(points[:, 0], points[:, 1], label="Points")
            for edge in hull_edges:
                p1, p2 = edge
                ax.plot([p1[0], p2[0]], [p1[1], p2[1]], "r-", linewidth=2)
        ax.legend()
        ax.set_title("Convex Hull Visualization")

    ani = animation.FuncAnimation(fig, animate, interval=100)
    plt.show()


if __name__ == "__main__":
    threading.Thread(
        target=lambda: app.run(host="0.0.0.0", port=8000, debug=False,
                               use_reloader=False)).start()
    plot_hull()
