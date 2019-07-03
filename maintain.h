

#ifndef maintain_h
#define maintain_h


using namespace std;
typedef long long llong;
const int K = 10 * 2;
const int candiK = 10 * 2;
const int threshold = 5;
const int SIZE = 1000;
const int DL = 10;
// frequency threshold


class Node {
private:
    llong cnt;
    string url;
    int accumulate;
public:
    Node() {
        cnt = 0;
        accumulate = 0;
        url.clear();
    }
    
    Node(llong _cnt, string _url, int _accu = 0) : cnt(_cnt), url(_url), accumulate(_accu) {}
    
    llong getCnt() const {
        return cnt;
    }
    
    void updateCnt(const llong d) {
        cnt += (llong) d;
    }
    
    void updateAccu() {
        accumulate += 1;
    }
    
    void freshAccu() {
        accumulate = 0;
    }
    
    int getAccu() const {
        return accumulate;
    }
    
    string getURL() const {
        return url;
    }
    
    bool operator< (const Node& rhs) const {
        if(cnt != rhs.cnt) return cnt > rhs.cnt;
        else return accumulate > rhs.accumulate;
    }
};

class candiNode {
public:
    int accumulate;
    llong cnt;
    string url;
    
    candiNode(const Node& nd) {
        accumulate = nd.getAccu();
        cnt = nd.getCnt();
        url = nd.getURL();
    }
    
    candiNode() {
        url.clear();
        accumulate = 0;
        cnt = 0;
    }
    
    // candiNode(llong _cnt = 0, string _url = "", int _accu = 0) : cnt(_cnt), url(_url), accumulate(_accu) {}
    
    bool operator < (const candiNode& rhs) const {
        return accumulate > rhs.accumulate;
    }
};

static priority_queue<Node> highFreq;
static priority_queue<candiNode> candidate;
// candidate

void clean(priority_queue<candiNode>& pq) {
    while (!pq.empty()) {
        pq.pop();
    }
}

void freshNode(priority_queue<Node>& pq) {
    vector<Node> buffer;
    while (!pq.empty()) {
        Node x = pq.top();
        pq.pop();
        
        x.freshAccu();
        
        buffer.push_back(x);
    }
    
    for(auto& i : buffer) {
        pq.push(i);
    }
}

void updatePQ(Node& target) {
    int found = 0;
    vector<Node> buffer1;
    vector<candiNode> buffer2;
    
    while (!highFreq.empty()) {
        Node x = highFreq.top();
        highFreq.pop();
        
        if(x.getURL() == target.getURL()) {
            found = 1;
            cout << "OK" << endl;
            x.updateCnt(target.getCnt());
            // x.updateAccu();
        }
        
        buffer1.push_back(x);
    }
    for(auto& i : buffer1) highFreq.push(i);
    buffer1.clear();
    
    if(found) return;
    else {
        while (!candidate.empty()) {
            candiNode x = candidate.top();
            candidate.pop();
            
            if(x.url == target.getURL()) {
                x.cnt++;
                x.accumulate++;
            }
            buffer2.push_back(x);
        }
        
        for(auto& i : buffer2) candidate.push(i);
        buffer2.clear();
        return;
    }
}

void combine() {
    // greedy algorithm:
    // choose accu max to -> pq
    
    while (candidate.size() > candiK) {
        candidate.pop();
    }
    
    vector<candiNode> buffer;
    while (!candidate.empty()) {
        candiNode x = candidate.top();
        candidate.pop();
        
        buffer.push_back(x);
    }
    
    for(auto& i : buffer) {
        if(highFreq.empty() || i.cnt > threshold || i.accumulate > highFreq.top().getAccu()) {
            Node cur(i.cnt, i.url, i.accumulate);
            highFreq.push(cur);
        }
    }
    
    buffer.clear();
    
    while (highFreq.size() > SIZE) {
        highFreq.pop();
    }
    
    clean(candidate);
}

void tryInsert(candiNode& t) {
    cout << "dbg: " << t.url << endl;
    candidate.push(t);
}

#endif /* maintain_h */
