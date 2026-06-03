
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <optional>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cmath>
#include <map>
#include <vector>
#include <tuple>
#include <queue>
#include <limits>
#include <string>
#include <algorithm>
#include <set>

#include <chrono>



#include <windows.h>

#include "tinyxml2.h"

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

using namespace std;

typedef pair<double, double> Coord;

typedef tuple<Coord, Coord, string> EdgeItem;

typedef map<Coord, vector<pair<Coord, double>>> Graph;


double haversine(const Coord& coord1, const Coord& coord2) {
    double lon1 = coord1.first;
    double lat1 = coord1.second;
    double lon2 = coord2.first;
    double lat2 = coord2.second;

    double R = 6371.0;

    double phi1 = lat1 * M_PI / 180.0;
    double phi2 = lat2 * M_PI / 180.0;
    double dphi = (lat2 - lat1) * M_PI / 180.0;
    double dlambda = (lon2 - lon1) * M_PI / 180.0;

    double a = sin(dphi / 2.0) * sin(dphi / 2.0) +
        cos(phi1) * cos(phi2) *
        sin(dlambda / 2.0) * sin(dlambda / 2.0);

    return 2.0 * R * atan2(sqrt(a), sqrt(1.0 - a));
}

string utf8_to_cp1251(const string& utf8_str) {
    int wide_len = MultiByteToWideChar(CP_UTF8, 0, utf8_str.c_str(), -1, nullptr, 0);
    if (wide_len <= 0) return utf8_str;

    wstring wide_str(wide_len, L'\0');
    MultiByteToWideChar(CP_UTF8, 0, utf8_str.c_str(), -1, &wide_str[0], wide_len);

    int cp1251_len = WideCharToMultiByte(1251, 0, wide_str.c_str(), -1, nullptr, 0, nullptr, nullptr);
    if (cp1251_len <= 0) return utf8_str;

    string cp1251_str(cp1251_len, '\0');
    WideCharToMultiByte(1251, 0, wide_str.c_str(), -1, &cp1251_str[0], cp1251_len, nullptr, nullptr);

    if (!cp1251_str.empty() && cp1251_str.back() == '\0') {
        cp1251_str.pop_back();
    }

    return cp1251_str;
}

string cp1251_to_utf8(const string& input) {
    int wlen = MultiByteToWideChar(1251, 0, input.c_str(), -1, nullptr, 0);
    if (wlen <= 0) return input;

    wstring wstr(wlen, L'\0');
    MultiByteToWideChar(1251, 0, input.c_str(), -1, &wstr[0], wlen);

    int ulen = WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), -1, nullptr, 0, nullptr, nullptr);
    if (ulen <= 0) return input;

    string utf8_str(ulen, '\0');
    WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), -1, &utf8_str[0], ulen, nullptr, nullptr);

    if (!utf8_str.empty() && utf8_str.back() == '\0') {
        utf8_str.pop_back();
    }

    return utf8_str;
}


string to_lower_simple(string s) {
    transform(s.begin(), s.end(), s.begin(), ::tolower);
    return s;
}


