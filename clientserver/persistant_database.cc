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
#include <sstream>
#include <cstdio>		/*remove*/


using namespace std;


Persistant_Database::Persistant_Database(){

	int status;
 	status = mkdir("/h/d8/o/dat12jn2/DBRoot", S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
 	if(status == 0){
		cout << "Persistant_Database has been created!" << endl;
 	} else if(status == -1){
 		cout << "Successfully read existing database!" << endl;
 	}
	uniquenbr = 0;
	uniquenbr2 = 0;
}
	
/*---------------NewsGroup operations------------------*/
void Persistant_Database::createNewsGroup(string& theName){
	//NewsGroup myNewsGroup(theName);
	//myNewsGroup.id = ++uniquenbr;
	
	cout << "entering Persistant_Database createNG" << endl;

	//auto it = find_if(  db.begin(), db.end(), 
	//[&](pair<int, NewsGroup> ng){return ng.second.name == theName;});
	
	//cout << "it->first " << it->first << endl;
	//if(it == db.end()){
	//	cout << "inserting into map" << endl;
	//	db.insert(make_pair(uniquenbr, myNewsGroup));
		
		string lol = to_string(++uniquenbr);
		string dir = "/h/d8/o/dat12jn2/DBRoot/";
		dir += lol; // OM TO_STRING HADE FUNKAT MED FATTIGA CYGWIN KANSKE???!?!?!
		dir += " ";
		dir += theName;
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
	string dir = "/h/d8/o/dat12jn2/DBRoot";
	// idToBeRemoved NEEDS THE TO_STRING
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
		while(ss){
			ss >> id >> name;
		} 
		//THIS 1 SHOULD BE id if the fucking to_string would work...
				
		if(stoi(id) == idToBeRemoved){
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

	pdir=opendir("/h/d8/o/dat12jn2/DBRoot"); 
	if (!pdir){
		printf ("opendir() failure; terminating");
		exit(1);
	}
	errno=0;
	while ((pent=readdir(pdir))){
		printf("%s", pent->d_name);
		string tempstr(pent->d_name);

		stringstream ss(tempstr);
		string id;
		string name;
		while(ss){
			ss >> id >> name;
		} 

		cout << "id is:" << id << " name is: " << name << endl;
		NewsGroup myNewsGroup(name);
		myNewsGroup.id = atoi(id.c_str());
		
		if(atoi(id.c_str()) != 0) {
			tempVec.push_back(myNewsGroup);
		}
		
		for(auto it = tempVec.begin(); it < tempVec.end(); ++it){
			cout << "WRITING FORLOOP" << endl << endl;
			cout << it->name << it->id << endl;
		}

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
	Article myArticle(title, author, content);
	myArticle.id = ++uniquenbr2;
	
	auto it = db.find(groupID);
	if(it != db.end()){
		it->second.theArticles.push_back(myArticle);

	} else {
		throw NewsGroupDoesNotExistException();
		cout << "throw a no such group exceptoion" << endl;
	}
}
void Persistant_Database::deleteArticle(int groupID, int articleID){
	auto it = db.find(groupID);
	if(it != db.end()){
		
		auto remover = remove_if(it->second.theArticles.begin(), 
		it->second.theArticles.end(), 
		[&](Article myR){return myR.id == articleID;});
		
		if(remover == it->second.theArticles.end()){
			throw ArticleDoesNotExistException();
			cout << "throw: no such article found" << endl;
		}
		it->second.theArticles.erase(remover, it->second.theArticles.end());
 
	} else {
		throw NewsGroupDoesNotExistException();
		cout << "throw: no such group found" << endl;
	}
}
vector<Article> Persistant_Database::listArticles(int inNewsGroup){
	vector<Article> tempVec;
	
	auto it = db.find(inNewsGroup);
	if(it != db.end()){
		for(unsigned int i = 0; i < it->second.theArticles.size(); ++i){
			tempVec.push_back(it->second.theArticles[i]);
		}
	} else {
		throw NewsGroupDoesNotExistException();
		cout << "throw no such group found" << endl;
	}

	return tempVec;

}
Article Persistant_Database::readArticle(int groupID, int articleID){

	auto it = db.find(groupID);
	if(it != db.end()){
		
		cout << "går in här?" << endl;
		for(unsigned int i = 0; i < it->second.theArticles.size(); ++i){
			cout << "inne i loopen " << i << endl;
			if(it->second.theArticles[i].id == articleID) {
				cout << "hittade rätt artikel" << endl;
				return it->second.theArticles[i];
			}
		}
		throw ArticleDoesNotExistException();
		cout << "throw: could not find article exception" << endl;

		} else {
			throw NewsGroupDoesNotExistException();
			cout << "could not find group errer" << endl;
	}
	
}



/*----------------------------------------------------*/
