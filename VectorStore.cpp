// NOTE: Per assignment rules, only this single include is allowed here.
#include "VectorStore.h"

// =====================================
// Helper functions
// =====================================

// Helper function to print n spaces for tree visualization
void printNSpace(int n)
{
    for (int i = 0; i < n; i++)
    {
        cout << " ";
    }
}

// =====================================
// AVLTree<K, T> implementation
// =====================================

template <class K, class T>
void AVLTree<K, T>::printTreeStructure() const
{
    int height = this->getHeight();
    if (this->root == NULL)
    {
        cout << "NULL\n";
        return;
    }
    queue<AVLNode *> q;
    q.push(root);
    AVLNode *temp;
    int count = 0;
    int maxNode = 1;
    int level = 0;
    int space = pow(2, height);
    printNSpace(space / 2);
    while (!q.empty())
    {
        temp = q.front();
        q.pop();
        if (temp == NULL)
        {
            cout << " ";
            q.push(NULL);
            q.push(NULL);
        }
        else
        {
            cout << temp->data;
            q.push(temp->pLeft);
            q.push(temp->pRight);
        }
        printNSpace(space);
        count++;
        if (count == maxNode)
        {
            cout << endl;
            count = 0;
            maxNode *= 2;
            level++;
            space /= 2;
            printNSpace(space / 2);
        }
        if (level == height)
            return;
    }
}

// TODO: Implement all AVLTree<K, T> methods here

template <class K, class T>
AVLTree<K, T>::AVLTree():root(nullptr){}

template <class K, class T>
AVLTree<K, T>::~AVLTree(){
    clear();
}

template <class K, class T>
typename AVLTree<K, T>::AVLNode* AVLTree<K, T>::rotateRight(AVLNode*& node){
    AVLNode* x = node->pLeft;
    AVLNode* T2 = x->pRight;

    x->pRight = node;
    node->pLeft = T2;

    return x;
}

template <class K, class T>
typename AVLTree<K, T>::AVLNode* AVLTree<K, T>::rotateLeft(AVLNode*& node){
    AVLNode* y = node->pRight;
    AVLNode* T2 = y->pLeft;

    y->pLeft = node;
    node->pRight = T2;

    return y;
}

template <class K, class T>
void AVLTree<K, T>::clearHelper(AVLNode* node){
    if (!node) return;

    clearHelper(node->pLeft);
    clearHelper(node->pRight);
    delete node;
}

template <class K, class T>
void AVLTree<K, T>::clear(){
	clearHelper(root);
	root = nullptr;
}
//int getHeight() const; int getSize() const; bool empty() const;
template <class K, class T>
int AVLTree<K, T>::getNodeHeight(AVLNode* node) const{
	if (!node) return 0;
	return 1 + max(getNodeHeight(node->pLeft), getNodeHeight(node->pRight));
}

template <class K, class T>
int AVLTree<K, T>::getHeight() const{
	return getNodeHeight(root);
}

template <class K, class T>
int AVLTree<K, T>::getBalance(AVLNode* node) const{
	if (!node) return 0;
	return getNodeHeight(node->pLeft) - getNodeHeight(node->pRight);
}

template <class K, class T>
int AVLTree<K, T>::countHelper(AVLNode* node) const{
	if (!node) return 0;
	return 1 + countHelper(node->pLeft) + countHelper(node->pRight);

}

template <class K, class T>
int AVLTree<K, T>::getSize() const{
	return countHelper(root);
}

template <class K, class T>
typename AVLTree<K, T>::AVLNode* AVLTree<K, T>::insertRec(AVLNode* node, const K& key, const T& value){
	if (!node) return new AVLNode(key, value);

	if (key < node->key)
		node->pLeft = insertRec(node->pLeft, key, value);
	else if (key > node->key)
		node->pRight = insertRec(node->pRight, key, value);
	else 
		return node;

	int balance = getBalance(node);

	if (balance > 1 && key < node->pLeft->key)
		return rotateRight(node);

	if (balance < -1 && key > node->pRight->key)
		return rotateLeft(node);
	
	if (balance > 1 && key > node->pLeft->key){
		node->pLeft = rotateLeft(node->pLeft);
		return rotateRight(node);
	}
	
	if (balance < -1 && key < node->pRight->key){
		node->pRight = rotateRight(node->pRight);
		return rotateLeft(node);
	}

	return node;
}

template <class K, class T>
void AVLTree<K, T>::insert(const K& key, const T& value){
	root = insertRec(root, key, value);
}

template <class K, class T>
typename AVLTree<K, T>::AVLNode* AVLTree<K, T>::removeRec(AVLNode* node, const K& key){
	if (!node) return nullptr;

	if (key < node->key)
		node->pLeft = removeRec(node->pLeft, key);
	else if (key > node->key)
		node->pRight = removeRec(node->pRight, key);
	else {
    // Node có 0 hoặc 1 child
    if (!node->pLeft || !node->pRight) {
        AVLNode* child = node->pLeft ? node->pLeft : node->pRight;
        delete node;
        return child;
    }

    // Node có 2 child → tìm successor
    AVLNode* temp = node->pRight;
    while (temp->pLeft)
        temp = temp->pLeft;

    node->key = temp->key;
    node->data = temp->data;

    node->pRight = removeRec(node->pRight, temp->key);
}

	if (!node) return node;

	int balance = getBalance(node);
	// 3. Xử lý các trường hợp mất cân bằng

    // Left-Left (LL)
    if (balance > 1 && getBalance(node->pLeft) >= 0)
        return rotateRight(node);

    // Left-Right (LR)
    if (balance > 1 && getBalance(node->pLeft) < 0) {
        node->pLeft = rotateLeft(node->pLeft);
        return rotateRight(node);
    }

    // Right-Right (RR)
    if (balance < -1 && getBalance(node->pRight) <= 0)
        return rotateLeft(node);

    // Right-Left (RL)
    if (balance < -1 && getBalance(node->pRight) > 0) {
        node->pRight = rotateRight(node->pRight);
        return rotateLeft(node);
    }

	return node;
}

template <class K, class T>
void AVLTree<K, T>::remove(const K& key) {
    root = removeRec(root, key);
}

template <class K, class T>
bool AVLTree<K, T>::contains(const K& key) const {
    AVLNode* curr = root;
    while (curr) {
        if (key == curr->key) return true;
        curr = (key < curr->key) ? curr->pLeft : curr->pRight;
    }
    return false;
}

