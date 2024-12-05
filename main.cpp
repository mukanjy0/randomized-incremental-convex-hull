#include <iostream>
#include <vector>
#include <curl/curl.h>
#include <json/json.h>

struct Point {
    double x, y;
};

void send_update(const std::string& url,
                 const std::vector<Point>& points,
                 const std::vector<std::pair<Point, Point>>& hull_edges) {
  CURL* curl;
  CURLcode res;

  curl = curl_easy_init();
  if (curl) {
    Json::Value root;
    Json::StreamWriterBuilder writer;

    // Convert points to JSON
    Json::Value points_json(Json::arrayValue);
    for (const auto& p : points) {
      Json::Value point_json;
      point_json.append(p.x);
      point_json.append(p.y);
      points_json.append(point_json);
    }

    // Convert hull edges to JSON
    Json::Value edges_json(Json::arrayValue);
    for (const auto& edge : hull_edges) {
      Json::Value edge_json(Json::arrayValue);
      Json::Value p1, p2;
      p1.append(edge.first.x);
      p1.append(edge.first.y);
      p2.append(edge.second.x);
      p2.append(edge.second.y);
      edge_json.append(p1);
      edge_json.append(p2);
      edges_json.append(edge_json);
    }

    root["points"] = points_json;
    root["hull_edges"] = edges_json;

    std::string json_data = Json::writeString(writer, root);

    // Set up libcurl POST request
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, json_data.c_str());
    curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, json_data.size());

    struct curl_slist* headers = nullptr;
    headers = curl_slist_append(headers, "Content-Type: application/json");
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

    res = curl_easy_perform(curl);
    if (res != CURLE_OK) {
      std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << std::endl;
    }

    curl_easy_cleanup(curl);
  }
}

int main() {
  std::vector<Point> points = {{0, 0}, {1, 1}, {2, 0}, {0.5, 2}};
  std::vector<std::pair<Point, Point>> hull_edges = {
          {{0, 0}, {1, 1}}, {{1, 1}, {2, 0}}, {{2, 0}, {0, 0}}
  };

  send_update("http://127.0.0.1:8000/update", points, hull_edges);

  return 0;
}