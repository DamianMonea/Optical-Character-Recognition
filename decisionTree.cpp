// copyright Luca Istrate, Andrei Medar

#include "./decisionTree.h"  // NOLINT(build/include)
#include <algorithm>
#include <cassert>
#include <cmath>
#include <iostream>
#include <limits>
#include <memory>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

using std::string;
using std::pair;
using std::vector;
using std::unordered_map;
using std::make_shared;

// structura unui nod din decision tree
// splitIndex = dimensiunea in functie de care se imparte
// split_value = valoarea in functie de care se imparte
// is_leaf si result sunt pentru cazul in care avem un nod frunza
Node::Node() {
    is_leaf = false;
    left = nullptr;
    right = nullptr;
}

void Node::make_decision_node(const int index, const int val) {
    split_index = index;
    split_value = val;
}

void Node::make_leaf(const vector<vector<int>> &samples,
                     const bool is_single_class) {
    // TODO(you)
    // Seteaza nodul ca fiind de tip frunza (modificati is_leaf si result)
    // is_single_class = true -> toate testele au aceeasi clasa (acela e result)
    // is_single_class = false -> se alege clasa care apare cel mai des
    if (is_single_class == true) {
        result = samples[0][0];
        is_leaf = true;
    } else {
        int samplesSize = samples.size();
        vector<int> most_wanted(10, 0);
        for (int i = 0; i < samplesSize; ++i) {
            most_wanted[samples[i][0]]++;
        }
        int max = std::numeric_limits<int>::min();
        for (int i = 0; i < 10; ++i) {
            if (most_wanted[i] > max) {
                max = most_wanted[i];
                result = i;
            }
        }
        is_leaf = true;
    }
}

float calculateInformationGain(const vector<vector<int>>& samples,
                                int splitIndex, int splitValue){
    if (samples.empty()) {
        return -1;
    }
    float result = 0;
    result += get_entropy(samples);
    auto children = get_split_as_indexes(samples, splitIndex, splitValue);
    vector<int>& left = children.first;
    vector<int>& right = children.second;
    if (left.empty() || right.empty()) {
        return -1;
    }
    result -= (left.size() * get_entropy_by_indexes(samples, left)
            + right.size() * get_entropy_by_indexes(samples, right))
            /samples.size();
    return result;
}

pair<int, int> find_best_split(const vector<vector<int>> &samples,
                               const vector<int> &dimensions) {
    // TODO(you)
    // Intoarce cea mai buna dimensiune si valoare de split dintre testele
    // primite. Prin cel mai bun split (dimensiune si valoare)
    // ne referim la split-ul care maximizeaza IG
    // pair-ul intors este format din (split_index, split_value)
    float aux;
    float maxim = 0;
    int dimSize = dimensions.size();
    int samplesSize = samples.size();
    int splitIndex = -1, splitValue = -1;
    vector<int> unique_values;
    for (int i = 0; i < dimSize; ++i){
        unique_values = compute_unique(samples, dimensions[i]);
        for (auto value : unique_values){
            aux = calculateInformationGain(samples, dimensions[i], value);
            if (aux > maxim) {
                maxim = aux;
                splitValue = value;
                splitIndex = dimensions[i];
            }
        }
    }
    return pair<int, int>(splitIndex, splitValue);
}

void Node::train(const vector<vector<int>> &samples) {
    // TODO(you)
    // Antreneaza nodul curent si copii sai, daca e nevoie
    // 1) verifica daca toate testele primite au aceeasi clasa (raspuns)
    // Daca da, acest nod devine frunza, altfel continua algoritmul.
    // 2) Daca nu exista niciun split valid, acest nod devine frunza. Altfel,
    // ia cel mai bun split si continua recursiv
    bool is_single_class = same_class(samples);
    if (is_single_class == true) {
        make_leaf(samples, is_single_class);
        return;
    } else {
        vector<int> dimensions = random_dimensions(samples[0].size());
        pair<int, int> find_split = find_best_split(samples, dimensions);
        int index = find_split.first;
        int val = find_split.second;
        if (val == -1 && index == -1) {
            make_leaf(samples, is_single_class);
            return;
        } else {
            this->make_decision_node(index, val);
            left = std::make_shared<Node>();
            right = std::make_shared<Node>();
            auto split_samples = split(samples, index, val);
            left->train(split_samples.first);
            right->train(split_samples.second);
        }
    }
}

