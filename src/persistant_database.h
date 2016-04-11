#ifndef PERSISTANT_DATABASE_H
#define PERSISTANT_DATABASE_H

#include <string>
#include <map>
#include <vector>

class Article {
public:
	std::string title;
	std::string author;
	std::string content;
	int id;
	Article() {}
	Article(std::string& title, std::string& author, 
			std::string& content) : title(title), author(author)
			, content(content) {}
	//Article()
};

class NewsGroup {
public:
	std::string name;
	std::vector<Article> theArticles;
	int id;
	NewsGroup(std::string& name) : name(name) {}

};

class Persistant_Database {
public:
	Persistant_Database();
	
	/*NewsGroup operations*/
	void createNewsGroup(std::string& theName);
	void deleteNewsGroup(int idToBeRemoved);
	std::vector<NewsGroup> listNewsGroup();

	/*Article operations*/
	void createArticle(	std::string& title, std::string& author, 
						std::string& content, int groupID);
	void deleteArticle(int groupID, int articleID);
	std::vector<Article> listArticles(int inNewsGroup);
	Article readArticle(int groupID, int articleID);

private:
	std::map<int, NewsGroup> db;
	int uniquenbr;
	int uniquenbr2;
};


#endif