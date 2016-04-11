#include "persistant_database.h"
#include <iostream>
#include <algorithm>
#include "exceptions.h"
#include <fstream>
//#include <cv.h>
//#include <boost/filesystem.hpp>
#include <cstdlib>		/*system*/
#include <sys/types.h> 	/*opendir*/
#include <dirent.h>		/*opendir*/
#include <sys/stat.h>	/*mkdir*/
#include <sys/types.h>	/*mkdir*/
#include <errno.h>		/*catching errno*/
#include <string.h>		/*catching errno*/
#include <stdio.h>		/*catching errno*/
#include <dirent.h>		/*readdir*/
#include <string>		/*string*/
#include <sstream>		/*ofstream*/
#include <cstdio>		/*remove*/


using namespace std;


Persistant_Database::Persistant_Database(){
	int maxID2 = 0;
	int status;
 	status = mkdir("/DBRoot", S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
 	if(status == 0){
		cout << "Persistant_Database has been created!" << endl;
 	} else if(status == -1){
 		cout << "Successfully read existing database!" << endl;
 	}
	cout << "vart fastnar vi? #1 " << endl;
 	int maxID = 0;
 	cout << "vart fastnar vi? #1.1 " << endl;
	DIR *pdir;
	cout << "vart fastnar vi? #1.2 " << endl;
	struct dirent *pent;
	cout << "vart fastnar vi? #2 " << endl;
	
	pdir=opendir("/DBRoot"); 
	if (!pdir){
		printf ("opendir() failure; terminating");
		throw NewsGroupDoesNotExistException();
		exit(1);
	}
	cout << "vart fastnar vi? #3 " << endl;
	errno=0;
	cout << "har hoppat över ..? " << endl;
	while ((pent=readdir(pdir))){
		 if (!strcmp(pent->d_name, ".") || !strcmp(pent->d_name, ".."))
          {
          	cout << " SKIPPAR punkter?" << endl;
             continue;
          }
 		cout << "EFTER ÖVERHOPPNINGEN" << endl;
		cout << "the pent->d_name: " << pent->d_name << endl;
		//printf("%s", pent->d_name);
		string tempstr(pent->d_name);

		stringstream ss(tempstr);
		string id;
		string name;
		string tempname;


		ss >> id;
		while(ss >> name){
			tempname += name;
			tempname += "\\ ";
		}
		tempname.pop_back();

		
		string newdir = "/DBRoot/";
		newdir += pent->d_name;
		DIR *pdir2;
		struct dirent *pent2;

		cout << "THE NEW DIR TO OPEN: " << newdir << endl;
		pdir2=opendir(newdir.c_str()); 
		if (!pdir2){
			printf ("opendir() failure; terminating");
			throw NewsGroupDoesNotExistException();
			exit(1);
		}
		errno=0;
		cout << "har hoppat över ..? " << endl;
		while ((pent2=readdir(pdir2))){
			 if (!strcmp(pent2->d_name, ".") || !strcmp(pent2->d_name, ".."))
	          {
	             continue;
	          }
	 		cout << "EFTER ÖVERHOPPNINGEN" << endl;
			cout << "the pent2->d_name: " << pent2->d_name << endl;
			//printf("%s", pent2->d_name);
			string tempstr(pent2->d_name);

			stringstream ss(tempstr);
			string id;
			
			ss >> id;
			cout << "ID IS: " << id << endl;
			if(atoi(id.c_str()) > maxID){
				
				maxID2 = atoi(id.c_str());
				cout << "MAXID2: " << maxID2 << endl;
			}
		}
		if (errno){
		printf ("readdir() failure; terminating");
		exit(1);
		}
		closedir(pdir2);


		
		if(atoi(id.c_str()) > maxID){
			maxID = atoi(id.c_str());
			cout << "MaxID: " << maxID << endl;
		}


	}
	cout << "vart fastnar vi? #4 " << endl;
	if (errno){
	printf ("readdir() failure; terminating");
	exit(1);
	}
	cout << "vart fastnar vi? #5 " << endl;
	closedir(pdir); // BEHÖER NOG CLOSEDIR(PDIR) HÄR NGNSTANS
	
	cout << "the maxID variable: " << maxID << endl;
	uniquenbr = maxID;
	uniquenbr2 = maxID2;


	//uniquenbr = 0;
	//uniquenbr2 = 0;
}
	
/*---------------NewsGroup operations------------------*/
void Persistant_Database::createNewsGroup(string& theName){
	//NewsGroup myNewsGroup(theName);
	//myNewsGroup.id = ++uniquenbr;
	/*
	string maxID = 0;
	DIR *pdir;
	struct dirent *pent;

	
	pdir=opendir("c:/DBRoot"); 
	if (!pdir){
		printf ("opendir() failure; terminating");
		throw NewsGroupDoesNotExistException();
		exit(1);
	}
	errno=0;
	cout << "har hoppat över ..? " << endl;
	while ((pent=readdir(pdir))){
		 if (!strcmp(pent->d_name, ".") || !strcmp(pent->d_name, ".."))
          {
             continue;
          }
 		cout << "EFTER ÖVERHOPPNINGEN" << endl;
		cout << "the pent->d_name: " << pent->d_name << endl;
		//printf("%s", pent->d_name);
		string tempstr(pent->d_name);

		stringstream ss(tempstr);
		string id;
		
		ss >> id;
		
		if(id > maxID){
			maxID = id;

		}
	}
	if (errno){
	printf ("readdir() failure; terminating");
	exit(1);
	}
	closedir(pdir); // BEHÖER NOG CLOSEDIR(PDIR) HÄR NGNSTANS
	
	cout << "the maxID variable: " << maxID << endl;
	uniquenbr = atoi(maxID.c_str());
	
	*/
	cout << "entering Persistant_Database createNG" << endl;

	//auto it = find_if(  db.begin(), db.end(), 
	//[&](pair<int, NewsGroup> ng){return ng.second.name == theName;});
	
	//cout << "it->first " << it->first << endl;
	//if(it == db.end()){
	//	cout << "inserting into map" << endl;
	//	db.insert(make_pair(uniquenbr, myNewsGroup));
		int temp = ++uniquenbr;
		//string lol = to_string(++uniquenbr);
		string dir = "/DBRoot/";
		dir += to_string(temp);//lol; // OM TO_STRING HADE FUNKAT MED FATTIGA CYGWIN KANSKE???!?!?!
		dir += " ";
		dir += theName;
		cout << "the DIR to create: " << dir << endl;
		int status = mkdir((char*)dir.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
		
		if(status == 0){
			cout << "NewsGroup created! " << endl;
		} else if(status == -1){
			//cout << strerror(errno) << endl;
			throw NewsGroupAlreadyExistsException();
		}
		
	//} else {
	//	throw NewsGroupAlreadyExistsException();
	//	cout << "throw: group already exists" << endl;
	//}
}

void Persistant_Database::deleteNewsGroup(int idToBeRemoved){
	cout << "id to remove " << idToBeRemoved << endl;
	//auto it = db.find(idToBeRemoved);
	//if(it != db.end()){
	//	db.erase(it);
	//} else {
	//	throw NewsGroupDoesNotExistException();
	//	cout << "throw no such newsGruop to delete" << endl;
	//}
	bool rmFlag = false;
	DIR *pdir;
	struct dirent *pent;
	string dir = "/DBRoot";
	// idToBeRemoved NEEDS THE TO_STRING ###########################################################################
	//dir += "/";
	//dir += "1*"; 
	cout << "the dir string: " << dir << endl;

	pdir=opendir(dir.c_str()); 
	if (!pdir){
		printf ("opendir() failure; terminating");
		throw NewsGroupDoesNotExistException();
		exit(1);
	}
	errno=0;
	while ((pent=readdir(pdir))){
		 if (!strcmp(pent->d_name, ".") || !strcmp(pent->d_name, ".."))
          {
             continue;
          }
		cout << "the pent->d_name: " << pent->d_name << endl;
		//printf("%s", pent->d_name);
		string tempstr(pent->d_name);

		stringstream ss(tempstr);
		string id;
		string name;
		string tempname;
		ss >> id;
		
		while(ss >> name){
			ss >> name;
			tempname += name;
		} 
		//THIS 1 SHOULD BE id if the fucking to_string would work...
				
		if(atoi(id.c_str()) == idToBeRemoved){
			string temp = dir;
			temp += "/";
			temp += id;
			temp += "*";
			//dir += "/1*";
			//dir += pent->d_name;
			string command = "rm -rf ";
			command += temp;
			
			cout << "THE TEMP STR: " << temp << endl;
			cout << "the command str: " << command << endl;
			system( command.c_str() );
			rmFlag = true;


			cout << "Found the file to remove" << endl;
			// if(remove(pent->d_name) != 0){
			// 	cout << "Could not remove the file!" << endl;
			// 	perror("error removing file: ");
			// } else {
			// 	cout << "NewsGroup: " << tempstr << " removed successfully!" << endl;
 		// 	}
		temp.clear();
		}



		cout << "id is:" << id << " name is: " << name << endl;
		
	}
	if(!rmFlag){
		throw NewsGroupDoesNotExistException();
	}
	

}

vector<NewsGroup> Persistant_Database::listNewsGroup(){
	vector<NewsGroup> tempVec;

	DIR *pdir;
	struct dirent *pent;

	pdir=opendir("/DBRoot"); 
	if (!pdir){
		printf ("opendir() failure; terminating");
		exit(1);
	}
	errno=0;
	while ((pent=readdir(pdir))){
		if (!strcmp(pent->d_name, ".") || !strcmp(pent->d_name, ".."))
          {
             continue;
          }
		printf("%s", pent->d_name);
		string tempstr(pent->d_name);

		stringstream ss(tempstr);
		string id;
		string name;
			
		string tempname;
		ss >> id;
		while(ss >> name){
			cout << "SS: " << name << endl;
			tempname += name;
			tempname += " "; 
		} 

		//while(ss){
		//	ss >> id >> name;
		//} 

		cout << "id is:" << id << " name is: " << tempname << endl;
		NewsGroup myNewsGroup(tempname);
		myNewsGroup.id = atoi(id.c_str());
		
		if(atoi(id.c_str()) != 0) {
			tempVec.push_back(myNewsGroup);
		}
		
		// for(auto it = tempVec.begin(); it < tempVec.end(); ++it){
		// 	cout << "WRITING FORLOOP" << endl << endl;
		// 	cout << it->name << it->id << endl;
		// }

	}
	if (errno){
	printf ("readdir() failure; terminating");
	exit(1);
	}
	closedir(pdir);

	
	return tempVec;
}

/*----------------------------------------------------*/

/*----------------Article operations------------------*/
void Persistant_Database::createArticle(	string& title, string& author, 
											string& content, int groupID){
	//Article myArticle(title, author, content);
	//myArticle.id = ++uniquenbr2;
	
	bool createflag = false;
	DIR *pdir;
	struct dirent *pent;
	pdir=opendir("/DBRoot"); 
	if (!pdir){
		printf ("opendir() failure; terminating");
		exit(1);
	}
	errno=0;
	while ((pent=readdir(pdir))){
		if (!strcmp(pent->d_name, ".") || !strcmp(pent->d_name, ".."))
          {
             continue;
          }
		printf("%s", pent->d_name);
		string tempstr(pent->d_name);

		stringstream ss(tempstr);
		string id;
		string name;
		ss >> id;
		while(ss){
			ss >> name;
		} 
		if(atoi(id.c_str()) == groupID){
			createflag = true;
			int temp = ++uniquenbr2;
			cout << "Found the folder to where the article is supposed to be added " << endl;
			string articleDir = "/DBRoot/";
			articleDir += pent->d_name;
			articleDir += "/";
			articleDir += to_string(temp); //to_string(id); // this should be id.to_string() #################################################################
			articleDir += " ";
			articleDir += title;
			articleDir += ".txt";
			cout << "The made articlename: " << articleDir << endl;

			ofstream outputFile(articleDir);
			outputFile << "Article by: " << author << endl << "\n" << content << endl;
			cout << "Successfully created Article: " << title << " and stored it in database!" << endl; 
		}
		if(!createflag){
			cout << "Could not find specified NewsGroup!" << endl;
			throw NewsGroupDoesNotExistException();
		}
		
		cout << "id is:" << id << " name is: " << name << endl;
	}
	if (errno){
	printf ("readdir() failure; terminating");
	exit(1);
	}
	closedir(pdir);




	// auto it = db.find(groupID);
	// if(it != db.end()){
	// 	it->second.theArticles.push_back(myArticle);

	// } else {
	// 	throw NewsGroupDoesNotExistException();
	// 	cout << "throw a no such group exceptoion" << endl;
	// }
}
void Persistant_Database::deleteArticle(int groupID, int articleID){
	
	string tempdir = "/DBRoot/";
	bool foundNG = false;
	bool foundArt = false;
	DIR *pdir;
	struct dirent *pent;
	pdir=opendir("/DBRoot"); 
	if (!pdir){
		printf ("opendir() failure; terminating");
		exit(1);
	}
	errno=0;
	while ((pent=readdir(pdir))){
		if (!strcmp(pent->d_name, ".") || !strcmp(pent->d_name, ".."))
          {
             continue;
          }
		printf("%s", pent->d_name);
		string tempstr(pent->d_name);

		stringstream ss(tempstr);
		string id;
		string name;
		//while(ss){
		//	ss >> id >> name;
		//}

		string tempname;
		ss >> id;
		while(ss >> name){
			cout << "SS: " << name << endl;
			tempname += name;
			tempname += "\\ "; 
		} 
		tempname.pop_back();

		if(atoi(id.c_str()) == groupID){
			foundNG = true;
			
			DIR *pdir2;
			struct dirent *pent2;
			string dir = "/DBRoot/";
			dir += pent->d_name;
			tempdir += id;
			tempdir += "\\ ";
			tempdir += tempname;
			


			cout << "the new DIR: " << dir << endl;
			pdir2=opendir(dir.c_str()); 
			if (!pdir2){
				printf ("opendir() failure; terminating");
				exit(1);
			}
			errno=0;
			
			while ((pent2=readdir(pdir2))){
				if (!strcmp(pent2->d_name, ".") || !strcmp(pent2->d_name, ".."))
		          {
		             continue;
		          }
				printf("%s", pent2->d_name);
				string tempstr(pent2->d_name);

				stringstream ss(tempstr);
				string id;
				string name;
				ss >> id;
				while(ss){
					ss >> name;
				} 
				cout << "PRINTING ID: " << id << endl;
				if(atoi(id.c_str()) == articleID){
					foundArt = true;
					cout << "DELETE THE ARTICLE! " << endl;
					string command = "rm -f ";
					command += tempdir;
					command += "/";
					command += id;
					//command += "\\ ";
					command += "*";
					//command += name;
					//command += "*";

					cout << "THE ART DEL COMMAND: " << command << endl; 
					system(command.c_str());
					

				}
				if(!foundNG){
					cout << "Could not find specified NewsGroup!" << endl;
					throw NewsGroupDoesNotExistException();
				}
				if(!foundArt){
					cout << "Could not find the specified Article!" << endl;
				}
				
				cout << "id is:" << id << " name is: " << name << endl;
	}
	if (errno){
	printf ("readdir() failure; terminating");
	exit(1);
	}
	closedir(pdir2);

		}
		if(!foundNG){
			cout << "Could not find specified NewsGroup!" << endl;
			throw NewsGroupDoesNotExistException();
		}
		
		cout << "id is:" << id << " name is: " << name << endl;
	}
	if (errno){
	printf ("readdir() failure; terminating");
	exit(1);
	}
	closedir(pdir);


	// auto it = db.find(groupID);
	// if(it != db.end()){
		
	// 	auto remover = remove_if(it->second.theArticles.begin(), 
	// 	it->second.theArticles.end(), 
	// 	[&](Article myR){return myR.id == articleID;});
		
	// 	if(remover == it->second.theArticles.end()){
	// 		throw ArticleDoesNotExistException();
	// 		cout << "throw: no such article found" << endl;
	// 	}
	// 	it->second.theArticles.erase(remover, it->second.theArticles.end());
 
	// } else {
	// 	throw NewsGroupDoesNotExistException();
	// 	cout << "throw: no such group found" << endl;
	// }
}
vector<Article> Persistant_Database::listArticles(int inNewsGroup){
	vector<Article> tempVec;
	
	DIR *pdir;
	struct dirent *pent;

	pdir=opendir("/DBRoot"); 
	if (!pdir){
		printf ("opendir() failure; terminating");
		exit(1);
	}
	errno=0;
	while ((pent=readdir(pdir))){
		if (!strcmp(pent->d_name, ".") || !strcmp(pent->d_name, ".."))
        {
            continue;
        }
		printf("%s", pent->d_name);
		string tempstr(pent->d_name);

		stringstream ss(tempstr);
		string id;
		string name;
		ss >> id;
		while(ss){
			ss >> name;
		} 

		cout << "THE NAME: " << name << "THE ID: " << id << endl;
		
		/*--------read2 start -------------------*/
		if(atoi(id.c_str()) == inNewsGroup){
			cout << "FOUND THE CORRECT NEWSGORUP TO LIST ARTICLES IN" << endl;
			

			DIR *pdir2;
			struct dirent *pent2;

			string dir = "/DBRoot/";
			dir += pent->d_name;
			cout << "opening second dir: " << dir << endl;
			pdir2=opendir(dir.c_str()); 
			if (!pdir2){
				printf ("opendir() failure; terminating");
				exit(1);
			}
			errno=0;
			while ((pent2=readdir(pdir2))){
				if (!strcmp(pent2->d_name, ".") || !strcmp(pent2->d_name, ".."))
		          {
		             continue;
		          }
				printf("%s", pent2->d_name);
				string tempstr(pent2->d_name);

				stringstream ss(tempstr);
				string id;
				string name;
				string tempname;

				ss >> id;
				while(ss >> name){
					cout << "SS: " << name << endl;
					tempname += name;
					tempname += " "; 
				} 
				cout << "ART NAME: " << name << endl;
				cout << "TEMP NAME: " << tempname << endl << endl << endl;
				string lol = "";
				string lol2 = "";

				Article myArticle(tempname, lol, lol2);
				myArticle.id = atoi(id.c_str());
				
				if(atoi(id.c_str()) != 0) {
					tempVec.push_back(myArticle);
				}

				

				
				
				
				
				
				// for(auto it = tempVec.begin(); it < tempVec.end(); ++it){
				// 	cout << "WRITING FORLOOP" << endl << endl;
				// 	cout << it->title << it->id << endl;
				// }

			}
			if (errno){
			printf ("readdir() failure; terminating");
			exit(1);
			}
			closedir(pdir2);

		}

		/*--------read2 slut -------------------*/

	

	}
	if (errno){
	printf ("readdir() failure; terminating");
	exit(1);
	}
	closedir(pdir);



	// auto it = db.find(inNewsGroup);
	// if(it != db.end()){
	// 	for(unsigned int i = 0; i < it->second.theArticles.size(); ++i){
	// 		tempVec.push_back(it->second.theArticles[i]);
	// 	}
	// } else {
	// 	throw NewsGroupDoesNotExistException();
	// 	cout << "throw no such group found" << endl;
	// }
	//reverse(tempVec.begin(), tempVec.end());
	return tempVec;

}
Article Persistant_Database::readArticle(int groupID, int articleID){
	string title;
	string author;
	string content;

	DIR *pdir;
	struct dirent *pent;
	pdir=opendir("/DBRoot"); 
	if (!pdir){
		printf ("opendir() failure; terminating");
		exit(1);
	}
	errno=0;
	while ((pent=readdir(pdir))){
		if (!strcmp(pent->d_name, ".") || !strcmp(pent->d_name, ".."))
          {
             continue;
          }
		printf("%s", pent->d_name);
		string tempstr(pent->d_name);

		stringstream ss(tempstr);
		string id;
		string name;
		ss >> id;
		while(ss){
			ss >> name;
		} 

		if(atoi(id.c_str()) == groupID){ //
			/*SECOND READ*/
			DIR *pdir2;
			struct dirent *pent2;
			string newdir = "/DBRoot/";
			newdir += pent->d_name;
			cout << "the new dir to read from: " << newdir << endl;
			pdir2=opendir(newdir.c_str()); 
			if (!pdir2){
				printf ("opendir() failure; terminating");
				exit(1);
			}
			errno=0;
			while ((pent2=readdir(pdir2))){
				if (!strcmp(pent2->d_name, ".") || !strcmp(pent2->d_name, ".."))
		          {
		             continue;
		          }
				printf("%s", pent2->d_name);
				string tempstr(pent2->d_name);

				stringstream ss(tempstr);
				string id;
				string name;
				string tempname;
				ss >> id;
				while(ss >> name){
					tempname += name;
					tempname += " ";
				} 
				cout << "TEMPNAME: " << tempname << endl;

				cout << "THE NAME BEFORE STRIP" << tempname << endl;
				if(atoi(id.c_str()) == articleID){
					for(unsigned int i = 0; i < tempname.length() - 4; ++i){
						title += tempname[i];
					}
					cout << "TITLE CREATED: " << title << endl;
					cout << "Found the Article to read " << endl;

					string articleDir = "/DBRoot/";
					articleDir += pent->d_name;
					articleDir += "/";
					articleDir += id; //to_string(id); // this should be id.to_string() // #######################################################
					articleDir += " ";
					articleDir += tempname;
					cout << "The made articlename: " << articleDir << endl;
					cout << "the made articlename in c: " << articleDir.c_str() << endl;
					articleDir.pop_back(); // lösningen................... haha lol
					//FATTAR INTE DETTA..................!!!!!!!!!!AAAH
					//cout << strncmp(articleDir.c_str(), "c:\\DBRoot\\1 Java\\1 this is a title with many spaces.txt", 55) <<endl;
					//cout << strcmp(articleDir.c_str(), "c:\\DBRoot\\1 Java\\1 this is a title with many spaces.txt") << endl;
				//	cout << articleDir.length() << endl;
				//	for(unsigned int i = 0; i < articleDir.length(); ++i){
				//		cout << "for: " << articleDir[i] << endl;
				//	}
					//Well, problems with different separators in windows/linux
					ifstream inputFile(articleDir);
					//cout << "the lol name: " << "c:\\DBRoot\\1 Java\\1 this is a title with many spaces.txt" << endl;
					//ifstream inputFile("c:\\DBRoot\\1 Java\\1 this is a title with many spaces.txt");

					//c:/DBRoot/1 Java/1 this is a title with many spaces.txt
					string templine;
					int whilecounter = 0;
					while(getline(inputFile,templine)){
						if(whilecounter == 0){
							for(unsigned int k = 12; k < templine.length(); ++k){
								author += templine[k];
							}
						}
						if(whilecounter != 0){
						content += templine;
						}

						whilecounter++;
					}





					
				}
				
				

			}
			if (errno){
			printf ("readdir() failure; terminating");
			exit(1);
			}
			closedir(pdir2);
			/* END OF SECOND READ*/
		}
		
		

	}
	if (errno){
	printf ("readdir() failure; terminating");
	exit(1);
	}
	closedir(pdir);













	// auto it = db.find(groupID);
	// if(it != db.end()){
		
	// 	cout << "går in här?" << endl;
	// 	for(unsigned int i = 0; i < it->second.theArticles.size(); ++i){
	// 		cout << "inne i loopen " << i << endl;
	// 		if(it->second.theArticles[i].id == articleID) {
	// 			cout << "hittade rätt artikel" << endl;
	// 			return it->second.theArticles[i];
	// 		}
	// 	}
	// 	throw ArticleDoesNotExistException();
	// 	cout << "throw: could not find article exception" << endl;

	// 	} else {
	// 		throw NewsGroupDoesNotExistException();
	// 		cout << "could not find group errer" << endl;
	// }
					cout << "THE CREATED author: " << author << endl;
					cout << "THE CREATED CONTENT: " << content << endl;

					

	Article myArticle(title, author, content);
	return myArticle;
}



/*----------------------------------------------------*/
