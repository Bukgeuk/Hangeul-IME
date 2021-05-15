#include <iostream>
#include <string>
#include <vector>

using namespace std;

constexpr int UNICODE_HANGUL_START = 0xAC00; // 유니코드 한글 시작
constexpr int UNICODE_CONSONANT_START = 0x11A8 - 1; // 유니코드 자음 시작
constexpr int UNICODE_VOWEL_START = 0x1161; // 유니코드 모음 시작

const vector<string> first_list = { "r", "R", "s", "e", "E", "f", "a", "q",
        "Q", "t", "T", "d", "w", "W", "c", "z", "x", "v", "g" };

const vector<string> middle_list = { "k", "o", "i", "O", "j", "p", "u", "P",
    "h", "hk", "ho", "hl", "y", "n", "nj", "np", "nl", "b", "m",
    "ml", "l" };

const vector<string> final_list = { " ", "r", "R", "rt", "s", "sw", "sg", "e",
    "f", "fr", "fa", "fq", "ft", "fx", "fv", "fg", "a",
    "q", "qt", "t", "T", "d", "w", "c", "z", "x", "v", "g" };

enum class Type {
    First, // 초성
    Middle, // 중성
    Final, // 종성
    Separate, // 독립된 글자
    Other // 기타 문자
};

// 자모음 체크
int isConsonantOrVowel(char c) {
    switch (c) {
    case 'r': // ㄱ
    case 'R': // ㄲ
    case 's': // ㄴ
    case 'e': // ㄷ
    case 'E': // ㄸ
    case 'f': // ㄹ
    case 'a': // ㅁ
    case 'q': // ㅂ
    case 'Q': // ㅃ
    case 't': // ㅅ
    case 'T': // ㅆ
    case 'd': // ㅇ
    case 'w': // ㅈ
    case 'W': // ㅉ
    case 'c': // ㅊ
    case 'z': // ㅋ
    case 'x': // ㅌ
    case 'v': // ㅍ
    case 'g': // ㅎ
        return 1;
    case 'k': // ㅏ
    case 'o': // ㅐ
    case 'i': // ㅑ
    case 'O': // ㅒ
    case 'j': // ㅓ
    case 'p': // ㅔ
    case 'u': // ㅕ
    case 'P': // ㅖ
    case 'h': // ㅗ
    case 'y': // ㅛ
    case 'n': // ㅜ
    case 'b': // ㅠ
    case 'm': // ㅡ
    case 'l': // ㅣ
        return 2;
    default:
        return -1;
    }
}

// 종성 결합
int combineFinal(char a, char b) {
    string s(1, a);
    s += b;

    auto result = find(final_list.begin(), final_list.end(), s);
    return result == final_list.end() ? -1 : result - final_list.begin();
}

// 중성 결합
int combineMiddle(char a, char b) {
    string s(1, a);
    s += b;

    auto result = find(middle_list.begin(), middle_list.end(), s);
    return result == middle_list.end() ? -1 : result - middle_list.begin();
}

vector<Type> divide(string str) {
    vector<Type> v;

    for (int i = 0; i < str.length(); i++) {
        if (isConsonantOrVowel(str[i]) == 1) { // 자음일때
            if (i < str.length() - 1) { // 뒤에 다른 글자가 있을 때
                if (isConsonantOrVowel(str[i + 1]) == 2) { // 그 글자가 모음이라면
                    v.push_back(Type::First); // 초성으로 넣어줌
                    continue;
                }
                if (i > 1) { // 앞에 다른 두 글자가 있을 때
                    if (v[i - 2] == Type::First && v[i - 1] == Type::Middle // 앞 두 글자가 초성 - 중성이고
                        && isConsonantOrVowel(str[i + 1]) != 2) { // 뒤 글자가 자음이거나 기타 문자라면
                        v.push_back(Type::Final); // 종성으로 넣어줌
                        continue;
                    }
                }
                if (i > 2) { // 앞에 다른 세 글자가 있을 때
                    if (v[i - 3] == Type::First && v[i - 2] == Type::Middle && v[i - 1] == Type::Final // 앞 세 글자가 초성 - 중성 - 종성이고
                        && isConsonantOrVowel(str[i + 1]) != 2 // 뒤 글자가 자음이거나 기타 문자이고
                        && combineFinal(str[i - 1], str[i]) != -1) { // 앞 종성과 현재 자음이 결합된다면
                        v.push_back(Type::Final); // 종성으로 넣어줌
                        continue;
                    }
                    if (v[i - 3] == Type::First && v[i - 2] == Type::Middle && v[i - 1] == Type::Middle // 앞 세 글자가 초성 - 중성 - 중성이고
                        && isConsonantOrVowel(str[i + 1]) != 2) { // 뒤 글자가 자음이거나 기타 문자라면
                        v.push_back(Type::Final); // 종성으로 넣어줌
                        continue;
                    }
                }
                if (i > 3) { // 앞에 다른 네 글자가 있을 때
                    if (v[i - 4] == Type::First && v[i - 3] == Type::Middle && v[i - 2] == Type::Middle && v[i - 1] == Type::Final // 앞 네 글자가 초성 - 중성 - 중성 - 종성이고
                        && isConsonantOrVowel(str[i + 1]) != 2 // 뒤 글자가 자음이거나 기타 문자이고
                        && combineFinal(str[i - 1], str[i]) != -1) { // 앞 종성과 현재 자음이 결합된다면
                        v.push_back(Type::Final); // 종성으로 넣어줌
                        continue;
                    }
                }
            } else { // 뒤에 다른 글자가 없을 때
                if (i > 1) { // 앞에 다른 두 글자가 있을 때
                    if (v[i - 2] == Type::First && v[i - 1] == Type::Middle) { // 앞 두 글자가 초성 - 중성이라면
                        v.push_back(Type::Final); // 종성으로 넣어줌
                        continue;
                    }
                }
                if (i > 2) { // 앞에 다른 세 글자가 있을 때
                    if (v[i - 3] == Type::First && v[i - 2] == Type::Middle && v[i - 1] == Type::Final // 앞 세 글자가 초성 - 중성 - 종성이고
                        && combineFinal(str[i - 1], str[i]) != -1) { // 앞 종성과 현재 자음이 결합된다면
                        v.push_back(Type::Final); // 종성으로 넣어줌
                        continue;
                    }
                    if (v[i - 3] == Type::First && v[i - 2] == Type::Middle && v[i - 1] == Type::Middle) { // 앞 세 글자가 초성 - 중성 - 중성이라면
                        v.push_back(Type::Final); // 종성으로 넣어줌
                        continue;
                    }
                }
                if (i > 3) { // 앞에 다른 네 글자가 있을 때
                    if (v[i - 4] == Type::First && v[i - 3] == Type::Middle && v[i - 2] == Type::Middle && v[i - 1] == Type::Final // 앞 네 글자가 초성 - 중성 - 중성 - 종성이고
                        && combineFinal(str[i - 1], str[i]) != -1) { // 앞 종성과 현재 자음이 결합된다면
                        v.push_back(Type::Final); // 종성으로 넣어줌
                        continue;
                    }
                }
            }

            v.push_back(Type::Separate); // 위 조건에 모두 해당되지 않는다면 독립된 글자로 넣어줌
            continue;
        } else if (isConsonantOrVowel(str[i]) == 2) { // 모음일때
            if (i != 0) { // 앞에 다른 글자가 있을 때
                if (v.back() == Type::First) { // 그 글자가 초성이라면
                    v.push_back(Type::Middle); // 중성으로 넣어줌
                    continue;
                }
                if (v.back() == Type::Middle // 그 글자가 중성이고
                    && combineMiddle(str[i - 1], str[i]) != -1) { // 현재 모음과 결합된다면
                    v.push_back(Type::Middle); // 중성으로 넣어줌
                    continue;
                }
            }

            v.push_back(Type::Separate); // 위 조건에 모두 해당되지 않는다면 독립된 글자로 넣어줌
            continue;
        } else if ((str[i] >= 32 && str[i] <= 64) || (str[i] >= 91 && str[i] <= 96) || (str[i] >= 123 && str[i] <= 126)) { // 기타 문자
            v.push_back(Type::Other);
            continue;
        }
    }

    return v;
}

