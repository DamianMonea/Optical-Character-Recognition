// copyright Luca Istrate, Andrei Medar
#include "randomForest.h"
#include <iostream>
#include <random>
#include <vector>
#include <string>
#include "decisionTree.h"

using std::vector;
using std::pair;
using std::string;
using std::mt19937;
using std::cout;
using std::endl;

vector<vector<int>> get_random_samples(const vector<vector<int>> &samples,
                                       int num_to_return) {
    // TODO(you)
    // Intoarce un vector de marime num_to_return cu elemente random,
    // diferite din samples
    vector<vector<int>> ret;
    int samplesSize = samples.size();
    unsigned int randValue = (unsigned int) num_to_return;
    bool *pushed = new bool[samplesSize];
    for (int i = 0; i < samplesSize; ++i)
        pushed[i] = false;
    unsigned int x = 0;
    for (int i = 0; i < num_to_return; ++i){
        x = random() % samplesSize;
        while (pushed[x] == true){
            x = random() % samplesSize;
        }
        pushed[x] = true;
        ret.push_back(samples.at(x));
    }
    ret.resize(num_to_return);
    return ret;
}

RandomForest::RandomForest(int num_trees, const vector<vector<int>> &samples)
    : num_trees(num_trees), images(samples) {}

void RandomForest::build() {
    // Aloca pentru fiecare Tree cate n / num_trees
    // Unde n e numarul total de teste de training
    // Apoi antreneaza fiecare tree cu testele alese
    assert(!images.empty());
    vector<vector<int>> random_samples;

    int data_size = images.size() / num_trees;

    for (int i = 0; i < num_trees; i++) {
        // cout << "Creating Tree nr: " << i << endl;
        random_samples = get_random_samples(images, data_size);

        // Construieste un Tree nou si il antreneaza
        trees.push_back(Node());
        trees[trees.size() - 1].train(random_samples);
    }
}

int RandomForest::predict(const vector<int> &image) {
    // TODO(you)
    // Va intoarce cea mai probabila prezicere pentru testul din argument
    // se va interoga fiecare Tree si se va considera raspunsul final ca
    // fiind cel majoritar
    int count[10];
    for (int i = 0; i < 10; ++i)
        count[i] = 0;
    for (int i = 0; i < num_trees; ++i){
        count[trees[i].predict(image)]++;
    }
    int maxim = -10;
    int res = 0;
    for (int i = 0; i < 10; ++i) {
        if (count[i] > 0 && count[i] > maxim){
            maxim = count[i];
            res = i;
        }
    }
    return res;
}