tuple<map<string, Coord>, vector<EdgeItem>> read_graphml(const string& file_path) {
    map<string, Coord> nodes;
    vector<EdgeItem> edges;

    tinyxml2::XMLDocument doc;

    if (doc.LoadFile(file_path.c_str()) != tinyxml2::XML_SUCCESS) {
        cerr << "Ошибка загрузки файла: " << file_path << endl;
        return make_tuple(nodes, edges);
    }

    tinyxml2::XMLElement* graphml = doc.FirstChildElement("graphml");

    if (!graphml) {
        cerr << "Не найден элемент graphml" << endl;
        return make_tuple(nodes, edges);
    }


    string key_x = "";
    string key_y = "";
    string key_name = "";

    for (tinyxml2::XMLElement* key = graphml->FirstChildElement("key"); key; key = key->NextSiblingElement("key")) {
        const char* id = key->Attribute("id");
        const char* attr_name = key->Attribute("attr.name");

        if (!id || !attr_name) continue;

        string attr = attr_name;

        if (attr == "x") {
            key_x = id;
        }
        else if (attr == "y") {
            key_y = id;
        }
        else if (attr == "name") {
            key_name = id;
        }
    }

    if (key_x.empty() || key_y.empty()) {
        cout << "Не удалось автоматически найти ключи x/y." << endl;
        cout << "Будут использованы стандартные ключи из кода преподавателя: x=d5, y=d4." << endl;

        key_x = "d5";
        key_y = "d4";
    }

    if (key_name.empty()) {
        cout << "Ключ name не найден автоматически. Будет использован d18." << endl;
        key_name = "d18";
    }

    cout << "Ключ x: " << key_x << endl;
    cout << "Ключ y: " << key_y << endl;
    cout << "Ключ name: " << key_name << endl;

    tinyxml2::XMLElement* graph = graphml->FirstChildElement("graph");

    if (!graph) {
        cerr << "Не найден элемент graph" << endl;
        return make_tuple(nodes, edges);
    }

    for (tinyxml2::XMLElement* node = graph->FirstChildElement("node"); node; node = node->NextSiblingElement("node")) {
        const char* id = node->Attribute("id");

        if (!id) continue;

        double x = 0.0;
        double y = 0.0;
        bool has_x = false;
        bool has_y = false;

        for (tinyxml2::XMLElement* data = node->FirstChildElement("data"); data; data = data->NextSiblingElement("data")) {
            const char* key = data->Attribute("key");
            const char* text = data->GetText();

            if (!key || !text) continue;

            string current_key = key;

            if (current_key == key_x) {
                x = atof(text);
                has_x = true;
            }
            else if (current_key == key_y) {
                y = atof(text);
                has_y = true;
            }
        }

        if (has_x && has_y) {
            nodes[id] = make_pair(x, y);
        }
    }

    for (tinyxml2::XMLElement* edge = graph->FirstChildElement("edge"); edge; edge = edge->NextSiblingElement("edge")) {
        const char* source_id = edge->Attribute("source");
        const char* target_id = edge->Attribute("target");

        if (!source_id || !target_id) continue;

        auto source_it = nodes.find(source_id);
        auto target_it = nodes.find(target_id);

        if (source_it == nodes.end() || target_it == nodes.end()) continue;

        string street_name = "";

        for (tinyxml2::XMLElement* data = edge->FirstChildElement("data"); data; data = data->NextSiblingElement("data")) {
            const char* key = data->Attribute("key");
            const char* text = data->GetText();

            if (!key || !text) continue;

            string current_key = key;

            if (current_key == key_name) {
                street_name = utf8_to_cp1251(text);
            }
        }

        edges.push_back(make_tuple(source_it->second, target_it->second, street_name));
    }

    cout << "Количество вершин: " << nodes.size() << endl;
    cout << "Количество ребер: " << edges.size() << endl;

    return make_tuple(nodes, edges);
}


Graph build_graph(const vector<EdgeItem>& edges) {
    Graph graph;

    for (int i = 0; i < edges.size(); i++) {
        Coord start;
        Coord end;
        string street;

        tie(start, end, street) = edges[i];

        double dist = haversine(start, end);

        graph[start].push_back(make_pair(end, dist));
        graph[end].push_back(make_pair(start, dist));
    }

    return graph;
}


tuple<vector<Coord>, double, vector<string>> dijkstra(const Graph& graph, Coord start, Coord end) {
    typedef pair<double, Coord> QueueItem;

    auto cmp = [](const QueueItem& left, const QueueItem& right) {
        return left.first > right.first;
        };

    priority_queue<QueueItem, vector<QueueItem>, decltype(cmp)> q(cmp);

    map<Coord, double> dist;
    map<Coord, Coord> parent;
    vector<Coord> visited;

    dist[start] = 0.0;
    parent[start] = start;

    q.push(make_pair(0.0, start));

    while (!q.empty()) {
        double current_distance = q.top().first;
        Coord current_node = q.top().second;
        q.pop();

        if (find(visited.begin(), visited.end(), current_node) != visited.end()) {
            continue;
        }

        visited.push_back(current_node);

        if (current_node == end) {
            break;
        }

        auto it = graph.find(current_node);

        if (it != graph.end()) {
            for (int i = 0; i < it->second.size(); i++) {
                Coord neighbor = it->second[i].first;
                double distance = it->second[i].second;

                double total_distance = current_distance + distance;

                if (dist.find(neighbor) == dist.end() || total_distance < dist[neighbor]) {
                    dist[neighbor] = total_distance;
                    parent[neighbor] = current_node;
                    q.push(make_pair(total_distance, neighbor));
                }
            }
        }
    }

    vector<Coord> path;
    vector<string> street_names;

    if (dist.find(end) == dist.end()) {
        return make_tuple(path, numeric_limits<double>::infinity(), street_names);
    }

    Coord current_node = end;

    while (true) {
        path.push_back(current_node);

        if (current_node == start) {
            break;
        }

        current_node = parent[current_node];
    }

    reverse(path.begin(), path.end());

    return make_tuple(path, dist[end], street_names);
}


pair<int, string> find_street_index(const vector<EdgeItem>& edges, const string& street_name_query) {
    string query = to_lower_simple(street_name_query);

    for (int i = 0; i < edges.size(); i++) {
        Coord start;
        Coord end;
        string name;

        tie(start, end, name) = edges[i];

        if (!name.empty()) {
            string current_name = to_lower_simple(name);

            if (current_name == query) {
                return make_pair(i, name);
            }

            if (current_name.find(query) != string::npos) {
                return make_pair(i, name);
            }
        }
    }

    return make_pair(-1, "");
}


