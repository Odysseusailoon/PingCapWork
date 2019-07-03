


#include <iostream>
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <algorithm>
#include <queue>
#include <vector>
#include <stack>
#include <map>
#include <set>
#include <sstream>
#include <iomanip>
#include <cmath>

#include "bloomFilter.h"
#include "maintain.h"

using namespace std;
typedef long long llong;
typedef set<string>::iterator ssii;

const int maxn = 1000;

const float fac = 0.0001;

#define Cmp(a, b) memcmp(a, b, sizeof(b))
#define Cpy(a, b) memcpy(a, b, sizeof(b))
#define Set(a, v) memset(a, v, sizeof(a))
#define debug(x) cout << #x << ": " << x << endl
#define _rep(i, l, r) for(int i = (l); i <= (r); i++)
#define _for(i, l, r) for(int i = (l); i < (r); i++)
#define _forDown(i, l, r) for(int i = (l); i >= r; i--)
#define debug_(ch, i) printf(#ch"[%d]: %d\n", i, ch[i])
#define debug_m(mp, p) printf(#mp"[%d]: %d\n", p->first, p->second)
#define debugS(str) cout << "dbg: " << str << endl;
#define debugArr(arr, x, y) _for(i, 0, x) { _for(j, 0, y) printf("%c", arr[i][j]); printf("\n"); }

int main() {
    bloom_parameters parameters;
    parameters.projected_element_count = maxn;
    parameters.false_positive_probability = fac;
    parameters.random_seed = 0xA5A5A5A5;
    
    if(!parameters) {
        cout << "Error - invalid set of bloom filter" << endl;
        return 1;
    }
    
    parameters.compute_optimal_parameters();
    
    bloom_filter filter(parameters);
    string urlList[] = {"JIECHUAN", "JIECHUAN", "IKJ", "XYZ", "XYZ", "XYZ"};
    
    /*
    for(size_t i = 0; i < (sizeof(urlList)) / (sizeof(string)); i++) {
        int cnt = 0;
        if(filter.contains(urlList[i])) { debug(++cnt); cout << "exist: " << urlList[i] << endl; }
        if(!filter.contains(urlList[i])) filter.insert(urlList[i]);
    }
     */

    /*
    for(size_t i = 0; i < 100; i++)
        filter.insert(i);
    */
    
    /*
    for(size_t i = 0; i < (sizeof(urlList)) / (sizeof(string)); i++) {
        if(filter.contains(urlList[i]))
            cout << "contains: " << urlList[i] << endl;
    }
     */
    
    /*
    for(size_t i = 0; i < 100; i++) {
        if(filter.contains(i))
            cout << "contains num: " << i << endl;
    }
    */
    
    /*
    for(int i = -1; i > -100; --i) {
        if(filter.contains(i))
            cout << "falsely contains: " << i << endl;
    }
    */
    
    /*
    cout << endl << "othercheck: " << endl;
    cout << filter.contains("ACM-ICPC") << endl;
    puts("");
    */
    cout << "caculate: " << endl;
    
    int timing = 0;
    for(size_t i = 0; i < (sizeof(urlList) / sizeof(string)); i++) {
        
        if(timing % DL == 0) {
            combine();
            freshNode(highFreq);
        }
        
        // other
        if(!filter.contains(urlList[i])) {
            candiNode cur;
            cur.cnt = 1;
            cur.url = urlList[i];
            cur.accumulate += 1;
            
            tryInsert(cur);
            filter.insert(urlList[i]);
        }
        else {
            Node x(1, urlList[i], 1);
            updatePQ(x);
        }
        
        timing = timing % DL + 1;
        //debug(timing);
    }
    
    printf("================\n");
    
    while (!candidate.empty()) {
        candiNode x = candidate.top();
        candidate.pop();
        cout << x.url << " " << x.accumulate << endl;
    }
    
    printf("=================\n");
    
    while (!highFreq.empty()) {
        Node x = highFreq.top();
        highFreq.pop();
        cout << x.getURL() << endl;
    }
}
