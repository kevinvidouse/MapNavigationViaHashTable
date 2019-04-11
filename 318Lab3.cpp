#include <iostream>
#include <fstream>
#include <vector>
#include <list>
#include <string>

using namespace std;

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


	size_t location;
	while (true)
	{
		cout << "Location to start: ";
		cin >> location;
		
		// Validate input
		if (location > 0 && location < locations.size())
		{
			break;
		}

		cout << "Invalid location!" << endl;

	}

	do
	{
		// Print information about the current location
		Location& loc = locations[location];
		cout << "Location " << location << ", " << loc.distance << " miles from " << loc.place_name << ", " << loc.state << endl;
		cout << "roads leading away:" << endl;

		// Print all the roads for the current location
		vector<Connection*>& cons = loc.connections;
		for (size_t i = 0; i < cons.size(); ++i)
		{
			Connection* con = cons[i];
			// Make sure that the location the road leads to is different than the current location
			int dest_location = con->location_a == location ? con->location_b : con->location_a;
			cout << (i + 1) << ": " << con->road_name << ", " << con->length << " miles to location " << dest_location << endl;
		}

		// Ask user for a road to take
		cout << "take which road? ";
		size_t road;
		cin >> road;

		// End the loop when user enters 0.
		if (road == 0)
		{
			break;
		}

		// Validate input and in case of error just display the same information again
		if (road < 1 || road > cons.size())
		{
			cout << "Invalid road!" << endl;
			continue;
		}

		// If we reached this point means we need to "navigate" to a new location.
		// Because the connection connects 2 locations we need to make sure the one we're navigating
		// to is not the current position
		location = cons[road - 1]->location_a == location ? cons[road - 1]->location_b : cons[road - 1]->location_a;

	} while (true);

	// Free memory allocated for connections
	for (list<Connection*>::iterator it = connections.begin(); it != connections.end(); ++it)
	{
		delete *it;
	}
}
