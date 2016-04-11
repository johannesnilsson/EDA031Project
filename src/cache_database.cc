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
	cout << "entering Cache_Database createNG" << endl;

	auto it = find_if(  db.begin(), db.end(), 
	[&](pair<int, NewsGroup> ng){return ng.second.name == theName;});
	
	cout << "it->first " << it->first << endl;
	if(it == db.end()){
		cout << "inserting into map" << endl;
		db.insert(make_pair(uniquenbr, myNewsGroup));
	} else {
		throw NewsGroupAlreadyExistsException();
		cout << "throw: group already exists" << endl;
	}
}

void Cache_Database::deleteNewsGroup(int idToBeRemoved){
	cout << "id to remove " << idToBeRemoved << endl;
	auto it = db.find(idToBeRemoved);
	if(it != db.end()){
		db.erase(it);
	} else {
		throw NewsGroupDoesNotExistException();
		cout << "throw no such newsGruop to delete" << endl;
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
		cout << "throw a no such group exceptoion" << endl;
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
			cout << "throw: no such article found" << endl;
		}
		it->second.theArticles.erase(remover, it->second.theArticles.end());
 
	} else {
		throw NewsGroupDoesNotExistException();
		cout << "throw: no such group found" << endl;
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
		cout << "throw no such group found" << endl;
	}

	return tempVec;

}
Article Cache_Database::readArticle(int groupID, int articleID){

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