template <class K, class T>
bool AVLTree<K, T>::empty() const {
    return root == nullptr;
}

template <class K, class T>
void AVLTree<K, T>::inorderHelper(AVLNode* node, void (*action)(const T&)) const {
    if (!node) return;
    inorderHelper(node->pLeft, action);
    action(node->data);
    inorderHelper(node->pRight, action);
}

template <class K, class T>
void AVLTree<K, T>::inorderTraversal(void (*action)(const T&)) const {
    inorderHelper(root, action);
}

// =====================================
// RedBlackTree<K, T> implementation
// =====================================

template <class K, class T>
void RedBlackTree<K, T>::printTreeStructure() const
{
    if (this->root == nullptr)
    {
        cout << "NULL\n";
        return;
    }

    queue<RBTNode *> q;
    q.push(root);
    RBTNode *temp;
    int count = 0;
    int maxNode = 1;
    int level = 0;

    // Calculate tree height for spacing
    int height = 0;
    queue<RBTNode *> heightQ;
    heightQ.push(root);
    while (!heightQ.empty())
    {
        int size = heightQ.size();
        height++;
        for (int i = 0; i < size; i++)
        {
            RBTNode *node = heightQ.front();
            heightQ.pop();
            if (node->left)
                heightQ.push(node->left);
            if (node->right)
                heightQ.push(node->right);
        }
    }

    int space = pow(2, height);
    printNSpace(space / 2);

    while (!q.empty())
    {
        temp = q.front();
        q.pop();

        if (temp == nullptr)
        {
            cout << "NULL";
            q.push(nullptr);
            q.push(nullptr);
        }
        else
        {
            // Print node data and color as tuple: (data, color)
            cout << "(" << temp->data << ",";
            if (temp->color == RED)
            {
                cout << "R)";
            }
            else
            {
                cout << "B)";
            }

            q.push(temp->left);
            q.push(temp->right);
        }

        printNSpace(space);
        count++;

        if (count == maxNode)
        {
            cout << endl;
            count = 0;
            maxNode *= 2;
            level++;
            space /= 2;
            printNSpace(space / 2);
        }

        if (level == height)
        {
            return;
        }
    }
}

// TODO: Implement all other RedBlackTree<K, T> methods here

template <class K, class T>
RedBlackTree<K, T>::RBTNode::RBTNode(const K &key, const T &value)
    : key(key), data(value), color(RED), parent(nullptr), left(nullptr), right(nullptr){}

template <class K, class T>
void RedBlackTree<K, T>::RBTNode::recolorToRed(){
    this->color = RED;
}

template <class K, class T>
void RedBlackTree<K, T>::RBTNode::recolorToBlack(){
    this->color = BLACK;
}

template <class K, class T>
RedBlackTree<K, T>::RedBlackTree():root(nullptr){}

template <class K, class T>
RedBlackTree<K, T>::~RedBlackTree(){
    clear();
}

template <class K, class T>
void RedBlackTree<K, T>::clearHelper(RBTNode *node){
    if (!node) return;
    clearHelper(node->left);
    clearHelper(node->right);
    delete node;
}

template <class K, class T>
void RedBlackTree<K, T>::clear(){
    clearHelper(root);
    root = nullptr;
}

template <class K, class T>
int RedBlackTree<K, T>::countNodes(RBTNode *node) const{
    if (!node) return 0;
    return 1 + countNodes(node->left) + countNodes(node->right);
}

template <class K, class T>
int RedBlackTree<K, T>::size() const{
    return countNodes(root);
}

// fix Red-Black tree violations after insertion
template <class K, class T>
void RedBlackTree<K, T>::fixInsertViolation(RBTNode *node)
{
    while (node != root && node->parent != nullptr && node->parent->color == RED)
    {
        RBTNode *parent = node->parent;
        RBTNode *grandparent = parent->parent;

        if (grandparent == nullptr)
            break;

        if (parent == grandparent->left)
        {
            RBTNode *uncle = grandparent->right;

            // Case 2: Uncle is RED - recolor
            if (uncle != nullptr && uncle->color == RED)
            {
                parent->recolorToBlack();
                uncle->recolorToBlack();
                grandparent->recolorToRed();
                node = grandparent;
            }
            else
            {
                // Case 3: Triangle (left-right) - rotate to make it straight
                if (node == parent->right)
                {
                    rotateLeft(parent);
                    node = parent;
                    parent = node->parent;
                }

                // Case 4: Straight line (left-left) - rotate at grandparent
                rotateRight(grandparent);
                parent->recolorToBlack();
                grandparent->recolorToRed();
            }
        }
        else
        {
            RBTNode *uncle = grandparent->left;

            // Case 2: Uncle is RED - recolor
            if (uncle != nullptr && uncle->color == RED)
            {
                parent->recolorToBlack();
                uncle->recolorToBlack();
                grandparent->recolorToRed();
                node = grandparent;
            }
            else
            {
                // Case 3: Triangle (right-left) - rotate to make it straight
                if (node == parent->left)
                {
                    rotateRight(parent);
                    node = parent;
                    parent = node->parent;
                }

                // Case 4: Straight line (right-right) - rotate at grandparent
                rotateLeft(grandparent);
                parent->recolorToBlack();
                grandparent->recolorToRed();
            }
        }
    }
    // Case 1: Root must be black
    root->recolorToBlack();
}

template <class K, class T>
bool RedBlackTree<K, T>::empty() const{
    return root == nullptr;
}

template <class K, class T>
void RedBlackTree<K, T>::insert(const K &key, const T &value)
{
    // Check if key already exists - if yes, do nothing
    if (contains(key))
        return;

    RBTNode *newNode = new RBTNode(key, value);

    // Standard BST insertion
    if (root == nullptr)
    {
        root = newNode;
        root->recolorToBlack(); // Case 1: Root must be black
        return;
    }

    RBTNode *current = root;
    RBTNode *parent = nullptr;

    while (current != nullptr)
    {
        parent = current;
        if (key < current->key)
            current = current->left;
        else
            current = current->right;
    }

    newNode->parent = parent;
    if (key < parent->key)
        parent->left = newNode;
    else
        parent->right = newNode;

    // Fix Red-Black tree violations
    fixInsertViolation(newNode);
}

// Protected helper: find maximum node in subtree (predecessor)
template <class K, class T>
typename RedBlackTree<K, T>::RBTNode *RedBlackTree<K, T>::findMax(RBTNode *node) const
{
    while (node->right != nullptr)
        node = node->right;
    return node;
}

