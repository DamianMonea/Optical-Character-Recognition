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
        vector<int> most_wanted(samplesSize, 0);
        for (int i = 0; i < samplesSize - 1; ++i) {
            for (int j = i; j < samplesSize; ++j)
            if (samples[i][0] == samples[j][0]) {
                most_wanted[i]++;
            }
        }
        int max = std::numeric_limits<int>::min();
        for (int i = 0; i < samplesSize; ++i) {
            if (most_wanted[i] > max) {
                max = most_wanted[i];
                result = samples[i][0];
            }
        }
        is_leaf = true;
    }
}

float calculateInformationGain(const vector<vector<int>>& samples,
                                int splitIndex, int splitValue){
    float result = 0;
    if (!samples.empty()) {
        result += get_entropy(samples);
    }
    pair<vector<int>, vector<int>> children =
    get_split_as_indexes(samples, splitIndex, splitValue);
    vector<int> left = children.first;
    vector<int> right = children.second;
    if (!left.empty() && !right.empty()) {
        result -= (left.size() * get_entropy_by_indexes(samples, left)
                + right.size() * get_entropy_by_indexes(samples, right))/samples.size();
    }
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
    float maxim = std::numeric_limits<int>::min();
    int dimSize = dimensions.size();
    int samplesSize = samples.size();
    int splitIndex = -1, splitValue = -1;
    vector<int> frequency(256, 0);
    for (int i = 0; i < dimSize; ++i){
        for (int j = 0; j < samplesSize; ++j){
            frequency[samples[j][dimensions[i]]]++;
        }
        for (int j = 0; j < 256; ++j){
            if (frequency[j] > 0){
                aux = calculateInformationGain(samples, dimensions[i], j);
            }
            if (aux > maxim){
                std::cout << aux << " " << std::endl;
                maxim = aux;
                splitValue = j;
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
    } else {
        vector<int> dimensions = random_dimensions(samples[0].size());
        pair<int, int> find_split = find_best_split(samples, dimensions);
        int index = find_split.first;
        int val = find_split.second;
        if (val == -1 && index == -1) {
            make_leaf(samples, is_single_class);
        } else {
            std::shared_ptr<Node> left(new Node);
            std::shared_ptr<Node> right(new Node);
            pair<vector<std::vector<int>>, vector<std::vector<int>>> split_samples =
            split(samples, index, val);
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
    int indexSize = index.size();
    vector<int> no_tests(10, 0);
    for (int i : index) {
        ++no_tests[samples[i][0]];
    }
    for (int j : no_tests) {
        if (j != 0) {
            int p = j / indexSize;
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
    int freq[256];
    for(int i = 0; i < 256; i++)
        freq[i] = 0;
    int samplesSize = samples.size();
    for(int i = 0; i < samplesSize; i++)
        freq[samples[i][col]]++;
    for(int i = 0; i < 256; i++)
        if(freq[i] > 0)
            uniqueValues.push_back(i);
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

    return pair<vector<vector<int>>, vector<vector<int>>>(left, right);
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
    randValue = rand_r(&randValue);
    randValue *= randValue;
    randValue +=1;
    int dimension = floor(sqrt(size));
    vector<int> rez;
    for (int i = 0; i < dimension; ++i) {
        if (i == 0) {
            int tmp = rand_r(&randValue) % size;
            while (tmp == 0) {
                tmp = rand_r(&randValue) % size;
            }
            rez.push_back(tmp);
        } else {
            int j = 0;
            int tmp = rand_r(&randValue) % size;
            while ((tmp == 0 || rez[j] == tmp) && j < i) {
                tmp = rand_r(&randValue) % size;
                ++j;
            }
            rez.push_back(tmp);
        }
    }
    return rez;
}
