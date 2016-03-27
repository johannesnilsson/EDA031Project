/* myserver.cc: sample server program */
#include "server.h"
#include "connection.h"
#include "connectionclosedexception.h"

#include <memory>
#include <iostream>
#include <string>
#include <stdexcept>
#include <cstdlib>

using namespace std;

/*
 * Read an integer from a client.
 */
int readNumber(const shared_ptr<Connection>& conn) {
	unsigned char byte1 = conn->read();
	unsigned char byte2 = conn->read();
	unsigned char byte3 = conn->read();
	unsigned char byte4 = conn->read();
	return (byte1 << 24) | (byte2 << 16) | (byte3 << 8) | byte4;
}

/*
 * Send a string to a client.
 */
void writeString(const shared_ptr<Connection>& conn, const string& s) {
	for (char c : s) {
		conn->write(c);
	}
	conn->write('$');
}

int main(int argc, char* argv[]){
	if (argc != 2) {
		cerr << "Usage: myserver port-number" << endl;
		exit(1);
	}
	
	int port = -1;
	try {
		port = atoi(argv[1]);
	} catch (exception& e) {
		cerr << "Wrong port number. " << e.what() << endl;
		exit(1);
	}
	
	Server server(port);
	if (!server.isReady()) {
		cerr << "Server initialization error." << endl;
		exit(1);
	}
	
	while (true) {
		auto conn = server.waitForActivity();
		if (conn != nullptr) {
			try {
				int nbr = readNumber(conn);
				string result;
				/*Dags att kolla alla möjliga commands*/
				if (nbr == 1) {
					result = "COM_LISTING_NG";
				} else if (nbr == 2) {
					result = "COM_CREATE_NG";
				} else if(nbr == 3){
					result = "COM_DELETE_NG";
				} else if(nbr == 4){
					result = "COM_LIST_ART";
				} else if(nbr == 5){
					result = "COM_CREATE_ART";
				} else if(nbr == 6){
					result = "COM_DELETE_ART";
				} else if(nbr == 7){
					result = "COM_GET_ART";
				} 

				writeString(conn, result);
			} catch (ConnectionClosedException&) {
				server.deregisterConnection(conn);
				cout << "Client closed connection" << endl;
			}
		} else {
			conn = make_shared<Connection>();
			server.registerConnection(conn);
			cout << "New client connects" << endl;
		}
	}
}
