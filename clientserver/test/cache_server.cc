/* myserver.cc: sample server program */
#include "server.h"
#include "connection.h"
#include "connectionclosedexception.h"
#include "messagehandler.h"
#include "protocol.h"
//#include "common.h"
#include "cache_database.h"
#include "exceptions.h"

#include <memory>
#include <iostream>
#include <string>
#include <stdexcept>
#include <cstdlib>
#include <vector>
//#include "persistant_database.h"

using namespace std;


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
	
	MessageHandler myMsgHandler;
	Cache_Database myDB;
	//Persistant_Database myDB;

	while (true) {
		auto conn = server.waitForActivity();
		if (conn != nullptr) {
			myMsgHandler.changeConnection(*conn.get());
			
			try {
				
				/*Get the command*/
				int nbr = myMsgHandler.recvCode();
				//cout << "Command Nbr is: " << nbr << endl;
				
				if (nbr == Protocol::COM_LIST_NG) {
					cout << "List the newsgroup" << endl;
					
					vector<NewsGroup> myVec = myDB.listNewsGroup();

					myMsgHandler.sendCode(Protocol::ANS_LIST_NG);
					myMsgHandler.sendIntParameter(myVec.size());
					for(auto it = myVec.begin(); it != myVec.end(); ++it){
						myMsgHandler.sendIntParameter(it->id);
						myMsgHandler.sendStringParameter(it->name);

					}
					myMsgHandler.sendCode(Protocol::ANS_END);
				}

				if(nbr == Protocol::COM_CREATE_NG){
					cout << "Create new NewsGroup" << endl;
					string requestedName = myMsgHandler.recvStringParameter();

					myMsgHandler.sendCode(Protocol::ANS_CREATE_NG);
					try{
						myDB.createNewsGroup(requestedName);
						myMsgHandler.sendCode(Protocol::ANS_ACK);
					} catch(NewsGroupAlreadyExistsException& e) {
						cerr << e.what() << endl;
						myMsgHandler.sendCode(Protocol::ANS_NAK);
						myMsgHandler.sendCode(Protocol::ERR_NG_ALREADY_EXISTS);
					}
					myMsgHandler.sendCode(Protocol::ANS_END);

				}

				if(nbr == Protocol::COM_DELETE_NG){
					cout << "Delete newsgroup" << endl;
					int idToRemove = myMsgHandler.recvIntParameter();
					cout << "id to remove" << idToRemove << endl;
					myMsgHandler.sendCode(Protocol::ANS_DELETE_NG);

					try{
						myDB.deleteNewsGroup(idToRemove);
						myMsgHandler.sendCode(Protocol::ANS_ACK);
					} catch (NewsGroupDoesNotExistException& e){
						cout << e.what() << endl;
						myMsgHandler.sendCode(Protocol::ANS_NAK);
						myMsgHandler.sendCode(Protocol::ERR_NG_DOES_NOT_EXIST);
					}
					
					myMsgHandler.sendCode(Protocol::ANS_END);
				}

				if(nbr == Protocol::COM_LIST_ART){
					cout << "list articles" << endl;
					int groupId = myMsgHandler.recvIntParameter();

					
					myMsgHandler.sendCode(Protocol::ANS_LIST_ART);
					try{
						vector<Article> myVec = myDB.listArticles(groupId);
						myMsgHandler.sendCode(Protocol::ANS_ACK);
						myMsgHandler.sendIntParameter(myVec.size());
						
						for(auto it = myVec.begin(); it != myVec.end(); ++it){
							myMsgHandler.sendIntParameter(it->id);
							myMsgHandler.sendStringParameter(it->title);
						}
					} catch(NewsGroupDoesNotExistException& e){
						cout << e.what() << endl;
						myMsgHandler.sendCode(Protocol::ANS_NAK);
						myMsgHandler.sendCode(Protocol::ERR_NG_DOES_NOT_EXIST);
					}
					myMsgHandler.sendCode(Protocol::ANS_END);
				}

				
				if(nbr == Protocol::COM_CREATE_ART){
					cout << "create article" << endl;
					myMsgHandler.sendCode(Protocol::ANS_CREATE_ART);
					int groupID = myMsgHandler.recvIntParameter();
					string title = myMsgHandler.recvStringParameter();
					string author = myMsgHandler.recvStringParameter();
					string content = myMsgHandler.recvStringParameter();

					try{
						myDB.createArticle(title, author, content, groupID);
						myMsgHandler.sendCode(Protocol::ANS_ACK);
					} catch (NewsGroupDoesNotExistException& e){
						cout << e.what() << endl;
						myMsgHandler.sendCode(Protocol::ANS_NAK);
						myMsgHandler.sendCode(Protocol::ERR_NG_DOES_NOT_EXIST);
					}
					myMsgHandler.sendCode(Protocol::ANS_END);

				}

				if(nbr == Protocol::COM_DELETE_ART){
					cout << "detele article" << endl;
					int groupID = myMsgHandler.recvIntParameter();
					int articleID = myMsgHandler.recvIntParameter();

					myMsgHandler.sendCode(Protocol::ANS_DELETE_ART);

					try{
						myDB.deleteArticle(groupID, articleID);
						myMsgHandler.sendCode(Protocol::ANS_ACK);
					} catch (ArticleDoesNotExistException& e){
						cout << e.what() << endl;
						myMsgHandler.sendCode(Protocol::ANS_NAK);
						myMsgHandler.sendCode(Protocol::ERR_ART_DOES_NOT_EXIST);

					} catch (NewsGroupDoesNotExistException& e){
						cout << e.what() << endl;
						myMsgHandler.sendCode(Protocol::ANS_NAK);
						myMsgHandler.sendCode(Protocol::ERR_NG_DOES_NOT_EXIST);
					}
					myMsgHandler.sendCode(Protocol::ANS_END);

				}

				if(nbr == Protocol::COM_GET_ART){
					cout << "get article" << endl;
					int groupID = myMsgHandler.recvIntParameter();
					int articleID = myMsgHandler.recvIntParameter();

					
					myMsgHandler.sendCode(Protocol::ANS_GET_ART);
					try{
						Article myArticle;
						myArticle = myDB.readArticle(groupID, articleID);
						myMsgHandler.sendCode(Protocol::ANS_ACK);
						myMsgHandler.sendStringParameter(myArticle.title);
						myMsgHandler.sendStringParameter(myArticle.author);
						myMsgHandler.sendStringParameter(myArticle.content);
					} catch(ArticleDoesNotExistException& e){
						cout << e.what() << endl;
						myMsgHandler.sendCode(Protocol::ANS_NAK);
						myMsgHandler.sendCode(Protocol::ERR_ART_DOES_NOT_EXIST);
					} catch(NewsGroupDoesNotExistException& e){
						cout << e.what() << endl;
						myMsgHandler.sendCode(Protocol::ANS_NAK);
						myMsgHandler.sendCode(Protocol::ERR_NG_DOES_NOT_EXIST);
					}

					

					
					

					
					

					
					myMsgHandler.sendCode(Protocol::ANS_END);
				}

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
