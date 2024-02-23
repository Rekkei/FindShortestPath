#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_map>
#include <queue>
#include <limits>
#include <string>
#include <sstream>
#include <algorithm>

using namespace std;

class Graph {
	public:

		Graph(ifstream &adjacentCities, ifstream &cityDistances);

		void sortNeighbors(const string &x);

		vector<string> neighbors;

		void add_vertex(const string &x);

		void add_edge(const string &x, const string &y);

		int finder(const string &x, const string &y);

		int get_edge_value(const string &x, const string &y);

		void set_edge_value(const string &x, const string &y, int v);

		vector<string> dijkstra(const string& start, const string& end);

		int getCityIndex(const string& city);

	private:
		int V = 81;
		int E = 81;
		vector<string> labelsOfNodes;
		vector<vector<int>> adj;

};

Graph::Graph(ifstream &adjacentCities, ifstream &cityDistances) {
	adj = vector<vector<int>>(V, vector<int>(V, 0));
	string adjacentLine;

	while (getline(adjacentCities, adjacentLine)) {
		int n = adjacentLine.length();
		int    count = 0;
		string firstCity = "";
		string neighbor  = "";

		for (int i = 0; i < n; i++) {
			if (adjacentLine[i] == ',' || i == (n - 1)) {
				add_vertex(firstCity); //Fill labelsofnodes with city
				firstCity = "";

				break;
			} else {
				firstCity += adjacentLine[i];
			}
		}
	}

	for(int i=0; i<V; i++) { //Fill adj vector
		for(int k=0; k<E; k++) {
			add_edge(labelsOfNodes[i],labelsOfNodes[k]);
			set_edge_value(labelsOfNodes[i],labelsOfNodes[k],finder(labelsOfNodes[i],labelsOfNodes[k]));

		}
	}

}


int Graph::finder(const string &x, const string &y) { //Find value of distances between cities using cityDistance file
	int indexX = getCityIndex(x);
	int indexY = getCityIndex(y);
	int stoiw=0;

	string  filePath = "cityDistances.txt";
	ifstream file(filePath);
	string distanceLine;
	getline(file,distanceLine);

	int current_index = 0;

	while (getline(file,distanceLine)) {
		int    count = 0;
		string word  = "";
		if (current_index == indexX) {

			file.close();
			for (int i = 0; i < distanceLine.length(); i++) {

				if (distanceLine[i] == ';') {

					if (count == indexY+1) {

						i++;
						while(distanceLine[i]!=';') {
							word += distanceLine[i];
							i++;
						}

						stoiw=stoi(word);

						break;
					}

					else {

						count++;
					}

				}

			}

		}
		++current_index;
	}
	return stoiw;
}
void Graph::sortNeighbors(const string &city) {
	 int indexX = getCityIndex(city);
    string filePath = "adjacentCities.txt";
    ifstream file(filePath);
    string line;
    int count = 0;
    int current_index = 0;

    while (getline(file, line)) {
        int n = line.length();
        int count = 0, countk = 0;
        string word = "";

        if (current_index == indexX) {
            file.close();
            for (int i = 0; i < n; i++) {
                if (line[i] == ',') {
                    if (count >= 1) {
                        neighbors.push_back(word);
                    }
                    word = "";
                    count++;
                } else if (i == (n - 1)) {
                    word += line[i];
                    neighbors.push_back(word);
                } else {
                    word += line[i];
                }
            }
        }
        ++current_index;
    }
	
    sort(neighbors.begin(), neighbors.end(), [this, &city](const string &a, const string &b) {
        return get_edge_value(city, a) < get_edge_value(city, b);
    });

}

void Graph::add_vertex(const string &x) {
	labelsOfNodes.push_back(x);

}

void Graph::add_edge(const string &x, const string &y) {

	int indexX = getCityIndex(x);
	int indexY = getCityIndex(y);

	if (indexX != -1 && indexY != -1) {

		adj[indexX][indexY] = 1;
		adj[indexY][indexX] = 1;

	}

}

