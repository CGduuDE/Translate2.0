#include <exception>
#include <iostream>
#include <ostream>
#include <stdexcept>
#include <string>
#include <windows.h>
using namespace std;

class RaiiClipboard
{
public:
    RaiiClipboard()
    {
        if (!OpenClipboard(nullptr))
            throw runtime_error("Can't open clipboard.");
        
    }

    ~RaiiClipboard()
    {
        CloseClipboard();
    }

     
private:
    RaiiClipboard(const RaiiClipboard&);
    RaiiClipboard& operator=(const RaiiClipboard&);
};

class RaiiTextGlobalLock
{
public:
    explicit RaiiTextGlobalLock(HANDLE hData)
        : m_hData(hData)
    {
        m_psz = static_cast<const char*>(GlobalLock(m_hData));
        if (!m_psz)
            throw runtime_error("Can't acquire lock on clipboard text.");
    }

    ~RaiiTextGlobalLock()
    {
        GlobalUnlock(m_hData);
    }

    const char* Get() const
    {
        return m_psz;
    }

private:
    HANDLE m_hData;
    const char* m_psz;

    
    RaiiTextGlobalLock(const RaiiTextGlobalLock&);
    RaiiTextGlobalLock& operator=(const RaiiTextGlobalLock&);
};

string GetClipboardText()
{
    RaiiClipboard clipboard;

    HANDLE hData = GetClipboardData(CF_TEXT);
    if (hData == nullptr)
        throw runtime_error("Can't get clipboard text.");

    RaiiTextGlobalLock textGlobalLock(hData);
    string text(textGlobalLock.Get());

    return text;
}



void TextToClipboar(const char* str)
{
    RaiiClipboard clipboard;
    HGLOBAL hMem = GlobalAlloc(GMEM_MOVEABLE, strlen(str) + 1);
    memcpy(GlobalLock(hMem), str, strlen(str) + 1);
    GlobalUnlock(hMem);

    EmptyClipboard();
    SetClipboardData(CF_TEXT, hMem);

}

int find(char a, char mass[]) {
    for (int i = 0; i < 28; i++) {
        if (a == mass[i]) {
            return i;
        }
    }

    return 0;
}


std::string translate(std::string str) { //ghbdtn rfr ltkf
    std::string out_string;
    int a = 0;
    char mass[28] = { ' ','q','w','e','r','t','y','u','i','o','p','a','s','d','f','g','h','j','k','l','z','x','c','v','b','n','m' };
    char mass1[28] = { ' ','й','ц','у','к','е','н','г','ш', 'щ', 'з', 'ф', 'ы', 'в', 'а', 'п', 'р', 'о', 'л', 'д', 'я', 'ч', 'с', 'м', 'и', 'т', 'ь', };

    for (int i = 0; i < str.length(); i++) {
        a = find(str[i], mass);
        out_string += mass1[a];
    }

    return out_string;
}

int main()
{
    setlocale(LC_ALL, "RUS");

    string b = GetClipboardText();
    b = translate(b);
    const char* a = b.c_str();
    TextToClipboar(a);

    return 0;
}
