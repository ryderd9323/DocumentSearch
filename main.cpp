//
// Project 2 - Search
// Builds inverted index of words and sites on which they can be found, allows 
// user to search specific phrases with modifiers and prints list of webpage 
// URLs whose contents match the query
//
// Ryder Douglas
// Creative component: buildStopWords(), a function that builds a set of stop
// words (words so common in a language that they have little to no effect on 
// search results, or have a negative effect on them). The index is then
// searched for any words in the set of stop words, which are removed.
// It requires a file, stopWords.txt, which is included in my submission. I
// wasn't sure whether additional file requirements would screw anything up
// with my submission, though, so the function call is commented out and 
// a hardcoded set of stop words is used instead.
//

#include <iostream>
#include <vector>
#include "search.h"

using namespace std;

void runTests() {

    // cleanToken testing
    string ans = "hello";
    string empty = "";

    cout << "testing cleanToken(): " << endl;
    cout << "ans is " << ans
         << "\n1. " << (ans == cleanToken(".hello"))
         << "\n2. " << (ans == cleanToken("..hello!"))
         << "\n3. " << (ans == cleanToken("HeLlO!")) << endl;

    cout << "\ntesting all-punctuation tokens" << endl
         << "4. " << (empty == cleanToken(".-!"))
         << "\n4b. " << (empty == cleanToken("2343241")) << endl;

    ans = "doesn't";

    cout << "\nans is now '" << ans << "'" << endl
         << "5. " << (ans == cleanToken("doesn't"))
         << "\n6. " << (ans == cleanToken("...doesn't"))
         << "\n7. " << (ans == cleanToken("doesn't!")) << endl;
	
	// Creative component
	// buildStopWords testing
	set<string> stopWords = buildStopWords();
	vector<string> stopTest = {"across", "after", "against", "along", "an", "and", "another", "any", "around", "as", "at", "before"};

	for (string word : stopTest) {
		if (!stopWords.count(word))
			cout << "Found a stopword :(" << endl;
	}

    // gatherTokens testing
    set<string> tokens = gatherTokens("to be be or not to be, is it?");
    set<string> seuss = gatherTokens("One Fish Two Fish Red fish bLuE fiSh !!!");

    set<string> tokensAns;
    set<string> seussAns;

    cout << endl << "testing gatherTokens: " << endl;

	tokensAns.insert("to");
	tokensAns.insert("be");
	tokensAns.insert("or");
	tokensAns.insert("not");
	tokensAns.insert("is");
	tokensAns.insert("it");

	cout << "1. " << (tokens.size() == 6) << endl
		 << "2. " << (tokens == tokensAns) << endl;

	seussAns.insert("one");
	seussAns.insert("fish");
	seussAns.insert("two");
	seussAns.insert("red");
	seussAns.insert("blue");

	cout << endl << "Seuss: " << endl
		 << "1. " << (seuss.size() == 5) << endl
		 << "2. " << (seuss == seussAns) << endl;

    // buildIndex testing
    map<string, set<string>> index;
    map<string, set<string>> indexANS;
	set<string> urls;
	string filename;

    cout << "\ntesting buildIndex: " << endl;

	filename = "tiny.txt";

	tokens = {"eggs", "milk", "fish", "bread", "cheese"};
	urls.insert("www.shoppinglist.com");
	for (string tkn : tokens) {
		indexANS.emplace(tkn, urls);
	}

	tokens = {"red", "green", "orange", "yellow", "blue",
			"indigo", "violet"};
	urls.clear();
	urls.insert("www.rainbow.org");
	for (string tkn : tokens) {
		indexANS.emplace(tkn, urls);
	}

	tokens =  {"one", "two"};
	urls.clear();
	urls.insert("www.dr.seuss.net");
	for (string tkn : tokens) {
		indexANS.emplace(tkn, urls);
	}

	tokens = {"i'm", "not", "trying", "to", "eat", "you"};
	urls.clear();
	urls.insert("www.bigbadwolf.com");
	for (string tkn : tokens) {
		indexANS.emplace(tkn, urls);
	}

	indexANS["fish"].insert("www.dr.seuss.net");
	indexANS["red"].insert("www.dr.seuss.net");
	indexANS["blue"].insert("www.dr.seuss.net");

	buildIndex(filename, index);
	cout << "\n1. " << (index.size() == 20)
		<< "\n2. " << (index == indexANS) << endl;

	tokens = {"eggs", "milk", "fish", "bread", "cheese", "red", "green",
			"orange", "yellow", "blue", "indigo", "violet", "one",
			"two", "i'm", "not", "trying", "to", "eat", "you"};

	cout << "3. " << (buildIndex("bad.txt", index) == 0) << endl;

    // findQueryMatches testing
    set<string> results;
    string query;

    query = "Student +lounge software -engineering";
    buildIndex("uiccs-news.txt", index);
    results = findQueryMatches(index, query);

    cout << "Search results: " << endl;
    for (string s : results) {
        cout << s << endl;
    }
}

void findQueryTiny() {
	map<string, set<string>> index;
	set<string> results;
	string query;

	query = "eggs";
	buildIndex("tiny.txt", index);
	results = findQueryMatches(index, query);

	cout << "Search results for tiny.txt: " << endl;
	for (string s : results) {
		cout << s << endl;
	}
}

int main() {
	findQueryTiny();
    //searchEngine("tiny.txt");

    return 0;
}