// Protected helper: replace node u with node v
template <class K, class T>
void RedBlackTree<K, T>::transplant(RBTNode *u, RBTNode *v)
{
    if (u->parent == nullptr)
        root = v;
    else if (u == u->parent->left)
        u->parent->left = v;
    else
        u->parent->right = v;

    if (v != nullptr)
        v->parent = u->parent;
}

// Protected helper: fix Red-Black tree violations after deletion
template <class K, class T>
void RedBlackTree<K, T>::fixDeleteViolation(RBTNode *x, RBTNode *xParent)
{
    while (x != root && (x == nullptr || x->color == BLACK))
    {
        if (x == xParent->left)
        {
            RBTNode *sibling = xParent->right;

            // Case 2: Sibling is RED - rotate and recolor
            if (sibling != nullptr && sibling->color == RED)
            {
                sibling->recolorToBlack();
                xParent->recolorToRed();
                rotateLeft(xParent);
                sibling = xParent->right;
            }

            // Case 3: Both children of sibling are BLACK - recolor and move up
            if ((sibling->left == nullptr || sibling->left->color == BLACK) &&
                (sibling->right == nullptr || sibling->right->color == BLACK))
            {
                sibling->recolorToRed();
                x = xParent;
                xParent = x->parent;
                // If parent was RED, recolor to BLACK and done
                if (x->color == RED)
                {
                    x->recolorToBlack();
                    break;
                }
            }
            else
            {
                // Case 4: Far child is BLACK, near child is RED - rotate at sibling
                if (sibling->right == nullptr || sibling->right->color == BLACK)
                {
                    if (sibling->left != nullptr)
                        sibling->left->recolorToBlack();
                    sibling->recolorToRed();
                    rotateRight(sibling);
                    sibling = xParent->right;
                }

                // Case 5: Far child is RED - rotate at parent and recolor
                sibling->color = xParent->color;
                xParent->recolorToBlack();
                if (sibling->right != nullptr)
                    sibling->right->recolorToBlack();
                rotateLeft(xParent);
                x = root; // Done
            }
        }
        else
        {
            RBTNode *sibling = xParent->left;

            // Case 2: Sibling is RED - rotate and recolor
            if (sibling != nullptr && sibling->color == RED)
            {
                sibling->recolorToBlack();
                xParent->recolorToRed();
                rotateRight(xParent);
                sibling = xParent->left;
            }

            // Case 3: Both children of sibling are BLACK - recolor and move up
            if ((sibling->right == nullptr || sibling->right->color == BLACK) &&
                (sibling->left == nullptr || sibling->left->color == BLACK))
            {
                sibling->recolorToRed();
                x = xParent;
                xParent = x->parent;
                // If parent was RED, recolor to BLACK and done
                if (x->color == RED)
                {
                    x->recolorToBlack();
                    break;
                }
            }
            else
            {
                // Case 4: Far child is BLACK, near child is RED - rotate at sibling
                if (sibling->left == nullptr || sibling->left->color == BLACK)
                {
                    if (sibling->right != nullptr)
                        sibling->right->recolorToBlack();
                    sibling->recolorToRed();
                    rotateLeft(sibling);
                    sibling = xParent->left;
                }

                // Case 5: Far child is RED - rotate at parent and recolor
                sibling->color = xParent->color;
                xParent->recolorToBlack();
                if (sibling->left != nullptr)
                    sibling->left->recolorToBlack();
                rotateRight(xParent);
                x = root; // Done
            }
        }
    }
    
    // Case 1: Root must be black
    if (x != nullptr)
        x->recolorToBlack();
}

template <class K, class T>
void RedBlackTree<K, T>::remove(const K &key)
{
    RBTNode *z = find(key);
    if (z == nullptr)
        return;

    // Step 1: If z has two children, swap with predecessor
    if (z->left != nullptr && z->right != nullptr)
    {
        RBTNode *predecessor = findMax(z->left);
        // Swap key and data
        K tempKey = z->key;
        T tempData = z->data;
        z->key = predecessor->key;
        z->data = predecessor->data;
        predecessor->key = tempKey;
        predecessor->data = tempData;
        z = predecessor;
    }

    // Step 2: Now z has at most one child
    RBTNode *x = (z->left != nullptr) ? z->left : z->right;
    RBTNode *xParent = z->parent;
    Color zColor = z->color;

    // Remove z and replace with x
    transplant(z, x);
    
    // Update xParent if x is nullptr
    if (x == nullptr && xParent != nullptr)
    {
        if (xParent->left == x)
            xParent->left = nullptr;
        else
            xParent->right = nullptr;
    }

    delete z;

    // If removed node was BLACK, fix violations
    if (zColor == BLACK)
    {
        if (x == nullptr && xParent == nullptr)
        {
            // Tree is now empty
            root = nullptr;
        }
        else if (x != nullptr && xParent == nullptr)
        {
            // x is new root
            root = x;
            x->recolorToBlack();
        }
        else
        {
            fixDeleteViolation(x, xParent);
        }
    }
}

template <class K, class T>
typename RedBlackTree<K, T>::RBTNode *RedBlackTree<K, T>::find(const K &key) const
{
    RBTNode *current = root;
    while (current != nullptr)
    {
        if (key < current->key)
            current = current->left;
        else if (key > current->key)
            current = current->right;
        else
            return current;
    }
    return nullptr;
}

template <class K, class T>
bool RedBlackTree<K, T>::contains(const K &key) const
{
    return find(key) != nullptr;
}

template <class K, class T>
typename RedBlackTree<K, T>::RBTNode *RedBlackTree<K, T>::lowerBoundNode(const K &key) const
{
    RBTNode *current = root;
    RBTNode *result = nullptr;

    while (current != nullptr)
    {
        if (current->key >= key)
        {
            result = current;
            current = current->left;
        }
        else
        {
            current = current->right;
        }
    }
    return result;
}

template <class K, class T>
typename RedBlackTree<K, T>::RBTNode *RedBlackTree<K, T>::upperBoundNode(const K &key) const
{
    RBTNode *current = root;
    RBTNode *result = nullptr;

    while (current != nullptr)
    {
        if (current->key > key)
        {
            result = current;
            current = current->left;
        }
        else
        {
            current = current->right;
        }
    }
    return result;
}

template <class K, class T>
typename RedBlackTree<K, T>::RBTNode *RedBlackTree<K, T>::lowerBound(const K &key, bool &found) const
{
    RBTNode *result = lowerBoundNode(key);
    found = (result != nullptr);
    return result;
}

