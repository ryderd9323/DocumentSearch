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

#pragma once

#include <iostream>
#include <set>
#include <map>
#include <sstream>
#include <fstream>
#include <algorithm>
using namespace std;


// takes whitespace-separated string of characters, return cleaned version
// of that string, less leading and trailing punctuation and converted to lowercase
string cleanToken(string s) {
    string lower;
    bool hasAlpha = false;

    while (ispunct(s.front())) {
        s.erase(0, 1);
        if (s.empty()) {
            return "";
        }
    }

    while (ispunct(s.back())) {
        s.pop_back();
    }

    lower = "";

    for (char c : s) {
        if (isalpha(c))
            hasAlpha = true;
        lower += tolower(c);
    }
    return (hasAlpha) ? lower : "";
}

// takes string containing body text from page, returns set of cleaned, unique tokens
// that appear in text
set<string> gatherTokens(string text) {
    set<string> tokens;
    stringstream ss(text);
    string tkn;
    
    while (ss >> tkn) {
        tkn = cleanToken(tkn);
        if (tkn.empty())
            continue;
        tokens.insert(tkn);
    }
    
    return tokens;
}

// Reads file containing a list of stop words, adds each word to a set
// to be checked when building index.
// Requires additional file which might be an issue, so I'll 
// comment out the function call and build the set manually, but will include
// the file in my submission
set<string> buildStopWords() {
    set<string> stopWords;
    ifstream inFS;
    string text;

    inFS.open("stopWords.txt");

    while (!inFS.eof()) {
		getline(inFS, text, '\r');
		if (text[0] == '#')
			continue;
		stopWords.insert(text);
	}

	return stopWords;
}

// populates index map. Keys are words, values are sets of pages containing that word.
// Return value is number of pages processed
int buildIndex(string filename, map<string, set<string>>& index) {
    int count;
	string url, bodyText;
	set<string> tokens;
	set<string> vals;   // holds url

	// create filestream
    fstream fs(filename);
    if (!fs.is_open())
        return 0;

    count = 0;
	while (!fs.eof()) {
        vals.clear();
		getline(fs, url, '\n');			// Read URL
        if (url.size() < 1) {
            continue;
        }
		count++;
		getline(fs, bodyText, '\n');	// Read body text
		tokens = gatherTokens(bodyText);
		for (string tkn : tokens) {
            if (!index.count(tkn)) {	// if tkn NOT in index
				vals.insert(url);       // start set of URLS
				index.emplace(tkn, vals); // and put tkn and set into index
			} else {
                // Otherwise, index entry already exists, so just find the
                // entry and add URL to tkn's set
				index.at(tkn).insert(url); 
			}
		}
	}
    return count;
}

// returns set of all sites containing query words. Parameters are reference to index map
// and query words as string
set<string> findQueryMatches(map<string, set<string>>& index, string sentence) {
    set<string> result;
    set<string> termSites;
    string term;
    char mod;
    stringstream ss(sentence);

    set<string> temp;

    // Iterate through query sequence word by word, saving modifier and cleaning token.
    // Searches index for term and, if an entry exists, adds sites to results as
    // determined by query modifier
    while (ss >> term) {
        mod = term[0];
        term = cleanToken(term);
        termSites = index.at(term);

        // check that word exists in index
        if (!index.count(term))
            continue;

        // add sites to temp set according to set operation as determined by modifier
        switch (mod)
        {
        case '+':
            set_intersection(result.begin(), result.end(), termSites.begin(), termSites.end(), inserter(temp, temp.begin()));
            break;

        case '-':
            set_difference(result.begin(), result.end(), termSites.begin(), termSites.end(), inserter(temp, temp.begin()));
            break;

        default:
            set_union(result.begin(), result.end(), termSites.begin(), termSites.end(), inserter(temp, temp.begin()));
            break;
        }
        result = temp;
        temp.clear();
    }
    
    
    return result;  // TODO:  update this.
}

// Constructs index, prints number of pages processed and distinct words found
// while building index, prompts user for query, finds matching pages, prints
// urls, repeats until user enters empty string
void searchEngine(string filename) {
    int pageCount, stopCount;
	map<string, set<string>> index;
	set<string > results;
    set<string> stopWords;
	string query;
	
	cout << "Stand by while building index..." << endl;

    pageCount = buildIndex(filename, index);

    // Creative component, buildStopWords() requires additional file which might
    // cause issues, so I'm commenting it out and building the set manually

    //stopWords = buildStopWords();
    stopWords = {"a", "aboard", "about", "above", "across", "after", 
    "against", "along", "an", "and", "another", "any", "around", "as", "at", 
    "before", "behind", "below", "beneath", "beside", "between", "beyond", 
    "but", "by", "certain", "down", "during", "each", "every", "except", 
    "following", "for", "from", "her", "his", "in", "inside", "into", "its", 
    "like", "minus", "my", "near", "next", "no", "nor", "of", "off", "on", 
    "onto", "opposite", "or", "our", "out", "outside", "over", "past", "plus", 
    "round", "since", "so", "some", "than", "that", "the", "their", "this", 
    "through", "to", "toward", "under", "underneath", "unlike", "until", "up", 
    "upon", "with", "without", "yet"};

    // To match output (number of unique terms), keep a count of stop words
    // removed from index
    stopCount = 0;
    for (string word : stopWords) {
        if (index.count(word)) {
            stopCount += index.erase(word);
        }
    }

	cout << "Indexed " << pageCount << " pages containing " << stopCount + index.size()
		 << " unique terms" << endl;

	while (true) {
		cout << "\nEnter query sentence (press enter to quit): ";
		getline(cin, query);
		if (query == "") {
			cout << "Thank you for searching!" << endl;
			return;
		}
		results = findQueryMatches(index, query);
		cout << "Found " << results.size() << " matching pages" << endl;
		for (string url : results) {
			cout << url << endl;
		}
	}
}


