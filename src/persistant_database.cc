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
 	status = mkdir("../src/DBRoot", S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);

	if(status == 0){
		cout << "Persistant_Database has been created!" << endl;
 	} else if(status == -1){
 		cout << "Successfully read existing database!" << endl;
 	}

 	int maxID = 0;
	DIR *pdir;
	struct dirent *pent;

	pdir=opendir("../src/DBRoot");
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

		string newdir = "../src/DBRoot/";
		newdir += pent->d_name;
		DIR *pdir2;
		struct dirent *pent2;

		pdir2=opendir(newdir.c_str());
		if (!pdir2){
			printf ("opendir() failure; terminating");
			throw NewsGroupDoesNotExistException();
			exit(1);
		}
		errno=0;
		while ((pent2=readdir(pdir2))){
			 if (!strcmp(pent2->d_name, ".") || !strcmp(pent2->d_name, ".."))
	          {
	             continue;
	          }

			string tempstr(pent2->d_name);

			stringstream ss(tempstr);
			string id;
			ss >> id;
			if(atoi(id.c_str()) > maxID){
				maxID2 = atoi(id.c_str());
			}
		}
		if (errno){
		printf ("readdir() failure; terminating");
		exit(1);
		}
		closedir(pdir2);

		if(atoi(id.c_str()) > maxID){
			maxID = atoi(id.c_str());
		}
	}

	if (errno){
	printf ("readdir() failure; terminating");
	exit(1);
	}
	closedir(pdir);

	uniquenbr = maxID;
	uniquenbr2 = maxID2;
}

/*---------------NewsGroup operations------------------*/
void Persistant_Database::createNewsGroup(string& theName){
	string tempname;
	DIR *pdir2;
	struct dirent *pent2;

	pdir2=opendir("../src/DBRoot");
	if (!pdir2){
		printf ("opendir() failure; terminating");
		throw NewsGroupDoesNotExistException();
		exit(1);
	}
	errno=0;
	while ((pent2=readdir(pdir2))){
		 if (!strcmp(pent2->d_name, ".") || !strcmp(pent2->d_name, ".."))
					{
						 continue;
					}

		string tempstr(pent2->d_name);

		stringstream ss(tempstr);
		string id;
		string name;

		ss >> id;
		while(ss >> name){
			ss >> name;
			tempname += name;
		}
		if(tempname == theName){
			throw NewsGroupAlreadyExistsException();
		} else {
			tempname.clear();
		}
	}
	if (errno){
		printf ("readdir() failure; terminating");
		exit(1);
	}
	closedir(pdir2);

		int temp = ++uniquenbr;
		string dir = "../src/DBRoot/";
		dir += to_string(temp);
		dir += " ";
		dir += theName;
		int status = mkdir((char*)dir.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
}

void Persistant_Database::deleteNewsGroup(int idToBeRemoved){
	bool rmFlag = false;
	DIR *pdir;
	struct dirent *pent;
	string dir = "../src/DBRoot";

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

		if(atoi(id.c_str()) == idToBeRemoved)
		{
			string temp = dir;
			temp += "/";
			temp += id;
			temp += "*";
			//dir += "/1*";
			//dir += pent->d_name;
			string command = "rm -rf ";
			command += temp;
			system( command.c_str() );
			rmFlag = true;
			temp.clear();
		}
	}
	if(!rmFlag){
		throw NewsGroupDoesNotExistException();
	}
}

