/* myclient.cc: sample client program */
#include "connection.h"
#include "connectionclosedexception.h"
#include "protocol.h"
#include "messagehandler.cc"
#include "exceptions.h"

#include <iostream>
#include <string>
#include <stdexcept>
#include <cstdlib>
#include <map>
#include <iterator>
#include <algorithm>
#include <regex>

using namespace std;

/*
 * Send an integer to the server as four bytes.
 */
void writeNumber(const Connection& conn, int value) {
	conn.write((value >> 24) & 0xFF);
	conn.write((value >> 16) & 0xFF);
	conn.write((value >> 8)	 & 0xFF);
	conn.write(value & 0xFF);
}


/*
 * Read a string from the server.
 */
string readString(const Connection& conn) {
	string s;
	char ch;
	while ((ch = conn.read()) != '$') {
		s += ch;
	}
	return s;
}
bool checkIfDigit(const string& IDtoCheck){


//return !IDtoCheck.empty() && find_if(IDtoCheck.begin(),
//IDtoCheck.end(), [] (char c) { return !isdigit(c); }) == IDtoCheck.end();

return !IDtoCheck.empty() && regex_match(IDtoCheck, regex("[0-9]+"));
} 
bool checkIfValid(string& s, int& artID){
	int articleID;
	if(!checkIfDigit(s)){
		cout << " Error: Invalid input! Must be a positive integer! " << endl;
		return false;
		}

		try{
			articleID = stoi(s);				
		} catch(out_of_range& e){
			cout << "Error: Integer out of range!" << endl;
			return false;
		}
	artID = articleID;
	return true;
}
void welcomeScreen(string name){
	cout<<"Welcome to the: \n";
	cout<< endl;
	cout<<"  _   _                         _____                               \n"; 
	cout<<" | \\ | |                       / ____|                              \n";
	cout<<" |  \\| |  ___ __      __ ___  | (___    ___  _ __ __   __ ___  _ __ \n";
	cout<<" | . ` | / _ \\ \\ /\\ / // __|  \\___ \\  / _ \\| '__|\\ \\ / // _ \\| '__|\n";
	cout<<" | |\\  ||  __/ \\ V  V / \\__ \\  ____) ||  __/| |    \\ V /|  __/| |   \n";
	cout<<" |_| \\_| \\___|  \\_/\\_/  |___/ |_____/  \\___||_|     \\_/  \\___||_|   \n";
	cout<< endl;
	cout<< name << "!" << endl;
	cout<< endl;
	cout<< "Type a command to preform or type\n";
	cout<< "'help' to get list of available commands! " << endl;
}

