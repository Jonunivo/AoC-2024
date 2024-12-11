#include <bits/stdc++.h>
using namespace std;
#define pb push_back
#define mp make_pair
#define int long long
vector <int> getNums(const string& str) {
    stringstream ss(str);
    vector <int> a;
    int num;
    while(ss >> num) a.push_back(num);
    return a;
}
int findNumberofDigits(int x) {
    return floor(log10(x)) + 1;
}
int32_t main() {
    string line;
    vector <int> ad;
    while (getline(cin, line)) {
        ad = getNums(line); 
    }
    map <int, int> h;
    for(auto x: ad) {
        h[x]++;
    }
    int blink = 75;
    for(int i = 0; i < blink; i++) {
        map <int, int> b;
        for(auto it = h.begin(); it != h.end(); it++) {
            int x = it->first;
            int cnt = it->second;
            int dig = findNumberofDigits(x);
            if(x == 0) b[1] += cnt;
            else if(dig % 2 == 0) {
                int hlf = dig / 2;
                int fst = x / pow(10, hlf);
                int snd = x % (int)pow(10, hlf);
                b[fst] += cnt;
                b[snd] += cnt;
            }
            else{
                b[x*2024] += cnt;
            }
        }
        h = b;
    }
    int ans = 0;
    for(auto it = h.begin(); it != h.end(); it++) {
        ans += it->second;
    }
    cout << ans << endl;

}
