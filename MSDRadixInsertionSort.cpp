
#include <iostream>
#include <cstdlib>
#include <vector>
#include <stack>
#include <iostream>
# include <chrono>
#include <assert.h>


using namespace std;
using std::cin;
using std::cout;
using std::endl;
using  ns = chrono::nanoseconds;
using get_time = chrono::steady_clock;


using namespace std;

//--------------------------------------------//
// helper function to generate a random array //
//--------------------------------------------//

// Node for the linked list in BucketSort
struct Node {
    unsigned long int val;
    Node * next;
    Node(unsigned long int val, Node * node) : val{val}, next{node} {}
};

// Calculates a^n
unsigned long int pow (unsigned long int a, unsigned long int n) {
    if (n == 0) {
        return 1;
    }

    unsigned long int x = pow(a, n / 2);
    if (n % 2 == 0) {
        return (x * x);
    }
    else {
        return (a * x * x);
    }
}

// Finds the nth digit of a number x
int nthDigit(unsigned long int x, int n) {

    unsigned long int a = x / pow(10, 10 - n - 1);

    return a % 10;
}


void bucketSort(vector<unsigned long int> &A, int l, int r, int d) {

    // Creates a vector of pairs
    // pair.first represents the linked list, pair.second represents the last element of the linked list
    vector<std::pair<Node *, Node *>> B = vector<std::pair<Node *, Node *>>(10, std::pair<Node *, Node *>(nullptr, nullptr));

    for (int i = l; i <= r; i++) {
        // create a new element node
        Node * elm = new Node(A[i], nullptr);
        int dthDigit = nthDigit(A[i], d);

        // place it in the dth digit bucket, if an element exists, append it to the end of the linked list there
        if (B[dthDigit].first != nullptr) {
            B[dthDigit].second->next = elm;
            B[dthDigit].second = elm;
        }
        // otherwise create a new list at the index
        else {
            B[dthDigit].first = elm;
            B[dthDigit].second = elm;
        }
    }

    int index = l;
    for (int i = 0; i < 10; i++) {
        // while there exists a linked list
        while (B[i].first != nullptr) {
            auto tmp = B[i].first;
            // add the head of the list to the indexed position in the original Array
            A[index++] = tmp->val;
            // remove the current head and make the next element the new head in the list
            B[i].first = tmp->next;
            delete tmp;
        }
    }
}


void swap(unsigned long int &A, unsigned long int &B) {
    auto tmp = A;
    A = B;
    B = tmp;
}

void insertionSort(vector<unsigned long int> &A, int n) {
    for (int i = 0; i < n; i++) {
        int j = i;
        while (j > 0 && A[j] < A[j - 1]) {
            swap(A[j], A[j-1]);
            j--;
        }
    }
}


//---------------------------------------------------------//
// MSDSort implemented as in the assignment description    //
//---------------------------------------------------------//

void MSDSort(vector<unsigned long int> &A, int m, int l, int r, int d, int threshold, int &total_num_calls)
{
    total_num_calls++;

    if (A.size() <= threshold) {
        insertionSort(A, A.size());
        return;
    }

    if (l < r) {
        bucketSort(A, l, r, d);
    }

    if (m - d > 1) {
        int l_Prime = l;
        while (l_Prime < r) {
            int dthDigit = nthDigit(A[l_Prime], d);
            // get the start and end of the subarray of elements that have the same dth digit
            int r_Prime = l_Prime;
            while (dthDigit == nthDigit(A[r_Prime], d)) {
                r_Prime++;
            }
            r_Prime--;
            // Run MSDSort on that subarray of elements
            MSDSort(A, m, l_Prime, r_Prime, d + 1, threshold, total_num_calls);
            l_Prime = r_Prime + 1;
        }
    }
}



void getRandomArray(vector<unsigned long int>& A)
{

    for (unsigned int i = 0; i < A.size(); i++)
        A[i] = (unsigned long int) rand() * (unsigned long int)  rand() * (unsigned long int)  rand();

}

#ifndef TESTING
int main()
{
    int m = 10;
    int threshold = 2;
    int n = 100000;
    vector<unsigned long int> A;

    // for tests on a random array, set this variable to false
    bool read_input_from_cin = true;
    
    if (read_input_from_cin)
    {
        cin >> n;  // read the size of array A
        cin >> threshold;  // read the threshold for the sorting algorithm 

        A.resize(n);

        // read array A from standard input
        for (int i = 0; i < n; i++)
            cin >> A[i];

    }
    else {
        A.resize(n);
        getRandomArray(A);

    }
    
    auto start = get_time::now(); //use auto keyword to minimize typing strokes :)

    int total_num_calls = 0;
    MSDSort(A, m, 0, n-1, 0, threshold, total_num_calls);
 
    auto end = get_time::now();
    auto diff = end - start;

    
    bool sorted = true;
    // print re-arranged A  to standard output    
    cout << A[0] << endl;
    for (int i = 1; i < n; i++)
    {
        cout << A[i] << endl;
        if (A[i] < A[i - 1])
            sorted = false;       
    }
    
     cout << total_num_calls << endl;
        
    // uncomment this line to have running time printed to the standard output
    // cout << "Time to sort: " <<  chrono::duration <double, milli>(diff).count() << " ms" << endl;

    return 0;

}

#endif