template <class K, class T>
typename RedBlackTree<K, T>::RBTNode *RedBlackTree<K, T>::upperBound(const K &key, bool &found) const
{
    RBTNode *result = upperBoundNode(key);
    found = (result != nullptr);
    return result;
}

template <class K, class T>
void RedBlackTree<K, T>::rotateLeft(RBTNode *node)
{
    if (node == nullptr || node->right == nullptr)
        return;

    RBTNode *temp = node->right;
    node->right = temp->left;

    if (temp->left != nullptr)
        temp->left->parent = node;

    temp->parent = node->parent;

    if (node->parent == nullptr)
        root = temp;
    else if (node == node->parent->left)
        node->parent->left = temp;
    else
        node->parent->right = temp;

    temp->left = node;
    node->parent = temp;
}

template <class K, class T>
void RedBlackTree<K, T>::rotateRight(RBTNode *node)
{
    if (node == nullptr || node->left == nullptr)
        return;

    RBTNode *temp = node->left;
    node->left = temp->right;

    if (temp->right != nullptr)
        temp->right->parent = node;

    temp->parent = node->parent;

    if (node->parent == nullptr)
        root = temp;
    else if (node == node->parent->right)
        node->parent->right = temp;
    else
        node->parent->left = temp;

    temp->right = node;
    node->parent = temp;
}


// =====================================
// VectorRecord implementation
// =====================================

const double EPS = 1e-9;

// ------------------------------
// VectorRecord Implementation
// ------------------------------
ostream &operator<<(ostream &os, const VectorRecord &record)
{
    os << record.id;
    return os;
}

// ------------------------------
// VectorStore Implementation
// ------------------------------

// Constructor
VectorStore::VectorStore(int dimension,
                         vector<float> *(*embeddingFunction)(const string &),
                         const vector<float>& referenceVector)
    : dimension(dimension),
      embeddingFunction(embeddingFunction),
      count(0),
      averageDistance(0.0),
      rootVector(nullptr)
{
    this->referenceVector = new vector<float>(referenceVector);
    this->vectorStore = new AVLTree<double, VectorRecord>();
    this->normIndex = new RedBlackTree<double, VectorRecord>();
}

std::vector<VectorRecord*> VectorStore::tempDeleteList;

void VectorStore::collectRecordsCallback(const VectorRecord& rec){
    tempDeleteList.push_back(const_cast<VectorRecord*>(&rec));
}

// Destructor
VectorStore::~VectorStore() {
    // Xóa từng record trong allRecords
    for (VectorRecord* rec : allRecords) {
        delete rec->vector;
        delete rec;
    }

    delete vectorStore;
    delete normIndex;
    delete referenceVector;
}


int VectorStore::size(){
    return count;
}

bool VectorStore::empty(){
    return count == 0;
}

void VectorStore::clear(){
    // Delete từng VectorRecord
    for (VectorRecord* rec : allRecords){
        delete rec->vector;
        delete rec;
    }
    
    allRecords.clear();
    vectorStore->clear();
    normIndex->clear();

    rootVector = nullptr;
    count = 0;
    averageDistance = 0.0;
}

vector<float>* VectorStore::preprocessing(string rawText){
    vector<float>* embedded = embeddingFunction(rawText);
    

    // Nếu embeddingFunction trả về nullptr → lỗi
    if (!embedded)
        return new vector<float>(dimension, 0.0f);

    int currentSize = embedded->size();

    if (currentSize > dimension){
        embedded->resize(dimension);
    } else if (currentSize < dimension){
        embedded->resize(dimension, 0.0f);
    }

    return embedded;
}

void VectorStore::collectRecordsInOrder(AVLTree<double, VectorRecord>::AVLNode* node,
                                        std::vector<VectorRecord*>& tempList) 
{
    if (!node) return;
    collectRecordsInOrder(node->pLeft, tempList);
    
    // CHỈ lưu con trỏ tới data trong node, KHÔNG tạo bản sao
    tempList.push_back(&node->data);
    
    collectRecordsInOrder(node->pRight, tempList);
}

void VectorStore::addText(string rawText)
{
    // 1. Tiền xử lý: rawText -> vector<float>*
    vector<float> *vec = preprocessing(rawText);
    
    // 2. Tính khoảng cách từ referenceVector
    double dist = 0.0;
    for (int i = 0; i < dimension; i++)
    {
        double diff = (*referenceVector)[i] - (*vec)[i];
        dist += diff * diff;
    }
    dist = sqrt(dist);

    // ====================================================
    // FIX: Check for duplicate distance before adding
    // ====================================================
    if (vectorStore->contains(dist)) {
        delete vec; // Clean up memory allocated in preprocessing
        return;
    }
    // ====================================================

    // 3. Tính chuẩn Euclidean
    double normVal = 0.0;
    for (int i = 0; i < dimension; i++)
        normVal += (*vec)[i] * (*vec)[i];
    normVal = sqrt(normVal);
    

    // 4. Tạo VectorRecord mới (ID bắt đầu từ 1)
    int newId = 0;
    for (auto r : allRecords) {
        if (r->id > newId) {
            newId = r->id;
        }
    }
    newId++; // ID mới = max + 1
    
    // Nếu allRecords rỗng thì newId = 1
    if (allRecords.empty()) {
        newId = 1;
    }
    
    VectorRecord *record = new VectorRecord();
    record->id = newId;
    record->rawText = rawText;
    record->rawLength = rawText.length();
    record->vector = vec;
    record->distanceFromReference = dist;
    record->norm = normVal;
    
    // 5. Lưu vào danh sách allRecords
    allRecords.push_back(record);
    
    // 6. Cập nhật count và averageDistance
    averageDistance = (averageDistance * count + dist) / (count + 1);
    count++;
    
    // 7. Rebuild cây AVL
    if (vectorStore)
    {
        vectorStore->clear();
        delete vectorStore;
    }
    vectorStore = new AVLTree<double, VectorRecord>();
    
    // Insert tất cả records vào cây mới
    for (auto r : allRecords)
    {
        vectorStore->insert(r->distanceFromReference, *r);
    }

    std::vector<VectorRecord*> checkList;
    collectVectorsInOrder(vectorStore->root, checkList);
    
    // 8. Tìm rootVector từ danh sách allRecords (gần averageDistance nhất)
    rootVector = allRecords[0];
    double minDiff = abs(allRecords[0]->distanceFromReference - averageDistance);
    for (auto r : allRecords)
    {
        double diff = abs(r->distanceFromReference - averageDistance);
        if (diff < minDiff)
        {
            minDiff = diff;
            rootVector = r;
        }
    }
    
    // 9. Cập nhật normIndex
    normIndex->insert(normVal, *record);
}