int main(int argc, char* argv[]) {
	if (argc != 3) {
		cerr << "Usage: of client <host-name> <port-number> '" << endl;
		exit(1);
	}
	
	int port = -1;
	try {
		port = atoi(argv[2]);
	} catch (exception& e) {
		cerr << "Wrong port number. " << e.what() << endl;
		exit(1);
	}
	
	Connection conn(argv[1], port);
	if (!conn.isConnected()) {
		cerr << "Connection attempt failed" << endl;
		exit(1);
	}
	welcomeScreen(argv[1]);
	
	//int nbr;
	string command;
	MessageHandler myMsgHandlr;
	myMsgHandlr.changeConnection(conn);
	//Server server(port); //WHAT?
	
	 while (getline(cin, command)/*cin >> command*/) {
	// nu har vi coomna,d om vi har mellanslag, skicka med error

// cout wrong format, or else continue with the word.
		try {
			//cout << command << " <- cmd is ...";
			if(command == "help"){
				cout << "-------------------------------------------------------------------------" << endl;
				cout << "Command: lsng (list newsgroup) Used to list all the available NewsGroups." << endl;
				cout << "Command: mkng (make newsgroup) Used to make a new NewsGroup" << endl;
				cout << "Command: rmng (remove newsgroup) Used to remove a NewsGroup" << endl;
				cout << "Command: lsart (list articles) Used to list all the available Articles" << endl;
				cout << "Command: mkart (make article) Used to create a new Article" << endl;
				cout << "Command: rmart (remove article) Used to remove an Article" << endl;
				cout << "Command: readart (read article) Used to read the specified Article" << endl;
				cout << "Command: quit (quit) Used to exit the program!" << endl;
				cout << "-------------------------------------------------------------------------" << endl;
				command.clear();
				continue;
			}
			if(command == "lsng"){
				map<int,string> theList;

				myMsgHandlr.sendCode(Protocol::COM_LIST_NG);
				myMsgHandlr.sendCode(Protocol::COM_END);

				int code = myMsgHandlr.recvCode();
				if(code == Protocol::ANS_LIST_NG){
					int nbrOfGrp = myMsgHandlr.recvIntParameter();
					for(int i = 0; i < nbrOfGrp; ++i){
						int id = myMsgHandlr.recvIntParameter();
						string name = myMsgHandlr.recvStringParameter();
						theList.insert(make_pair(id, name));	
					}
					int code = myMsgHandlr.recvCode();
					if(code == Protocol::ANS_END){
						if(theList.empty()){
							cout << "No NewsGroups to list." << endl;
							command.clear();
							continue;
						}
						cout << "Group ID: \t Group name:" << endl;
						for_each(theList.rbegin(), theList.rend(), 
							[](pair<int,string> mypair){cout << mypair.first << "\t\t " << mypair.second << endl;});
						command.clear();
						continue;
						}
				} 
			}
			//if(is_permutation(command.begin(), command.end(), "mkng")){
			if(command == "mkng"){
				string name;
				cout << "Enter a name for the new NewsGroup!" << endl;
				//cin.ignore();
				getline(cin, name);
				cout << "The name you just entered: " << name << endl;

				myMsgHandlr.sendCode(Protocol::COM_CREATE_NG);
				myMsgHandlr.sendStringParameter(name);
				myMsgHandlr.sendCode(Protocol::COM_END);

				int code = myMsgHandlr.recvCode();
				if(code == Protocol::ANS_CREATE_NG){
					code = myMsgHandlr.recvCode();
					if(code == Protocol::ANS_ACK){
						cout << "Successfully created NewsGroup: " << name << endl;
					} else if(code == Protocol::ANS_NAK){
						code = myMsgHandlr.recvCode();
						if(code == Protocol::ERR_NG_ALREADY_EXISTS){
							cerr << "That name aldready exists, try again with another name!" << endl;
						}
					}
					code = myMsgHandlr.recvCode();
					if(code == Protocol::ANS_END){
						command.clear();
						continue;
					}
				}
			}
			if(command == "rmng"){
				int ID;
				string IDstring;
				cout << "Enter the ID of the NewsGroup to delete!" << endl;
				//cin >> ID;
				getline(cin, IDstring);
				
				if(!checkIfValid(IDstring, ID)){
				continue;
				}

				myMsgHandlr.sendCode(Protocol::COM_DELETE_NG);
				myMsgHandlr.sendIntParameter(ID);
				myMsgHandlr.sendCode(Protocol::COM_END);

				int code = myMsgHandlr.recvCode();
				if(code == Protocol::ANS_DELETE_NG){
					code = myMsgHandlr.recvCode();
					if(code == Protocol::ANS_ACK){
						cout << "Successfully removed the group " << ID << endl;
					} else if(code == Protocol::ANS_NAK){
						code = myMsgHandlr.recvCode();
						if(code == Protocol::ERR_NG_DOES_NOT_EXIST){
							cout << "NewsGroup with ID \"" << ID << "\" does not exist." << endl;
						}
					}
					code = myMsgHandlr.recvCode();
					if(code == Protocol::ANS_END){
						command.clear();
						continue;
					}
				}
			}
			if(command == "mkart"){
				int newsGroup;
				string newsGroupstring;
				string title;
				string authour;
				string content;
				
				cout << "Enter the ID of the NewsGroup the Article belongs to!" << endl;
				getline(cin, newsGroupstring);
				//cin >> newsGroup;
				//cin.ignore(); // used to flush the 'newline' before getline

				if(!checkIfValid(newsGroupstring, newsGroup)){
				continue;
				}
				
				cout << "Enter the title of the Article to create." << endl;
				getline(cin, title);
				//cin >> title;
				cout << "Enter the author of the Article." << endl;
				getline(cin, authour);
				//cin >> authour;
				cout << "Enter the content of the Article." << endl;
				getline(cin, content);
				//cin >> content;
				
				myMsgHandlr.sendCode(Protocol::COM_CREATE_ART);
				myMsgHandlr.sendIntParameter(newsGroup);
				myMsgHandlr.sendStringParameter(title);
				myMsgHandlr.sendStringParameter(authour);
				myMsgHandlr.sendStringParameter(content);
				myMsgHandlr.sendCode(Protocol::COM_END);

				int code = myMsgHandlr.recvCode();

				if(code == Protocol::ANS_CREATE_ART){
					code = myMsgHandlr.recvCode();
					if(code == Protocol::ANS_ACK){
						cout << "Successfully added article \"" << title << "\" to NewsGroup: " << newsGroup << endl;
					} else if(code == Protocol::ANS_NAK){
						code = myMsgHandlr.recvCode();
						if(code == Protocol::ERR_NG_DOES_NOT_EXIST){
							cout << "NewsGroup with ID: " << newsGroup << " does not exist." << endl;
						}
					}
					code = myMsgHandlr.recvCode();
					if(code == Protocol::ANS_END){
						continue;
					}
				}
			}
			if(command == "lsart"){
				map<int, string> articleList;
				int groupID;
				string groupIDstring;
				cout << "Enter which NewsGroup's articles you wish to list!" << endl;
				//cin >> groupID;

				getline(cin, groupIDstring);

				if(!checkIfValid(groupIDstring, groupID)){
				continue;
				}
				
				myMsgHandlr.sendCode(Protocol::COM_LIST_ART);
				myMsgHandlr.sendIntParameter(groupID);
				myMsgHandlr.sendCode(Protocol::COM_END);

				int code = myMsgHandlr.recvCode();
				if(code == Protocol::ANS_LIST_ART){
					code = myMsgHandlr.recvCode();
					if(code == Protocol::ANS_ACK){
						int nbrOfArts = myMsgHandlr.recvIntParameter();
						for(int i = 0; i < nbrOfArts; ++i){
							int articleID = myMsgHandlr.recvIntParameter();
							string articleTitle = myMsgHandlr.recvStringParameter();
							articleList.insert(make_pair(articleID, articleTitle));
						}
					} else if(code == Protocol::ANS_NAK){
						code = myMsgHandlr.recvCode();
						if(code == Protocol::ERR_NG_DOES_NOT_EXIST){
							cout << "That NewsGroup does not exist!" << endl;
							code = myMsgHandlr.recvCode();
							continue;
						}
					}
					code = myMsgHandlr.recvCode();
					if(code == Protocol::ANS_END){
						if(articleList.empty()){
							cout << "No articles available!" << endl;
							continue;
						}
						cout << "Article ID:" << "\t" << "Article name: " << endl;
						for_each(articleList.rbegin(), articleList.rend(), 
							[](pair<int,string> mypair){cout << mypair.first << "\t\t" << mypair.second << endl;});
						continue;
					}
				}
			}
			if(command == "rmart"){
				int groupID;
				int articleID;
				string groupIDstring;
				string articleIDstring;

				cout << "Enter the ID of the NewsGroup" << endl;
				getline(cin, groupIDstring);

				if(!checkIfValid(groupIDstring, groupID)){
				continue;
				}
				
				cout << "Enter the ID of the Article" << endl;
				getline(cin, articleIDstring);
				if(!checkIfValid(articleIDstring, articleID)){
				continue;
				}

				myMsgHandlr.sendCode(Protocol::COM_DELETE_ART);
				myMsgHandlr.sendIntParameter(groupID);
				myMsgHandlr.sendIntParameter(articleID);
				myMsgHandlr.sendCode(Protocol::COM_END);

				int code = myMsgHandlr.recvCode();
				if(code == Protocol::ANS_DELETE_ART){
					code = myMsgHandlr.recvCode();
					if(code == Protocol::ANS_ACK){
						cout << "Article " << articleID << " was successfully removed!" << endl;
					} else if(code == Protocol::ANS_NAK){
						code = myMsgHandlr.recvCode();
						if(code == Protocol::ERR_NG_DOES_NOT_EXIST){
							cout << "No such NewsGroup exists!" << endl;
						} else if(code == Protocol::ERR_ART_DOES_NOT_EXIST){
							cout << "No such Article exists!" << endl;
						}
					}
					code = myMsgHandlr.recvCode(); //denna behövs alltid 
					if(code == Protocol::ANS_END){
						continue;
					}
				}
			}
			if(command == "readart"){
				int groupID;
				int articleID;
				string groupIDstring;
				string articleIDstring;

				string title;
				string authour;
				string content;
				
				cout << "Enter the ID of the NewsGroup" << endl;
				getline(cin, groupIDstring);

				//int groupTemp;
				if(!checkIfValid(groupIDstring, groupID)){
				continue;
				}

				cout << "Enter the ID of the Article" << endl;
				getline(cin, articleIDstring);

				if(!checkIfValid(articleIDstring, articleID)){
				continue;
				}

				myMsgHandlr.sendCode(Protocol::COM_GET_ART);
				myMsgHandlr.sendIntParameter(groupID);
				myMsgHandlr.sendIntParameter(articleID);
				myMsgHandlr.sendCode(Protocol::COM_END);

				int code = myMsgHandlr.recvCode();
				if(code == Protocol::ANS_GET_ART){
					code = myMsgHandlr.recvCode();
					if(code == Protocol::ANS_ACK){
						title   = myMsgHandlr.recvStringParameter();
						authour = myMsgHandlr.recvStringParameter();
						content = myMsgHandlr.recvStringParameter();
					} else if(code == Protocol::ANS_NAK){
						code = myMsgHandlr.recvCode();
						if(code == Protocol::ERR_NG_DOES_NOT_EXIST){
							cout << "No such NewsGroup exists!" << endl;
							code = myMsgHandlr.recvCode();
							continue;
						} else if(code == Protocol::ERR_ART_DOES_NOT_EXIST){
							cout << "No such Article exists!" << endl;
							code = myMsgHandlr.recvCode();
							continue;
						}
					}
				}
				code = myMsgHandlr.recvCode();
				if(code == Protocol::ANS_END){
					cout << title << " Written by: " << authour << endl << endl;
					cout << content << endl;
					continue;
				}
			}
			if(command == "quit"){
				cout << "Exiting... See you next time!" << endl;
				exit(1); 
			}

			cout << "No such command available, type 'help' to get a list of available commands!" << endl;

			//writeNumber(conn, nbr);
			//string reply = readString(conn);
			//cout << " " << reply << endl;
			//cout << "Type another number: ";
		} catch (ConnectionClosedException&) {
			cout << " no reply from server. Exiting." << endl;
			exit(1);
		}
	}
}