int Node::predict(const vector<int> &image) const {
    // TODO(you)
    // Intoarce rezultatul prezis de catre decision tree
    if (is_leaf == true) {
        return result;
    }
    if (image[split_index - 1] <= split_value) {
        return left->predict(image);
    } else {
        return right->predict(image);
    }
}

bool same_class(const vector<vector<int>> &samples) {
    // TODO(you)
    // Verifica daca testele primite ca argument au toate aceeasi
    // clasa(rezultat). Este folosit in train pentru a determina daca
    // mai are rost sa caute split-uri
    int samplesSize = samples.size();
    int first = samples[0][0];
    for (int i = 1; i < samplesSize; ++i) {
        if (first != samples[i][0]) {
            return false;
        }
    }
    return true;
}

float get_entropy(const vector<vector<int>> &samples) {
    // Intoarce entropia testelor primite
    assert(!samples.empty());
    vector<int> indexes;

    int size = samples.size();
    for (int i = 0; i < size; i++) indexes.push_back(i);

    return get_entropy_by_indexes(samples, indexes);
}

float get_entropy_by_indexes(const vector<vector<int>> &samples,
                             const vector<int> &index) {
    // TODO(you)
    // Intoarce entropia subsetului din setul de teste total(samples)
    // Cu conditia ca subsetul sa contina testele ale caror indecsi se gasesc in
    // vectorul index (Se considera doar liniile din vectorul index)
    float entropy = 0.0f;
    float indexSize = index.size();
    vector<int> no_tests(10, 0);
    for (int i : index) {
        ++no_tests[samples[i][0]];
    }
    //std::cout << no_tests[samples[indexSize - 1][0]] << " ";
    for (int j : no_tests) {
        if (j != 0) {
           // std::cout << no_tests[j] << " ";
            float p = (float)(j / indexSize);
           // std::cout << p << " "<< std::endl;
            entropy -= p * log2(p); 
        }
    }
    return entropy;
}

vector<int> compute_unique(const vector<vector<int>> &samples, const int col) {
    // TODO(you)
    // Intoarce toate valorile (se elimina duplicatele)
    // care apar in setul de teste, pe coloana col
    vector<int> uniqueValues;
    vector<bool> freq(256, false);
    int samplesSize = samples.size();
    for (int i = 0; i < samplesSize; i++)
        freq[samples[i][col]] = true;
    for (int i = 0; i < 256; i++) {
        if (freq[i] == true) {
            uniqueValues.push_back(i);
        }
    }
    return uniqueValues;
}

pair<vector<vector<int>>, vector<vector<int>>> split(
    const vector<vector<int>> &samples, const int split_index,
    const int split_value) {
    // Intoarce cele 2 subseturi de teste obtinute in urma separarii
    // In functie de split_index si split_value
    vector<vector<int>> left, right;

    auto p = get_split_as_indexes(samples, split_index, split_value);
    for (const auto &i : p.first) left.push_back(samples[i]);
    for (const auto &i : p.second) right.push_back(samples[i]);

    return make_pair(left, right);
}

pair<vector<int>, vector<int>> get_split_as_indexes(
    const vector<vector<int>> &samples, const int split_index,
    const int split_value) {
    // TODO(you)
    // Intoarce indecsii sample-urilor din cele 2 subseturi obtinute in urma
    // separarii in functie de split_index si split_value
    vector<int> left, right;
    int samplesSize = samples.size();
    for (int i = 0; i < samplesSize; ++i){
        if (samples[i][split_index] <= split_value){
            left.push_back(i);
        }
        if (samples[i][split_index] > split_value){
            right.push_back(i);
        }
    }
    return make_pair(left, right);
}

vector<int> random_dimensions(const int size) {
    // TODO(you)
    // Intoarce sqrt(size) dimensiuni diferite pe care sa caute splitul maxim
    // Precizare: Dimensiunile gasite sunt > 0 si < size
    unsigned int randValue = (unsigned int) size;
    vector<int> result;
    vector<bool> generated(size, false);
    int to_generate = floor(sqrt(size));
    while(to_generate) {
        int tmp = 1 + rand_r(&randValue) % (size - 1);
        if(!generated[tmp]) {
            generated[tmp] = true;
            result.push_back(tmp);
            to_generate--;
        }
    }
    return result;
}