// Helper function đệ quy
void VectorStore::getVectorHelper(AVLTree<double, VectorRecord>::AVLNode* node,
                                  int index,
                                  int& currentIndex,
                                  VectorRecord*& result)
{
    if (!node || result) return;

    // Duyệt trái
    getVectorHelper(node->pLeft, index, currentIndex, result);
    
    // THÊM CHECK NÀY!
    if (result) return;

    // Kiểm tra node hiện tại
    if (currentIndex == index) {
        result = &node->data;
        return;
    }
    currentIndex++;

    // Duyệt phải
    getVectorHelper(node->pRight, index, currentIndex, result);
}

VectorRecord* VectorStore::getVector(int index){
    if (index < 0 || index >= count)
        throw std::out_of_range("Index is invalid!");

    int currentIndex = 0;
    VectorRecord* result = nullptr;

    getVectorHelper(vectorStore->getRoot(), index, currentIndex, result);

    return result;
}

string VectorStore::getRawText(int index){
    VectorRecord* record = getVector(index); // dùng helper getVector
    if (!record)
        throw std::out_of_range("Index is invalid!");

    return record->rawText;
}

int VectorStore::getId(int index){
    VectorRecord* record = getVector(index); // dùng helper getVector
    if (!record)
        throw std::out_of_range("Index is invalid!");

    return record->id;
}


// Rebuild root if needed
void VectorStore::rebuildRootIfNeeded()
{
    if (allRecords.empty())
        return;
    
    // Rebuild cây AVL
    vectorStore->clear();
    delete vectorStore;
    vectorStore = new AVLTree<double, VectorRecord>();
    
    for (auto r : allRecords)
    {
        vectorStore->insert(r->distanceFromReference, *r);
    }
    
    // Tìm rootVector đúng theo thuật toán
    rootVector = allRecords[0];
    double minDiff = abs(allRecords[0]->distanceFromReference - averageDistance);
    for (auto r : allRecords)
    {
        double diff = abs(r->distanceFromReference - averageDistance);
        if (diff < minDiff)
        {
            minDiff = diff;
            rootVector = r;
        }
    }
}
// Rebuild tree with new root
void VectorStore::rebuildTreeWithNewRoot(VectorRecord *newRoot)
{
    if (!newRoot || allRecords.empty())
        return;
    
    rootVector = newRoot;
    
    // Rebuild cây AVL
    vectorStore->clear();
    delete vectorStore;
    vectorStore = new AVLTree<double, VectorRecord>();
    
    for (auto r : allRecords)
    {
        vectorStore->insert(r->distanceFromReference, *r);
    }
}

// Remove at
bool VectorStore::removeAt(int index)
{
    if (index < 0 || index >= count)
        return false;
    
    // 1. Lấy record cần xóa từ cây AVL (theo thứ tự in-order)
    VectorRecord *recordToRemove = getVector(index);
    if (!recordToRemove) return false;
    
    int idToRemove = recordToRemove->id;
    double distToRemove = recordToRemove->distanceFromReference;
    double normToRemove = recordToRemove->norm;
    
    
    // 2. Tìm và xóa record khỏi allRecords theo ID
    VectorRecord* actualRecord = nullptr;
    for (auto it = allRecords.begin(); it != allRecords.end(); ++it)
    {
        if ((*it)->id == idToRemove)
        {
            actualRecord = *it;
            allRecords.erase(it);
            break;
        }
    }
    
    if (!actualRecord) {
        return false;
    }
        
    // 3. Cập nhật averageDistance THEO CÔNG THỨC
    if (count == 1) {
        averageDistance = 0.0;
    } else {
        double oldAvg = averageDistance;
        averageDistance = (averageDistance * count - distToRemove) / (count - 1);
    }
    
    count--;
    
    
    // 4. Xóa khỏi normIndex
    normIndex->remove(normToRemove);
    
    // 5. Giải phóng memory của record đã xóa
    delete actualRecord->vector;
    delete actualRecord;
    
    // 6. Nếu không còn record nào
    if (count == 0)
    {
        vectorStore->clear();
        delete vectorStore;
        vectorStore = new AVLTree<double, VectorRecord>();
        rootVector = nullptr;
        return true;
    }
    
    // 7. Rebuild cây AVL
    vectorStore->clear();
    delete vectorStore;
    vectorStore = new AVLTree<double, VectorRecord>();
    
    for (auto r : allRecords)
    {
        vectorStore->insert(r->distanceFromReference, *r);
    }
    
    // 8. Tìm rootVector mới
    rootVector = allRecords[0];
    double minDiff = abs(allRecords[0]->distanceFromReference - averageDistance);
    
    for (auto r : allRecords)
    {
        double diff = abs(r->distanceFromReference - averageDistance);
        if (diff < minDiff)
        {
            minDiff = diff;
            rootVector = r;
        }
    }
    
    return true;
}

// Distance by metric
double VectorStore::distanceByMetric(const vector<float> &a,
                                     const vector<float> &b,
                                     const string &metric) const
{
    if (metric == "cosine")
    {
        double dot = 0.0, norm1 = 0.0, norm2 = 0.0;
        
        for (size_t i = 0; i < a.size(); i++)
        {
            dot += a[i] * b[i];
            norm1 += a[i] * a[i];
            norm2 += b[i] * b[i];
        }
        
        // THÊM CHECK: Nếu một trong hai vector có norm = 0 → trả về khoảng cách vô cực
        if (norm1 < EPS || norm2 < EPS)
            return std::numeric_limits<double>::max();  // Thay vì return 0.0
        
        double similarity = dot / (sqrt(norm1) * sqrt(norm2));
        return 1.0 - similarity;
    }
    else if (metric == "l1")
    {
        return l1Distance(a, b);
    }
    else
    {
        return l2Distance(a, b);
    }
}

// L2 distance
double VectorStore::l2Distance(const vector<float> &v1, const vector<float> &v2) const
{
    double dist = 0.0;
    for (size_t i = 0; i < v1.size(); i++)
    {
        double diff = v1[i] - v2[i];
        dist += diff * diff;
    }
    return sqrt(dist);
}

// L1 distance
double VectorStore::l1Distance(const vector<float> &v1, const vector<float> &v2) const
{
    double dist = 0.0;
    for (size_t i = 0; i < v1.size(); i++)
    {
        dist += abs(v1[i] - v2[i]);
    }
    return dist;
}

