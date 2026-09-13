#ifndef VECTORSTORE_H
#define VECTORSTORE_H

#pragma once
// NOTE: Per assignment rules, only this single include is allowed here.
#include "main.h"

// ------------------------------
// AVL balance enum
// ------------------------------
enum BalanceValue
{
    LH = -1, // Left Higher
    EH = 0,  // Equal Height
    RH = 1   // Right Higher
};

// ------------------------------
// Generic AVL Tree (template)
// ------------------------------
template <class K, class T>
class AVLTree
{
    friend class VectorStore; // Allow VectorStore to access protected/private members

public:
    class AVLNode
    {
    public:
        K key;
        T data;
        AVLNode *pLeft;
        AVLNode *pRight;
        BalanceValue balance;

        AVLNode(const K &key, const T &value)
            : key(key), data(value), pLeft(nullptr), pRight(nullptr), balance(EH) {}

        friend class VectorStore; // Allow VectorStore to access AVLNode members
    };

protected:
    AVLNode *root;

    AVLNode *rotateRight(AVLNode *&node);
    AVLNode *rotateLeft(AVLNode *&node);
    void clearHelper(AVLNode *node);
    int getNodeHeight(AVLNode* node) const;
    int getBalance(AVLNode* node) const;
    int countHelper(AVLNode* node) const;
    AVLNode* insertRec(AVLNode* node, const K& key, const T& value);
    AVLNode* removeRec(AVLNode* node, const K& key);
    void inorderHelper(AVLNode* node, void (*action)(const T&)) const;


public:
    AVLTree();
    ~AVLTree();

    void insert(const K &key, const T &value);
    void remove(const K &key);
    bool contains(const K &key) const;

    int getHeight() const;
    int getSize() const;
    bool empty() const;
    void clear();

    void printTreeStructure() const;

    void inorderTraversal(void (*action)(const T &)) const;
    AVLNode*successor(AVLNode* node) const;

    AVLNode *getRoot() const { return root; }
};


enum Color
{
    RED,
    BLACK
};

// RedBlackTree class
template <class K, class T>
class RedBlackTree
{
    friend class VectorStore; // Allow VectorStore to access protected/private members

public:
    // RBTNode class

    class RBTNode
    {
    public:
        K key;
        T data;
        Color color;
        RBTNode *parent;
        RBTNode *left;
        RBTNode *right;

        // Constructor
        RBTNode(const K &key, const T &value);

        void recolorToRed();
        void recolorToBlack();

        friend class VectorStore; // Allow VectorStore to access RBTNode members
    };

private:
    RBTNode *root;

protected:
    void rotateLeft(RBTNode *node);
    void rotateRight(RBTNode *node);

    RBTNode *lowerBoundNode(const K &key) const;
    RBTNode *upperBoundNode(const K &key) const;

    // Helper function
    void clearHelper(RBTNode *node);
    int countNodes(RBTNode *node) const;
    void fixInsertViolation(RBTNode *node);
    RBTNode *findMax(RBTNode *node) const;
    void transplant(RBTNode *u, RBTNode *v);
    void fixDeleteViolation(RBTNode *x, RBTNode *xParent);
    void rangeQueryValuesHelper(RBTNode* node,
                                const K& low,
                                const K& high,
                                std::vector<T*>& result) const;
public:
    RedBlackTree();
    ~RedBlackTree();

    bool empty() const;
    int size() const;
    void clear();
    void insert(const K &key, const T &value);
    void remove(const K &key);
    RBTNode *find(const K &key) const;
    bool contains(const K &key) const;

    RBTNode *lowerBound(const K &key, bool &found) const;
    RBTNode *upperBound(const K &key, bool &found) const;

    void printTreeStructure() const;

};


// ------------------------------
// VectorRecord
// ------------------------------
class VectorRecord
{
public:
    int id;
    std::string rawText;
    int rawLength;
    std::vector<float> *vector;
    double distanceFromReference;
    double norm;

    VectorRecord()
        : id(-1), rawLength(0), vector(nullptr), distanceFromReference(0.0), norm(0.0) {}

    VectorRecord(int _id,
                 const std::string &_rawText,
                 std::vector<float> *_vec,
                 double _dist,
                 double _norm = 0)
        : id(_id),
          rawText(_rawText),
          rawLength(static_cast<int>(_rawText.size())),
          vector(_vec),
          distanceFromReference(_dist),
          norm(_norm) {}

    // Overload operator << to print only the id
    friend std::ostream &operator<<(std::ostream &os, const VectorRecord &record);
};