void print_edges(const vector<EdgeItem>& edges) {
    set<string> printed;

    cout << endl;
    cout << "Список найденных улиц:" << endl;

    for (int i = 0; i < edges.size(); i++) {
        Coord start;
        Coord end;
        string name;

        tie(start, end, name) = edges[i];

        if (!name.empty() && printed.find(name) == printed.end()) {
            cout << name << endl;
            printed.insert(name);
        }
    }
}


string find_street_between(const vector<EdgeItem>& edges, Coord a, Coord b) {
    for (int i = 0; i < edges.size(); i++) {
        Coord start;
        Coord end;
        string name;

        tie(start, end, name) = edges[i];

        if ((start == a && end == b) || (start == b && end == a)) {
            return name;
        }
    }

    return "";
}


vector<string> get_street_names_for_path(const vector<EdgeItem>& edges, const vector<Coord>& path) {
    vector<string> names;

    for (int i = 0; i + 1 < path.size(); i++) {
        string name = find_street_between(edges, path[i], path[i + 1]);

        if (!name.empty()) {
            names.push_back(name);
        }
    }

    return names;
}


void visualize_path_with_network(
    const map<string, Coord>& nodes,
    const vector<EdgeItem>& edges,
    const vector<Coord>& path,
    const vector<string>& street_names = vector<string>(),
    const sf::Vector2u& figsize = sf::Vector2u(900, 900)
) {
    sf::RenderWindow window(sf::VideoMode({ figsize.x, figsize.y }), "Shortest path");

    double minX = numeric_limits<double>::max();
    double maxX = -numeric_limits<double>::max();
    double minY = numeric_limits<double>::max();
    double maxY = -numeric_limits<double>::max();

    for (auto it = nodes.begin(); it != nodes.end(); ++it) {
        double x = it->second.first;
        double y = it->second.second;

        if (x < minX) minX = x;
        if (x > maxX) maxX = x;
        if (y < minY) minY = y;
        if (y > maxY) maxY = y;
    }

    auto transform_coord = [=](const Coord& c) -> sf::Vector2f {
        double scaleX = (figsize.x - 40.0) / (maxX - minX + 1e-6);
        double scaleY = (figsize.y - 40.0) / (maxY - minY + 1e-6);

        float x = static_cast<float>((c.first - minX) * scaleX + 20.0);
        float y = static_cast<float>(figsize.y - ((c.second - minY) * scaleY + 20.0));

        return sf::Vector2f(x, y);
        };

    sf::View view = window.getDefaultView();
    sf::View default_view = view;

    while (window.isOpen()) {
        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
            }

            if (const auto* wheel = event->getIf<sf::Event::MouseWheelScrolled>()) {
                if (wheel->delta > 0) {
                    view.zoom(0.85f);  
                }
                else {
                    view.zoom(1.15f);  
                }

                window.setView(view);
            }

            if (const auto* key = event->getIf<sf::Event::KeyPressed>()) {
                if (key->code == sf::Keyboard::Key::R) {
                    view = default_view;
                    window.setView(view);
                }
            }
        }

        float move_speed = 6.0f;

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left)) {
            view.move({ -move_speed, 0.0f });
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right)) {
            view.move({ move_speed, 0.0f });
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up)) {
            view.move({ 0.0f, -move_speed });
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down)) {
            view.move({ 0.0f, move_speed });
        }

        window.setView(view);

        window.clear(sf::Color::White);

        for (int i = 0; i < edges.size(); i++) {
            Coord start;
            Coord end;
            string name;

            tie(start, end, name) = edges[i];

            sf::Vertex line[2];

            line[0].position = transform_coord(start);
            line[0].color = sf::Color(180, 180, 180);

            line[1].position = transform_coord(end);
            line[1].color = sf::Color(180, 180, 180);

            window.draw(line, 2, sf::PrimitiveType::Lines);
        }

        for (int i = 0; i + 1 < path.size(); i++) {
            sf::Vertex line[2];

            line[0].position = transform_coord(path[i]);
            line[0].color = sf::Color::Red;

            line[1].position = transform_coord(path[i + 1]);
            line[1].color = sf::Color::Red;

            window.draw(line, 2, sf::PrimitiveType::Lines);
        }

        float point_radius = 1.5f;

        for (int i = 0; i < path.size(); i++) {
            sf::CircleShape circle(point_radius);
            circle.setFillColor(sf::Color::Blue);

            sf::Vector2f point = transform_coord(path[i]);
            circle.setPosition({ point.x - point_radius, point.y - point_radius });

            window.draw(circle);
        }

        window.display();
    }
}