// Cosine similarity
double VectorStore::cosineSimilarity(const vector<float> &v1, const vector<float> &v2) const
{
    double dot = 0.0, norm1 = 0.0, norm2 = 0.0;
    
    for (size_t i = 0; i < v1.size(); i++)
    {
        dot += v1[i] * v2[i];
        norm1 += v1[i] * v1[i];
        norm2 += v2[i] * v2[i];
    }
    
    if (norm1 < EPS || norm2 < EPS)
        return 0.0;
    
    return dot / (sqrt(norm1) * sqrt(norm2));
}

void VectorStore::collectAllVectorsHelper(typename AVLTree<double, VectorRecord>::AVLNode *node,
                                          vector<VectorRecord *> &result)
{
    if (!node)
        return;
    
    collectAllVectorsHelper(node->pLeft, result);
    result.push_back(&node->data);
    collectAllVectorsHelper(node->pRight, result);
}

void VectorStore::setReferenceVector(const std::vector<float>& newReference) {
    if (!referenceVector) 
        referenceVector = new std::vector<float>(newReference);
    else 
        *referenceVector = newReference;

    if (count == 0) {
        rootVector = nullptr;
        averageDistance = 0.0;
        return;
    }

    // 1. Cập nhật distanceFromReference cho tất cả records trong allRecords
    double totalDist = 0.0;
    for (VectorRecord* rec : allRecords) {
        double dist = 0.0;
        for (int i = 0; i < dimension; i++) {
            double diff = (*referenceVector)[i] - (*(rec->vector))[i];
            dist += diff * diff;
        }
        rec->distanceFromReference = sqrt(dist);
        totalDist += rec->distanceFromReference;
    }

    // 2. Cập nhật averageDistance
    averageDistance = totalDist / count;

    // 3. Tìm rootVector mới (gần averageDistance nhất)
    rootVector = allRecords[0];
    double minDiff = std::abs(allRecords[0]->distanceFromReference - averageDistance);
    for (VectorRecord* rec : allRecords) {
        double diff = std::abs(rec->distanceFromReference - averageDistance);
        if (diff < minDiff) {
            minDiff = diff;
            rootVector = rec;
        }
    }

    // 4. Rebuild cây AVL với distanceFromReference mới
    vectorStore->clear();
    delete vectorStore;
    vectorStore = new AVLTree<double, VectorRecord>();
    
    for (VectorRecord* rec : allRecords) {
        vectorStore->insert(rec->distanceFromReference, *rec);
    }

    // 5. Rebuild normIndex (norm không đổi nên chỉ cần clear và insert lại)
    normIndex->clear();
    for (VectorRecord* rec : allRecords) {
        normIndex->insert(rec->norm, *rec);
    }
}

vector<float>* VectorStore::getReferenceVector() const {
    return referenceVector;
}

VectorRecord* VectorStore::getRootVector() const {
    return rootVector;
}

double VectorStore::getAverageDistance() const {
    return averageDistance;
}

void VectorStore::setEmbeddingFunction(vector<float>* (*newEmbeddingFunction)(const string&)) {
    embeddingFunction = newEmbeddingFunction;
}

// Helper function đệ quy
void VectorStore::forEachHelper(AVLTree<double, VectorRecord>::AVLNode* node,
                                void (*action)(vector<float>&, int, string&)) {
    if (!node) return;

    // Duyệt cây con bên trái
    forEachHelper(node->pLeft, action);

    // Thực thi action trên node hiện tại
    action(*node->data.vector, node->data.id, node->data.rawText);

    // Duyệt cây con bên phải
    forEachHelper(node->pRight, action);
}

// Hàm public
void VectorStore::forEach(void (*action)(vector<float>&, int, string&)) {
    if (!action) return; // phòng trường hợp action là nullptr
    forEachHelper(vectorStore->getRoot(), action);
}

// Helper function đệ quy
void VectorStore::collectIdsInOrder(AVLTree<double, VectorRecord>::AVLNode* node,
                                    std::vector<int>& ids) const
{
    if (!node) return;
    // Duyệt cây con bên trái
    collectIdsInOrder(node->pLeft, ids);

    // Thêm id của node hiện tại
    ids.push_back(node->data.id);

    // Duyệt cây con bên phải
    collectIdsInOrder(node->pRight, ids);
}

// Hàm public
std::vector<int> VectorStore::getAllIdsSortedByDistance() const
{
    std::vector<int> ids;
    collectIdsInOrder(vectorStore->getRoot(), ids);
    return ids;
}

// Helper function đệ quy
void VectorStore::collectVectorsInOrder(AVLTree<double, VectorRecord>::AVLNode* node,
                                        std::vector<VectorRecord*>& outList) const
{
    if (!node) return;

    // Duyệt cây con bên trái
    collectVectorsInOrder(node->pLeft, outList);

    // Thêm con trỏ tới VectorRecord hiện tại
    outList.push_back(&node->data);

    // Duyệt cây con bên phải
    collectVectorsInOrder(node->pRight, outList);
}

// Hàm public
std::vector<VectorRecord*> VectorStore::getAllVectorsSortedByDistance() const
{
    std::vector<VectorRecord*> result;
    collectVectorsInOrder(vectorStore->getRoot(), result);
    return result;
}



double VectorStore::estimateD_Linear(const std::vector<float>& query,
                                     int k,
                                     double averageDistance,
                                     const std::vector<float>& reference,
                                     double c0_bias,
                                     double c1_slope)
{
    if (query.size() != reference.size() || query.empty()) return 0.0;

    // 1. Tính dr = ||query - reference|| (khoảng cách Euclidean)
    double dr = 0.0;
    for (size_t i = 0; i < query.size(); ++i) {
        double diff = static_cast<double>(query[i]) - static_cast<double>(reference[i]);
        dr += diff * diff;
    }
    dr = std::sqrt(dr);

    // 2. Tính D theo công thức: D = |dr - averageDistance| + c1_slope * averageDistance * k + c0_bias
    double D = std::abs(dr - averageDistance) + c1_slope * averageDistance * k + c0_bias;

    return D;
}