wstring combine(string str, vector<Type> typelist) {
    wstring result;
    int i = 0;

    while (i < str.length()) {
        if (typelist[i] == Type::Separate) {
            if (isConsonantOrVowel(str[i]) == 1) { // 자음이라면
                if (i < str.length() - 1) { // 뒤에 글자가 더 있고
                    int combinedFinal = combineFinal(str[i], str[i + 1]);
                    if (isConsonantOrVowel(str[i + 1]) == 1 && combinedFinal != -1) { // 그 글자가 자음이고 현재 글자와 결합 가능할 때
                        result.push_back(UNICODE_CONSONANT_START + combinedFinal);
                        i++;
                        continue;
                    }
                }
                int idx = find(final_list.begin(), final_list.end(), string(1, str[i])) - final_list.begin();
                result.push_back(UNICODE_CONSONANT_START + idx);
                i++;
                continue;
            }
            else if (isConsonantOrVowel(str[i]) == 2) { // 모음이라면
                int idx = find(middle_list.begin(), middle_list.end(), string(1, str[i])) - middle_list.begin();
                result.push_back(UNICODE_VOWEL_START + idx);
                i++;
                continue;
            }
        }
        else if (typelist[i] == Type::Other) {
            result.push_back(str[i]);
            i++;
            continue;
        }

        // 초성 : 무조건 1개
        int firstIdx = 0;
        if (typelist[i] == Type::First) {
            firstIdx = find(first_list.begin(), first_list.end(), string(1, str[i])) - first_list.begin();
            i++;
        }

        // 중성 : 1개 ~ 2개
        int middleIdx = 0;
        if (typelist[i] == Type::Middle) {
            middleIdx = find(middle_list.begin(), middle_list.end(), string(1, str[i])) - middle_list.begin();
            i++;

            if (i < str.length()) {
                if (typelist[i] == Type::Middle) {
                    int combinedMiddle = combineMiddle(str[i - 1], str[i]);

                    if (combinedMiddle != -1)
                        middleIdx = combinedMiddle;

                    i++;
                }
            }
        }

        // 종성 : 0개 ~ 2개
        int finalIdx = 0;
        
        if (i < str.length()) {
            if (typelist[i] == Type::Final) {
                finalIdx = find(final_list.begin(), final_list.end(), string(1, str[i])) - final_list.begin();
                i++;

                if (i < str.length()) {
                    if (typelist[i] == Type::Final) {
                        int combinedFinal = combineFinal(str[i - 1], str[i]);

                        if (combinedFinal != -1)
                            finalIdx = combinedFinal;

                        i++;
                    }
                }
            }
        }

        // 결합
        result.push_back(UNICODE_HANGUL_START + (firstIdx * 21 * 28) + (middleIdx * 28) + finalIdx);
    }

    return result;
}

int main(void) {
    string temp;

    wcout.imbue(std::locale("korean"));
    
    while (1) {
        getline(cin, temp);
        auto tl = divide(temp);
        wcout << combine(temp, tl) << endl;
    }

    return 0;
}
