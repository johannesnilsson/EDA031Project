#include "cache_database.h"
#include <iostream>
#include <algorithm>
#include "exceptions.h"


using namespace std;


Cache_Database::Cache_Database(){

	cout << "Cache_Database has been created" << endl;
	uniquenbr = 0;
	uniquenbr2 = 0;
}

/*---------------NewsGroup operations------------------*/
void Cache_Database::createNewsGroup(string& theName){
	NewsGroup myNewsGroup(theName);
	myNewsGroup.id = ++uniquenbr;

	auto it = find_if(  db.begin(), db.end(),
	[&](pair<int, NewsGroup> ng){return ng.second.name == theName;});

	if(it == db.end()){
		db.insert(make_pair(uniquenbr, myNewsGroup));
	} else {
		throw NewsGroupAlreadyExistsException();
	}
}

void Cache_Database::deleteNewsGroup(int idToBeRemoved){
	auto it = db.find(idToBeRemoved);
	if(it != db.end()){
		db.erase(it);
	} else {
		throw NewsGroupDoesNotExistException();
	}
}

vector<NewsGroup> Cache_Database::listNewsGroup(){
	vector<NewsGroup> tempVec;

	for(auto it = db.begin(); it != db.end(); ++it){
		tempVec.push_back(it->second);
	}

	return tempVec;
}

/*----------------------------------------------------*/

/*----------------Article operations------------------*/
void Cache_Database::createArticle(	string& title, string& author,
									string& content, int groupID){
	Article myArticle(title, author, content);
	myArticle.id = ++uniquenbr2;

	auto it = db.find(groupID);
	if(it != db.end()){
		it->second.theArticles.push_back(myArticle);

	} else {
		throw NewsGroupDoesNotExistException();
	}
}
void Cache_Database::deleteArticle(int groupID, int articleID){
	auto it = db.find(groupID);
	if(it != db.end()){

		auto remover = remove_if(it->second.theArticles.begin(),
		it->second.theArticles.end(),
		[&](Article myR){return myR.id == articleID;});

		if(remover == it->second.theArticles.end()){
			throw ArticleDoesNotExistException();
		}
		it->second.theArticles.erase(remover, it->second.theArticles.end());

	} else {
		throw NewsGroupDoesNotExistException();
	}
}
vector<Article> Cache_Database::listArticles(int inNewsGroup){
	vector<Article> tempVec;

	auto it = db.find(inNewsGroup);
	if(it != db.end()){
		for(unsigned int i = 0; i < it->second.theArticles.size(); ++i){
			tempVec.push_back(it->second.theArticles[i]);
		}
	} else {
		throw NewsGroupDoesNotExistException();
	}
	return tempVec;
}

Article Cache_Database::readArticle(int groupID, int articleID){
	auto it = db.find(groupID);
	if(it != db.end())
	{

		for(unsigned int i = 0; i < it->second.theArticles.size(); ++i){
			if(it->second.theArticles[i].id == articleID) {
				return it->second.theArticles[i];
			}
		}
		throw ArticleDoesNotExistException();

		} else {
			throw NewsGroupDoesNotExistException();
	}
}
/*----------------------------------------------------*/