int VectorStore::findNearest(const vector<float> &query, string metric){
    if (empty())
        return -1;

    // Kiểm tra metric hợp lệ
    if (metric != "euclidean" && metric != "cosine" && metric != "manhattan")
        throw invalid_metric();

    double minDist = std::numeric_limits<double>::max();
    int nearestId = -1;

    // Thu thập tất cả vectors
    std::vector<VectorRecord *> allVectors;
    collectAllVectorsHelper(vectorStore->root, allVectors);

    // Tìm vector gần nhất
    for (VectorRecord *record : allVectors)
    {
        // BỎ QUA vector có norm = 0 khi dùng cosine
        if (metric == "cosine" && record->norm < EPS)
            continue;

        double dist = distanceByMetric(query, *(record->vector), metric);
        
        // Nếu khoảng cách nhỏ hơn, hoặc bằng nhau nhưng ID nhỏ hơn (in-order)
        if (dist < minDist - EPS || (fabs(dist - minDist) < EPS && record->id < nearestId))
        {
            minDist = dist;
            nearestId = record->id;
        }
    }

    return nearestId;
}

// Helper: partition
int VectorStore::partitionPairs(vector<pair<double, int>> &arr, int low, int high)
{
    double pivot = arr[high].first;
    int i = low - 1;
    
    for (int j = low; j < high; j++)
    {
        if (arr[j].first < pivot)
        {
            i++;
            swap(arr[i], arr[j]);
        }
    }
    
    swap(arr[i + 1], arr[high]);
    return i + 1;
}

// Helper: quicksort
void VectorStore::quickSortPairs(vector<pair<double, int>> &arr, int low, int high)
{
    if (low < high)
    {
        int pi = partitionPairs(arr, low, high);
        quickSortPairs(arr, low, pi - 1);
        quickSortPairs(arr, pi + 1, high);
    }
}

// Top K nearest
int *VectorStore::topKNearest(const vector<float> &query, int k, string metric){
    // 1. Kiểm tra metric hợp lệ
    if (metric != "euclidean" && metric != "cosine" && metric != "manhattan")
        throw invalid_metric();
    
    // 2. Kiểm tra k hợp lệ
    if (k <= 0 || k > count)
        throw invalid_k_value();
    
    // 3. Kiểm tra store rỗng
    if (empty())
        return nullptr;

    // Tính chuẩn Euclidean của query
    double nq = 0.0;
    for (float val : query)
        nq += val * val;
    nq = std::sqrt(nq);

    // Ước lượng bán kính D
    double D = estimateD_Linear(query, k, averageDistance, *referenceVector);

    // Lọc bằng Red-Black Tree (normIndex)
    bool foundLower, foundUpper;
    double lowerBound = nq - D;
    double upperBound = nq + D;

    auto lowerNode = normIndex->lowerBound(lowerBound, foundLower);
    auto upperNode = normIndex->upperBound(upperBound, foundUpper);

    std::vector<std::pair<double, int>> candidates;

    if (!foundLower || !foundUpper)
    {
        // fallback: dùng toàn bộ vectorStore
        std::vector<VectorRecord*> allVectors;
        collectAllVectorsHelper(vectorStore->root, allVectors);

        for (auto r : allVectors)
        {
            double dist = distanceByMetric(query, *(r->vector), metric);
            candidates.push_back({dist, r->id});
        }
    }
    else
    {
        // Duyệt Red-Black Tree từ lowerNode đến upperNode
        auto current = lowerNode;
        while (current && current->key <= upperNode->key)
        {
            double dist = distanceByMetric(query, *(current->data.vector), metric);
            candidates.push_back({dist, current->data.id});

            // tìm successor
            if (current->right)
            {
                current = current->right;
                while (current->left)
                    current = current->left;
            }
            else
            {
                auto parent = current->parent;
                while (parent && current == parent->right)
                {
                    current = parent;
                    parent = parent->parent;
                }
                current = parent;
            }
        }
    }

    int m = candidates.size();
    std::cout << "Value m: " << m;  // ← PHẢI CÓ DÒNG NÀY!
    
    if (m == 0) return nullptr;

    // Sort bằng bubble sort
    for (int i = 0; i < m - 1; i++)
    {
        for (int j = i + 1; j < m; j++)
        {
            bool swapNeeded = false;
            if (fabs(candidates[i].first - candidates[j].first) > 1e-9)
                swapNeeded = candidates[i].first > candidates[j].first;
            else
                swapNeeded = candidates[i].second > candidates[j].second;

            if (swapNeeded)
                std::swap(candidates[i], candidates[j]);
        }
    }

    // Lấy top-k
    int limit = std::min(m, k);
    int* result = new int[limit];
    for (int i = 0; i < limit; i++)
        result[i] = candidates[i].second;

    return result;
}

void VectorStore::rangeQueryAVLHelper(
    AVLTree<double, VectorRecord>::AVLNode* node,
    double minDist,
    double maxDist,
    std::vector<int>& results) const
{
    if (!node) return;

    double key = node->key; // distanceFromReference

    // Nếu key lớn hơn minDist → duyệt left
    if (key > minDist)
        rangeQueryAVLHelper(node->pLeft, minDist, maxDist, results);

    // Nếu nằm trong range → thêm vào kết quả
    if (key >= minDist && key <= maxDist)
        results.push_back(node->data.id);

    // Nếu key nhỏ hơn maxDist → duyệt right
    if (key < maxDist)
        rangeQueryAVLHelper(node->pRight, minDist, maxDist, results);
}

int* VectorStore::rangeQueryFromRoot(double minDist, double maxDist) const
{
    if (!rootVector || minDist > maxDist)
        return nullptr;

    std::vector<int> results;
    rangeQueryAVLHelper(vectorStore->root, minDist, maxDist, results);

    int k = results.size();
    if (k == 0) return nullptr;

    // Trả về mảng ID thuần túy
    int* arr = new int[k];
    for (int i = 0; i < k; i++)
        arr[i] = results[i];

    return arr;
}

void VectorStore::rangeQueryHelper(
    AVLTree<double, VectorRecord>::AVLNode* node,
    const vector<float>& query,
    double radius,
    const string& metric,
    vector<int>& result)
{
    if (!node) return;

    // Duyệt trái
    rangeQueryHelper(node->pLeft, query, radius, metric, result);

    // Tính khoảng cách
    double dist = distanceByMetric(query, *(node->data.vector), metric);

    // Kiểm tra điều kiện bán kính
    if (dist <= radius) {
        result.push_back(node->data.id);
    }

    // Duyệt phải
    rangeQueryHelper(node->pRight, query, radius, metric, result);
}

// kiểm tra metric hợp lệ (thêm vào class VectorStore)
bool VectorStore::isValidMetric(const std::string &metric) const {
    return metric == "euclidean" || metric == "cosine" || metric == "manhattan";
}