void check_variant_8_small_graph() {
    cout << endl;
    cout << "Проверка варианта 8 на маленьком графе:" << endl;

    const int INF = 1000000000;
    int n = 4;

    vector<vector<pair<int, int>>> graph(n);

    auto add_edge = [&](int a, int b, int w) {
        graph[a].push_back(make_pair(b, w));
        graph[b].push_back(make_pair(a, w));
        };


    add_edge(0, 1, 5); // A-B
    add_edge(0, 2, 2); // A-C
    add_edge(0, 3, 4); // A-D
    add_edge(1, 2, 3); // B-C
    add_edge(1, 3, 6); // B-D
    add_edge(2, 3, 1); // C-D

    int start = 3;  // D
    int finish = 0; // A

    vector<int> dist(n, INF);
    vector<int> parent(n, -1);

    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> q;

    dist[start] = 0;
    q.push(make_pair(0, start));

    while (!q.empty()) {
        int current_dist = q.top().first;
        int v = q.top().second;
        q.pop();

        if (current_dist != dist[v]) {
            continue;
        }

        for (int i = 0; i < graph[v].size(); i++) {
            int to = graph[v][i].first;
            int weight = graph[v][i].second;

            if (dist[v] + weight < dist[to]) {
                dist[to] = dist[v] + weight;
                parent[to] = v;
                q.push(make_pair(dist[to], to));
            }
        }
    }

    vector<int> path;

    for (int v = finish; v != -1; v = parent[v]) {
        path.push_back(v);
    }

    reverse(path.begin(), path.end());

    cout << "Кратчайшее расстояние D -> A: " << dist[finish] << endl;
    cout << "Путь: ";

    for (int i = 0; i < path.size(); i++) {
        cout << char('A' + path[i]);

        if (i + 1 < path.size()) {
            cout << " -> ";
        }
    }

    cout << endl;
}


int main() {
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    check_variant_8_small_graph();

    cout << endl;
    cout << "Загрузка графа города..." << endl;

    map<string, Coord> nodes;
    vector<EdgeItem> edges;

    tie(nodes, edges) = read_graphml("stockholm_road_network.graphml");

    if (nodes.empty() || edges.empty()) {
        cout << "Граф не загружен. Проверь файл stockholm_road_network.graphml" << endl;
        return 0;
    }


    bool show_streets = false;

    if (show_streets) {
        print_edges(edges);
        return 0;
    }


    string start_street_query = "Sveavagen";
    string end_street_query = "Valhallavagen";

    pair<int, string> start_result = find_street_index(edges, start_street_query);
    pair<int, string> end_result = find_street_index(edges, end_street_query);

    int start_index = start_result.first;
    int end_index = end_result.first;

    if (start_index == -1 || end_index == -1) {
        cout << endl;
        cout << "Не удалось найти заданную улицу для начала или конца маршрута." << endl;
        cout << "Поставь show_streets = true, запусти программу и выбери улицы из списка." << endl;
        cout << endl;

        cout << "Сейчас программа выведет первые найденные улицы:" << endl;
        print_edges(edges);

        return 0;
    }

    cout << endl;
    cout << "Стартовая улица: " << start_result.second << endl;
    cout << "Конечная улица: " << end_result.second << endl;

    Coord start_node = get<0>(edges[start_index]);
    Coord end_node = get<1>(edges[end_index]);

    Graph graph = build_graph(edges);

    vector<Coord> path;
    double distance;
    vector<string> street_names;

    auto time_start = chrono::high_resolution_clock::now();

    tie(path, distance, street_names) = dijkstra(graph, start_node, end_node);

    auto time_end = chrono::high_resolution_clock::now();

    chrono::duration<double> elapsed = time_end - time_start;

    cout << "Время работы алгоритма: " << elapsed.count() << " секунд" << endl;

    if (path.empty()) {
        cout << "Путь не найден." << endl;
        return 0;
    }

    street_names = get_street_names_for_path(edges, path);

    cout << endl;
    cout << "Найден путь длиной: " << distance << " км" << endl;

    cout << "Количество точек в маршруте: " << path.size() << endl;

    cout << endl;
    cout << "Улицы на пути:" << endl;

    if (street_names.empty()) {
        cout << "Названия улиц на пути не найдены." << endl;
    }
    else {
        for (int i = 0; i < street_names.size(); i++) {
            if (!street_names[i].empty()) {
                cout << "- " << street_names[i] << endl;
            }
        }
    }

    cout << endl;
    cout << "Открывается окно визуализации маршрута..." << endl;

    visualize_path_with_network(nodes, edges, path, street_names);

    return 0;
}