vector<NewsGroup> Persistant_Database::listNewsGroup(){
	vector<NewsGroup> tempVec;
	DIR *pdir;
	struct dirent *pent;

	pdir=opendir("../src/DBRoot");
	if (!pdir){
		printf ("opendir() failure; terminating");
		exit(1);
	}
	errno=0;
	while ((pent=readdir(pdir)))
	{
		if (!strcmp(pent->d_name, ".") || !strcmp(pent->d_name, ".."))
          {
             continue;
          }
		string tempstr(pent->d_name);

		stringstream ss(tempstr);
		string id;
		string name;

		string tempname;
		ss >> id;
		while(ss >> name)
		{
			tempname += name;
			tempname += " ";
		}
		NewsGroup myNewsGroup(tempname);
		myNewsGroup.id = atoi(id.c_str());

		if(atoi(id.c_str()) != 0) {
			tempVec.push_back(myNewsGroup);
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
	bool createflag = false;
	DIR *pdir;
	struct dirent *pent;
	pdir=opendir("../src/DBRoot");
	if (!pdir){
		//printf ("opendir() failure; terminating");
		exit(1);
	}
	errno=0;
	while ((pent=readdir(pdir))){
		if (!strcmp(pent->d_name, ".") || !strcmp(pent->d_name, ".."))
          {
             continue;
          }
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
			string articleDir = "../src/DBRoot/";
			articleDir += pent->d_name;
			articleDir += "/";
			articleDir += to_string(temp);
			articleDir += " ";
			articleDir += title;
			articleDir += ".txt";

			ofstream outputFile(articleDir);
			outputFile << "Article by: " << author << endl << "\n" << content << endl;
		}
		//if(!createflag){
		//	cout << "Could not find specified NewsGroup!" << endl;
		//	throw NewsGroupDoesNotExistException();
	//	}
	}
	if(!createflag){
		cout << "Could not find specified NewsGroup!" << endl;
		throw NewsGroupDoesNotExistException();
	}
	if (errno){
	printf ("readdir() failure; terminating");
	exit(1);
	}
	closedir(pdir);
}
void Persistant_Database::deleteArticle(int groupID, int articleID){

	string tempdir = "../src/DBRoot/";
	bool foundNG = false;
	bool foundArt = false;
	DIR *pdir;
	struct dirent *pent;
	pdir=opendir("../src/DBRoot");
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

		if(atoi(id.c_str()) == groupID){
			foundNG = true;

			DIR *pdir2;
			struct dirent *pent2;
			string dir = "../src/DBRoot/";
			dir += pent->d_name;
			tempdir += id;
			tempdir += "\\ ";
			tempdir += tempname;

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
				string tempstr(pent2->d_name);

				stringstream ss(tempstr);
				string id;
				string name;
				ss >> id;
				while(ss){
					ss >> name;
				}

				if(atoi(id.c_str()) == articleID){
					foundArt = true;
					string command = "rm -f ";
					command += tempdir;
					command += "/";
					command += id;
					//command += "\\ ";
					command += "*";
					//command += name;
					//command += "*";
					system(command.c_str());


				}

	}
	if(!foundNG){
		cout << "Could not find specified NewsGroup!" << endl;
		throw NewsGroupDoesNotExistException();
	}
	if(!foundArt){
		cout << "Could not find the specified Article!" << endl;
		throw ArticleDoesNotExistException();
	}

	if (errno){
	printf ("readdir() failure; terminating");
	exit(1);
	}
	closedir(pdir2);

		}
	}
	if (errno){
	printf ("readdir() failure; terminating");
	exit(1);
	}
	closedir(pdir);

	if(!foundNG){
			throw NewsGroupDoesNotExistException();
		}
}

vector<Article> Persistant_Database::listArticles(int inNewsGroup){
	vector<Article> tempVec;

	DIR *pdir;
	struct dirent *pent;

	pdir=opendir("../src/DBRoot");
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
		string tempstr(pent->d_name);

		stringstream ss(tempstr);
		string id;
		string name;
		ss >> id;
		while(ss){
			ss >> name;
		}
		/*--------read2 start -------------------*/
		if(atoi(id.c_str()) == inNewsGroup){
			DIR *pdir2;
			struct dirent *pent2;

			string dir = "../src/DBRoot/";
			dir += pent->d_name;
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
				string lol = "";
				string lol2 = "";

				Article myArticle(tempname, lol, lol2);
				myArticle.id = atoi(id.c_str());

				if(atoi(id.c_str()) != 0) {
					tempVec.push_back(myArticle);
				}
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

	return tempVec;
}

Article Persistant_Database::readArticle(int groupID, int articleID){
	string title;
	string author;
	string content;
	bool readFlag = false;

	DIR *pdir;
	struct dirent *pent;
	pdir=opendir("../src/DBRoot");
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
		string tempstr(pent->d_name);

		stringstream ss(tempstr);
		string id;
		string name;
		ss >> id;
		while(ss){
			ss >> name;
		}

		if(atoi(id.c_str()) == groupID){
			/*SECOND READ*/
			DIR *pdir2;
			struct dirent *pent2;
			string newdir = "../src/DBRoot/";
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
				if(atoi(id.c_str()) == articleID){
					for(unsigned int i = 0; i < tempname.length() - 4; ++i){
						title += tempname[i];
					}

					string articleDir = "../src/DBRoot/";
					articleDir += pent->d_name;
					articleDir += "/";
					articleDir += id;
					articleDir += " ";
					articleDir += tempname;
					articleDir.pop_back();
					ifstream inputFile(articleDir);
					readFlag = true;

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

	if(readFlag){
		Article myArticle(title, author, content);
		return myArticle;
	} else {
		throw ArticleDoesNotExistException();
	}
}
/*----------------------------------------------------*/