int* VectorStore::rangeQuery(const std::vector<float> &query, double radius, std::string metric) const
{
    if (vectorStore == nullptr || vectorStore->root == nullptr)
        return nullptr;

    if (!isValidMetric(metric))
        throw invalid_metric();

    std::vector<std::pair<double, int>> candidates; // <distance, id>
    std::vector<VectorRecord*> allVectors;
    const_cast<VectorStore*>(this)->collectAllVectorsHelper(vectorStore->root, allVectors);

    double effectiveRadius = radius + 1e-7;

    // Thu thập các candidates trong bán kính
    for (VectorRecord* r : allVectors) {
        double dist = distanceByMetric(query, *(r->vector), metric);
        if (dist <= effectiveRadius) {
            candidates.push_back({dist, r->id});
        }
    }

    if (candidates.empty())
        return nullptr;

    // Sắp xếp theo distance tăng dần, nếu bằng nhau thì theo ID tăng dần
    for (size_t i = 0; i < candidates.size() - 1; i++) {
        for (size_t j = i + 1; j < candidates.size(); j++) {
            bool swapNeeded = false;
            
            // So sánh distance
            if (fabs(candidates[i].first - candidates[j].first) > 1e-9) {
                swapNeeded = candidates[i].first > candidates[j].first;
            } else {
                // Distance bằng nhau → so sánh ID
                swapNeeded = candidates[i].second > candidates[j].second;
            }
            
            if (swapNeeded) {
                std::swap(candidates[i], candidates[j]);
            }
        }
    }

    // Trả về mảng ID đã được sắp xếp
    int m = static_cast<int>(candidates.size());
    int* arr = new int[m];
    for (int i = 0; i < m; ++i) 
        arr[i] = candidates[i].second;

    return arr;
}

int* VectorStore::boundingBoxQuery(const std::vector<float>& minBound,
                                   const std::vector<float>& maxBound) const
{
    if (vectorStore == nullptr || vectorStore->root == nullptr)
        return nullptr;

    if (minBound.size() != maxBound.size() || minBound.empty())
        return nullptr;

    std::vector<VectorRecord*> allVectors;
    const_cast<VectorStore*>(this)->collectAllVectorsHelper(vectorStore->root, allVectors);

    std::vector<int> resultIds;

    for (VectorRecord* record : allVectors) {
        const std::vector<float>& vec = *(record->vector);
        if (vec.size() != minBound.size()) continue;

        bool inside = true;
        for (size_t i = 0; i < vec.size(); ++i) {
            if (vec[i] <= minBound[i] || vec[i] >= maxBound[i]) {
                inside = false;
                break;
            }
        }

        if (inside)
            resultIds.push_back(record->id);
    }

    if (resultIds.empty())
        return nullptr;

    // Trả về mảng ID thuần túy
    int m = static_cast<int>(resultIds.size());
    int* arr = new int[m];
    for (int i = 0; i < m; ++i)
        arr[i] = resultIds[i];

    return arr;
}

double VectorStore::getMaxDistance() const
{
    if (!vectorStore || vectorStore->empty())
        return 0.0;

    auto node = vectorStore->getRoot();
    while (node->pRight != nullptr)
    {
        node = node->pRight;
    }
    return node->key;
}


double VectorStore::getMinDistance() const
{
    if (!vectorStore || vectorStore->empty())
        return 0.0;

    auto node = vectorStore->getRoot();
    while (node->pLeft != nullptr)
    {
        node = node->pLeft;
    }
    return node->key;
}

VectorRecord VectorStore::computeCentroid(const std::vector<VectorRecord*>& records) const
{
    VectorRecord centroid;
    
    if (records.empty())
        return centroid; // Trả về centroid rỗng nếu không có vector

    size_t dim = records[0]->vector->size(); // chiều vector
    std::vector<float>* centroidVec = new std::vector<float>(dim, 0.0f);

    // 1. Cộng tất cả các vector
    for (const VectorRecord* rec : records)
    {
        for (size_t i = 0; i < dim; i++)
        {
            (*centroidVec)[i] += (*(rec->vector))[i];
        }
    }

    // 2. Chia cho số vector để lấy trung bình
    size_t m = records.size();
    for (size_t i = 0; i < dim; i++)
    {
        (*centroidVec)[i] /= static_cast<float>(m);
    }

    // 3. Tạo VectorRecord cho centroid
    centroid.id = -1;               // -1 hoặc giá trị đặc biệt cho centroid
    centroid.vector = centroidVec;
    centroid.rawText = "";           // centroid không có rawText
    centroid.rawLength = 0;
    centroid.distanceFromReference = 0.0; // chưa tính
    centroid.norm = 0.0;            // có thể tính nếu muốn

    // 4. Tính norm nếu cần
    double normVal = 0.0;
    for (float val : *centroidVec)
        normVal += val * val;
    centroid.norm = sqrt(normVal);

    return centroid;
}

void VectorStore::findVectorNearestToDistanceHelper(typename AVLTree<double, VectorRecord>::AVLNode *node,
                                                     double targetDistance,
                                                     double &minDiff,
                                                     VectorRecord *&result)
{
    if (!node)
        return;
    
    double diff = abs(node->data.distanceFromReference - targetDistance);
    if (diff < minDiff)
    {
        minDiff = diff;
        result = &node->data;
    }
    
    findVectorNearestToDistanceHelper(node->pLeft, targetDistance, minDiff, result);
    findVectorNearestToDistanceHelper(node->pRight, targetDistance, minDiff, result);
}

// Find vector nearest to distance
VectorRecord *VectorStore::findVectorNearestToDistance(double targetDistance) const
{
    VectorRecord *result = nullptr;
    double minDiff = numeric_limits<double>::max();
    const_cast<VectorStore *>(this)->findVectorNearestToDistanceHelper(vectorStore->root, targetDistance, minDiff, result);
    return result;
}

// Explicit template instantiation for the type used by VectorStore
template class AVLTree<double, VectorRecord>;
template class AVLTree<double, double>;
template class AVLTree<int, double>;
template class AVLTree<int, int>;
template class AVLTree<double, string>;
template class AVLTree<int, string>;

template class RedBlackTree<double, VectorRecord>;
template class RedBlackTree<double, double>;
template class RedBlackTree<int, double>;
template class RedBlackTree<int, int>;
template class RedBlackTree<double, string>;
template class RedBlackTree<int, string>;