#include <iostream>
#include <fstream>
#include <vector>
#include <list>
#include <string>
#include <set>
#include <limits>
#include <cmath>

using namespace std;

#define PI 3.14159265358979323846f

// Connection declaration
struct Connection
{
	string road_name;
	string kind;
	int location_a;
	int location_b;
	float length;
};

// Location declaration
struct Location
{
	float longitude;
	float latitude;
	float distance;
	string state;
	string place_name;
	vector<Connection*> connections;
};


vector<Location> locations; // All locations
list<Connection*> connections;	// Auxiliary list to help release the memory at the end.
								// We can't release it from our locations' connections because the
								// connections are duplicated and we would be releasing those
								// pointers twice.

// Calculates the direction from location 1 to location 2.
string findDirection(Location& l1, Location& l2);

int main() {
	// Open the files for reading
	ifstream intFile("/home/www/class/een318/intersections.txt");
	ifstream conFile("/home/www/class/een318/connections.txt");

	// Check if there was an error opening the files
	if (!intFile.is_open())
	{
		cout << "There was an error opening intersections.txt" << endl;
		return -1;
	}
	else if (!conFile.is_open())
	{
		cout << "There was an error opening connections.txt" << endl;
		return -1;
	}

	// Read the intersections
	while (!intFile.eof())
	{
		Location loc;
		intFile >> loc.longitude >> loc.latitude >> loc.distance >> loc.state;
		getline(intFile, loc.place_name);

		// Add each intersection to a position in the array
		// This allows us to access a position just by doing locations[index] where index is the location id
		locations.push_back(loc);
	}

	// Read the connections
	while (!conFile.eof())
	{
		Connection* con = new Connection();
		conFile >> con->road_name >> con->kind >> con->location_a >> con->location_b >> con->length;

		// Add the connection to each location
		locations[con->location_a].connections.push_back(con);
		locations[con->location_b].connections.push_back(con);
		
		// Add the connection to our list of connections so we can release it at the end
		connections.push_back(con);
	}

	// Ask for start intersection
	size_t startLocation;
	while (true)
	{
		cout << "Location to start: ";
		cin >> startLocation;
		
		// Validate input
		if (startLocation > 0 && startLocation < locations.size())
		{
			break;
		}

		cout << "Invalid location!" << endl;
	}

	// Ask for end intersection
	size_t endLocation;
	while (true)
	{
		cout << "Location to end: ";
		cin >> endLocation;

		// Validate input
		if (endLocation > 0 && endLocation < locations.size())
		{
			break;
		}

		cout << "Invalid location!" << endl;
	}


	// Find shortest distance using Dijkstra's algorithm

	float INF = std::numeric_limits<float>::max();
	const int numIntersections = locations.size();
	
	// Create a vector for distances and initialize all distances as infinite (INF)
	vector<float> dist(numIntersections, INF);
	vector<pair<int,Connection*> > cameFrom(numIntersections, make_pair<int, Connection*>(-1, NULL));

	// List of roads to process
	set< pair<float, int> > locationsQueue;

	// Insert the start location in and initialize its distance as 0
	locationsQueue.insert(make_pair(0.0f, startLocation));
	dist[startLocation] = 0;

	// Go through the queue till is empty - means that all shortest distances are filled
	while (!locationsQueue.empty())
	{
		// The first location in queue is the minimum distance
		int u = locationsQueue.begin()->second;
		// thus we need to remove it from the queue
		locationsQueue.erase(locationsQueue.begin());
		

		// Go through all roads that connect to this intersection
		Location& loc = locations[u];
		for (vector<Connection*>::iterator con = loc.connections.begin(); con != loc.connections.end(); ++con)
		{
			// Get location index and distance of current connection to u
			int v = (*con)->location_a == u ? (*con)->location_b : (*con)->location_a;
			float distance = (*con)->length;

			//  Check if the distance between u and v is smaller than a previously route.
			if (dist[u] + distance < dist[v])
			{
				// Check if the location is already on the queue to be processed and if it is we need to remove it
				if (dist[v] != INF)
					locationsQueue.erase(locationsQueue.find(make_pair(dist[v], v)));

				// Update distance of the v with the new smaller one
				dist[v] = dist[u] + distance;
				locationsQueue.insert(make_pair(dist[v], v));
				
				// Update the information of where we're coming from to get to the current intersection
				cameFrom[v].first = u;
				cameFrom[v].second = *con;
			}
		}
	}

	// Start at the end and trace back to the beginning taking the shortest path
	// Our path will contain the shortest path from start to finish
	list<pair<int, Connection*> > path;
	int v = endLocation;
	if (dist[v] != INF)
	{
		while (!(v == startLocation))
		{
			path.push_front(cameFrom[v]);
			v = cameFrom[v].first;
		}
	}

	// Print the path
	for (list<pair<int, Connection*> >::iterator loc = path.begin(); loc != path.end(); ++loc)
	{
		// Grab the information
		Connection* con = (*loc).second;
		int fromIndex = (*loc).first;
		int toIndex = con->location_a == (*loc).first ? con->location_b : con->location_a;
		Location& fromLoc = locations[fromIndex];
		Location& toLoc = locations[toIndex];

		// Print the information
                cout << "from intersection " << fromIndex << " take " << con->road_name << " " << con->length << " miles " << findDirection(fromLoc, toLoc) << " to intersection " << toIndex << endl;
	}

	// Free memory allocated for connections
	for (list<Connection*>::iterator it = connections.begin(); it != connections.end(); ++it)
	{
		delete *it;
	}
}

// Calculates the cardinal direction from location 1 to location 2.
string findDirection(Location& l1, Location& l2)
{
	float x1 = l1.latitude;
	float y1 = l1.longitude;
	float x2 = l2.latitude;
	float y2 = l2.longitude;

	// Calculate the angle where 0 is N, 45 is NE, 90 is E, etc.
	float angle = atan2f((y1 - y2), (x1 - x2));
	float degrees = angle * (180 / PI);

	// Map the angle to the cardinal direction
	string coordNames[] = { "N", "NE", "E", "SE", "S", "SW", "W", "NW", "N" };
	unsigned int index = (int)round(degrees / 45) % sizeof(coordNames);
	return coordNames[index];
}