int Graph::getCityIndex(const string& city) {
	auto it = find(labelsOfNodes.begin(), labelsOfNodes.end(), city);
	if (it != labelsOfNodes.end()) {
		return distance(labelsOfNodes.begin(), it);
	}
	return -1;
}

int Graph::get_edge_value(const string &x, const string &y) {
	int indexX = getCityIndex(x);
	int indexY = getCityIndex(y);

	return adj[indexX][indexY];
}

void Graph::set_edge_value(const string &x, const string &y, int v) {
	int indexX = getCityIndex(x);
	int indexY = getCityIndex(y);

	adj[indexX][indexY] = v;
	adj[indexY][indexX] = v;


}

vector<string> Graph::dijkstra(const string &startCity, const string &targetCity) {
    priority_queue<pair<int, string>, vector<pair<int, string>>, greater<pair<int, string>>> pq;
    vector<int> dist(V, INT_MAX);
    vector<string> parent(V, "");

    int startIndex = getCityIndex(startCity);
    int targetIndex = getCityIndex(targetCity);

    dist[startIndex] = 0;
    pq.push({0, startCity});

    while (!pq.empty()) {
        string currentCity = pq.top().second;
        pq.pop();

        int currentIndex = getCityIndex(currentCity);

        
        neighbors.clear();
        sortNeighbors(currentCity);//Use the sorted neighbors

        for (const string &neighbor : neighbors) {
            int neighborIndex = getCityIndex(neighbor);
            if (dist[currentIndex] + get_edge_value(currentCity, neighbor) < dist[neighborIndex]) {
                dist[neighborIndex] = dist[currentIndex] + get_edge_value(currentCity, neighbor);
                parent[neighborIndex] = currentCity;
                pq.push({dist[neighborIndex], neighbor});
            }
        }
    }

    		int total=0;

    vector<string> path;
    string city = targetCity;
    while (getCityIndex(city) != -1) {//Reconstruct the shortest path
        path.push_back(city);
        city = parent[getCityIndex(city)];
    }
    reverse(path.begin(), path.end());
	neighbors.clear();
	
	cout<<dist[targetIndex];
    return path;
}
int main() {

	ifstream adjacentCities("adjacentCities.txt");
	ifstream cityDistances("cityDistances.txt");

	Graph graph(adjacentCities, cityDistances);
	int ch=0,k=0;
	string city,scity;
	do {
		cout<<"1)Enter city(or select)"<<"\n2)Print selected(or entered) city"<<"\n3)List k closest  cities (to selected city)"<<"\n4)Find shortest path to"<<"\n5)Exit"<<endl;
		cin>>ch;
		switch(ch) {
			
			case 1:
				cin>>city;
				break;
			case 2:
				if(city.empty()) {
					cout<<"Please enter a city name"<<endl;
					break;
				}
				cout<<"Selected city:";
				cout<<city<<endl;;
				break;
			case 3:
				if(city.empty()) {
					cout<<"Please enter a city name"<<endl;
					break;
				}
				graph.sortNeighbors(city);
				cout<<"Please select a number(k)(Maximum neighbor number is ("<< graph.neighbors.size()<<")):";
				cin>>k;
				if(k>graph.neighbors.size()){
					cout<<"Out of number please select again."<<endl;
					graph.neighbors.clear();
					break;
				}	
				for(int i=0;i<k;i++){
					cout<<graph.neighbors[i]<<" ";
				}
				cout<<endl;
				graph.neighbors.clear();
				break;
			case 4:
				cin>>scity;
				vector<string> shortest_path = graph.dijkstra(city, scity);
				
				// Print the shortest path
				cout << "Shortest path: ";
				for (const string& dcity : shortest_path) {
					cout << dcity << " ";
				}
				cout << endl;


		}
	} while(ch!=5);


	adjacentCities.close();
	cityDistances.close();

	return 0;
}