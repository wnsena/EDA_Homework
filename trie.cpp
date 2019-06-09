//William
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <algorithm>
#include <set>
#include <math.h>
#include "text_processing.cpp"

using namespace std;


const unsigned int number_of_tries = 14165314;
const unsigned int number_of_indexes = 260289729;

struct disk_trie {
  unsigned int children[36];
  unsigned int kinship[2];
};

struct trie {
  trie *children[36];
  unsigned int index;
};

unsigned int branch_word, position_word, trie_position = 1;
void put_word_and_counter (disk_trie *tree, unsigned int *counters, const vector<int> &word) {
  branch_word = 0;
  for (position_word = 0; position_word != word.size(); position_word++) {
    if (!tree[branch_word].children[word[position_word]]) {
      tree[branch_word].children[word[position_word]] = trie_position;
      tree[trie_position].kinship[0] = branch_word;
      tree[trie_position].kinship[1] = word[position_word];
      trie_position++;
    };
    branch_word = tree[branch_word].children[word[position_word]];
  };
  counters[branch_word]++;
};

unsigned int branch_index, position_index, counter_index;
void put_index (unsigned int *indexes, const disk_trie *tree, const unsigned int *counters, const vector<int> &word, const unsigned int &index) {
  branch_index = 0;
  for (position_index = 0; position_index != word.size(); position_index++)
    branch_index = tree[branch_index].children[word[position_index]];
  counter_index = counters[branch_index];
  indexes[counter_index]++;
  indexes[counter_index + indexes[counter_index]] = index;
};

void accumulate_counters (unsigned int *counters) {
  unsigned int current, accumulated = 0;
  for(unsigned int i = 0; i != trie_position; i++) {
    current = counters[i];
    counters[i] = accumulated;
    accumulated += (current + 1);
  };
  cout << accumulated << " indexes." << endl;
  cout << trie_position << " tries." << endl;
};

trie *branch_search; unsigned int position_search;
unsigned int search(trie *tree, const vector<int> &word) {
  branch_search = tree;
  for (position_search = 0; position_search != word.size(); position_search++) {
    branch_search = branch_search->children[word[position_search]];
    if (!branch_search)
      return 0;
  };
  return branch_search->index;
};


trie *load_trie (const disk_trie *disk_tree) {
  vector<trie *> pointers(number_of_tries);
  pointers[0] = new trie();
  pointers[0]->index = 0;
  for (unsigned int position = 1; position != number_of_tries; position++) {
    pointers[position] = new trie();
    pointers[position]->index = position;
    pointers[disk_tree[position].kinship[0]]->children[disk_tree[position].kinship[1]] = pointers[position];
  };
  return pointers[0];
};

void read_and_insert(trie* tree, vector<PTI*>& Titulos) {
	vector < vector <int> > Line_words;
	string titulo, line;
	int i = 0;
	size_t inicio_do_titulo, fim_do_titulo;
	PTI* novo_titulo;
	for (int page_index = 1; page_index < 16; page_index++) {
		ifstream File("wikiCorpus (" + to_string(page_index) + ")");
		if (File.is_open()) {
			while (getline(File, line)) {
				if (line == "ENDOFARTICLE.") {
					i = -1;
				}
				else {
					i++;
					if (i < 2) {
						inicio_do_titulo = line.find("title");
						if (inicio_do_titulo != string::npos) {
							inicio_do_titulo += 7;
							fim_do_titulo = line.find(" non");
							titulo = line.substr(inicio_do_titulo, fim_do_titulo - inicio_do_titulo - 1);
							novo_titulo = new PTI();
							novo_titulo->titulo = titulo;
							novo_titulo->indice = Titulos.size() + 1;
							Titulos.push_back(novo_titulo);
						}
					}
					else {
						convert(line);
						Line_words = word_breaker(line);
						for (vector < vector <int> >::iterator it = Line_words.begin(); it != Line_words.end(); it++) {
							if (!it->empty()) {
								put_word(tree, *it, Titulos.size());
							}
						}
					}
				}
			}
			File.close();
		}
	}
}

void read_and_insert2(trie* tree) {
	//variables used
	vector < vector <int> > Line_words;
	string line;
	int text_counter = 1;
	ifstream File("sorted_text.txt");

	if (File.is_open())
	{
		while (getline(File, line))
		{
			convert(line);
			Line_words = word_breaker(line);
			for (vector < vector <int> >::iterator it = Line_words.begin(); it != Line_words.end(); it++)
			{
				if (!it->empty())    put_word(tree, *it, text_counter);
			}
			text_counter++;
		}
		File.close();
	}
}