// ------------------------------
// VectorStore
// ------------------------------
class VectorStore
{
private:
    AVLTree<double, VectorRecord> *vectorStore;
    RedBlackTree<double, VectorRecord> *normIndex;

    std::vector<float> *referenceVector;
    VectorRecord *rootVector;

    int dimension;
    int count;
    double averageDistance;
    std::vector<VectorRecord*> allRecords;  // Lưu TẤT CẢ records

    std::vector<float> *(*embeddingFunction)(const std::string &);

    double distanceByMetric(const std::vector<float> &a,
                            const std::vector<float> &b,
                            const std::string &metric) const;

    static std::vector<VectorRecord*> tempDeleteList;
    static void collectRecordsCallback(const VectorRecord& rec);
    void collectRecordsInOrder(AVLTree<double, VectorRecord>::AVLNode* node,
                                        std::vector<VectorRecord*>& tempList);
    void getVectorHelper(AVLTree<double, VectorRecord>::AVLNode* node, int index, int& currentIndex, VectorRecord*& result);

    void findVectorNearestToDistanceHelper(AVLTree<double, VectorRecord>::AVLNode* node, double targetDistance, double& minDiff, VectorRecord*& result);
    VectorRecord *findVectorNearestToDistance(double targetDistance) const;
    void rebuildRootIfNeeded();
    void rebuildTreeWithNewRoot(VectorRecord *newRoot);
    void collectAllVectorsHelper(typename AVLTree<double, VectorRecord>::AVLNode *node, vector<VectorRecord *> &result);
    void forEachHelper(AVLTree<double, VectorRecord>::AVLNode* node, void (*action)(vector<float>&, int, string&));
    void collectIdsInOrder(AVLTree<double, VectorRecord>::AVLNode* node, std::vector<int>& ids) const;
    void collectVectorsInOrder(AVLTree<double, VectorRecord>::AVLNode* node, std::vector<VectorRecord*>& outList) const;
    int partitionPairs(vector<pair<double, int>> &arr, int low, int high);
    void quickSortPairs(vector<pair<double, int>> &arr, int low, int high);

    void rangeQueryAVLHelper(
    AVLTree<double, VectorRecord>::AVLNode* node,
    double minDist,
    double maxDist,
    std::vector<int>& results) const;

    void rangeQueryHelper(
    AVLTree<double, VectorRecord>::AVLNode* node,
    const vector<float>& query,
    double radius,
    const string& metric,
    vector<int>& result);

    bool isValidMetric(const std::string &metric) const;

public:

    VectorStore(int dimension,
                std::vector<float> *(*embeddingFunction)(const std::string &),
                const std::vector<float> &referenceVector);
    ~VectorStore();

    int size();
    bool empty();
    void clear();

    std::vector<float> *preprocessing(std::string rawText);
    void addText(std::string rawText);

    VectorRecord *getVector(int index);
    std::string getRawText(int index);
    int getId(int index);

    bool removeAt(int index);

    void setReferenceVector(const std::vector<float> &newReference);
    std::vector<float> *getReferenceVector() const;
    VectorRecord *getRootVector() const;
    double getAverageDistance() const;
    void setEmbeddingFunction(std::vector<float> *(*newEmbeddingFunction)(const std::string &));

    void forEach(void (*action)(std::vector<float> &, int, std::string &));
    vector<int> getAllIdsSortedByDistance() const;
    vector<VectorRecord *> getAllVectorsSortedByDistance() const;

    double l1Distance(const vector<float> &v1, const vector<float> &v2) const;
    double l2Distance(const vector<float> &v1, const vector<float> &v2) const;
    double cosineSimilarity(const vector<float> &v1, const vector<float> &v2) const;



    double estimateD_Linear(const std::vector<float> &query, int k, double averageDistance, const std::vector<float> &reference, double c0_bias = 1e-9, double c1_slope = 0.05);

    int findNearest(const std::vector<float> &query, std::string metric = "cosine");
    int *topKNearest(const std::vector<float> &query, int k, std::string metric = "cosine");

    int *rangeQueryFromRoot(double minDist, double maxDist) const;
    int *rangeQuery(const std::vector<float> &query, double radius, std::string metric = "cosine") const;
    int *boundingBoxQuery(const std::vector<float> &minBound, const std::vector<float> &maxBound) const;

    double getMaxDistance() const;
    double getMinDistance() const;
    VectorRecord computeCentroid(const std::vector<VectorRecord *> &records) const;
};

#endif // VECTORSTORE_H
