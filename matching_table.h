#pragma once

const int INITIAL_CONS = 19;
const int MEDIAL_CONS = 21;
const int FINAL_CONS = 28;

static const wchar_t initialConsonant_kor[] =
{
    L'ㄱ', L'ㄲ', L'ㄴ', L'ㄷ',
    L'ㄸ', L'ㄹ', L'ㅁ', L'ㅂ',
    L'ㅃ', L'ㅅ', L'ㅆ', L'ㅇ',
    L'ㅈ', L'ㅉ', L'ㅊ', L'ㅋ',
    L'ㅌ', L'ㅍ', L'ㅎ'
};

static const wchar_t initialConsonant_eng[] =
{
    L'r', L'R', L's', L'e',
    L'E', L'f', L'a', L'q',
    L'Q', L't', L'T', L'd',
    L'w', L'W', L'c', L'z',
    L'x', L'v', L'g'
};

static const wchar_t medialConsonant_kor[] =
{
    L'ㅏ', L'ㅐ', L'ㅑ', L'ㅒ',
    L'ㅓ', L'ㅔ', L'ㅕ', L'ㅖ',
    L'ㅗ', L'ㅘ', L'ㅙ', L'ㅚ',
    L'ㅛ', L'ㅜ', L'ㅝ', L'ㅞ',
    L'ㅟ', L'ㅠ', L'ㅡ', L'ㅢ', L'ㅣ'
};

static const wchar_t medialConsonant_eng[] =
{
    L'k', L'o', L'i', L'O',
    L'j', L'p', L'u', L'P',
    L'h', L' ', L' ', L' ',
    L'y', L'n', L' ', L' ',
    L' ', L'b', L'm', L' ', L'l'
};

static const wchar_t finalConsonant_kor[] =
{
    L' ', L'ㄱ', L'ㄲ', L'ㄳ',
    L'ㄴ', L'ㄵ', L'ㄶ', L'ㄷ',
    L'ㄹ', L'ㄺ', L'ㄻ', L'ㄼ',
    L'ㄽ', L'ㄾ', L'ㄿ', L'ㅀ',
    L'ㅁ', L'ㅂ', L'ㅄ', L'ㅅ',
    L'ㅆ', L'ㅇ', L'ㅈ', L'ㅊ',
    L'ㅋ', L'ㅌ', L'ㅍ', L'ㅎ'
};

static const wchar_t finalConsonant_eng[] =
{
    L' ', L'r', L'R', L' ',
    L's', L' ', L' ', L'e',
    L'f', L' ', L' ', L' ',
    L' ', L' ', L' ', L' ',
    L'a', L'q', L' ', L't',
    L'T', L'd', L'w', L'c',
    L'z', L'x', L'v', L'g'
};

static const wchar_t pairConsonant_kor[] =
{
    L'ㅘ', L'ㅙ', L'ㅚ', L'ㅝ',
    L'ㅞ', L'ㅟ', L'ㅢ', L'ㄳ',
    L'ㄵ', L'ㄶ', L'ㄺ', L'ㄻ',
    L'ㄼ', L'ㄽ', L'ㄾ', L'ㄿ',
    L'ㅀ', L'ㅄ'
};

static const wchar_t pairConsonant_eng[][3] =
{
    L"hk", L"ho", L"hl", L"nj",
    L"np", L"nl", L"ml", L"rt",
    L"sw", L"sg", L"fr", L"fa",
    L"fq", L"ft", L"fx", L"fv",
    L"fg", L"qt"
};