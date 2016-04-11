#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H

#include <exception>
#include <iostream>


struct NewsGroupAlreadyExistsException : public std::exception {
	const char * what() const throw (){
		return "NewsGroup with that name already exists.";
	}
};

struct NewsGroupDoesNotExistException : public std::exception {
	const char * what() const throw (){
		return "NewsGroup was not found/ does not exists";
	}
};

struct ArticleDoesNotExistException : public std::exception {
	const char * what() const throw() {
		return "The article was not found/ does not exists";
	}
};

#endif