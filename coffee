#include <string>
#include <vector>

using namespace std;

int solution(vector<string> order) {
    int answer = 0;
    
    int americano = 4500;
    int cafelatte = 5000;
   
    for (const string& coffee : order) {
        if (coffee.find("americano") != string::npos) {
            // "americano"가 포함된 모든 경우: 차가운 아메리카노
            answer += americano;
        } else if (coffee.find("cafelatte") != string::npos) {
            // "cafelatte"가 포함된 모든 경우: 차가운/따뜻한 카페 라테
            answer += cafelatte;
        } else {
            answer += americano;
        }// "anything" 또는 그 외의 경우: 차가운 아메리카노
    }
    
    return answer;
}
