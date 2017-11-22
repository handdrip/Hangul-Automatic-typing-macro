#pragma once
#include "precompiled.h"
#include "nana_subclassing.h"

namespace at
{
    static std::mutex g_mutex;
    std::condition_variable g_convar;
    std::atomic<bool> g_isStarted = false;

    using valint = const unsigned int;
    auto g_phonemes = std::make_unique<std::wstring>(L"");
    std::unordered_map<unsigned long, std::wstring> g_matchingMap;

    void initTable()
    {
        auto length = static_cast<valint> (wcslen(initialConsonant_kor));
        for (int i = 0; i < length; i++)
        {
            valint index = static_cast<valint>(initialConsonant_kor[i]);
            g_matchingMap[index] = initialConsonant_eng[i];
        }

        std::wstring &&pairConsonant = pairConsonant_kor;

        length = static_cast<valint> (wcslen(medialConsonant_kor));
        for (int i = 0; i < length; i++)
        {
            valint index = static_cast<valint>(medialConsonant_kor[i]);
            auto position = pairConsonant.find(medialConsonant_kor[i]);

            if (position == std::wstring::npos)
                g_matchingMap[index] = medialConsonant_eng[i];
            else
                g_matchingMap[index] = pairConsonant_eng[position];
        }

        length = static_cast<valint> (wcslen(finalConsonant_kor));
        for (int i = 0; i < length; i++)
        {
            valint index = static_cast<valint>(finalConsonant_kor[i]);
            auto position = pairConsonant.find(finalConsonant_kor[i]);

            if (position == std::wstring::npos)
                g_matchingMap[index] = finalConsonant_eng[i];
            else
                g_matchingMap[index] = pairConsonant_eng[position];
        }
    }

    bool isSpecialletter(const wchar_t& letter)
    {
        return (letter == L'\r' || letter == L' ') ? true : false;
    }

    class Phonemicizer
    {
    public:
        Phonemicizer()
        {
        }

    public:
        static void perform(const std::wstring& text)
        {
            static const wchar_t SPACE = L' ';
            static const wchar_t START = 0xAC00;
            static const int MF = MEDIAL_CONS * FINAL_CONS;

            assert(text.size() != 0);

            std::wstring result;

            int interval = 0;

            for (const auto& letter : text)
            {
                interval = letter - START;

                if (letter == SPACE || interval < 0)
                {
                    result.push_back(letter);
                    continue;
                }
                else if (letter == L'\n')
                {
                    continue;
                }

                wchar_t phonemes[4] = { 0, };

                phonemes[0] = initialConsonant_kor[interval / MF];
                phonemes[1] = medialConsonant_kor[interval % MF / FINAL_CONS];

                valint P = interval % FINAL_CONS;
                phonemes[2] = (finalConsonant_kor[P] == SPACE) ? L'\0' : finalConsonant_kor[P];

                result.append(phonemes);
            }

            toAlphabet(result);
        }

    private:
        static void toAlphabet(const std::wstring& phonemes)
        {
            assert(g_matchingMap.begin() != g_matchingMap.end());

            at::g_phonemes->clear();

            std::wstring result;
            for (const auto& letter : phonemes)
            {
                if (isSpecialletter(letter))
                    result.push_back(letter);
                else
                    result.append(g_matchingMap[letter]);
            }

            at::g_phonemes = std::make_unique<std::wstring>(result);
        }
    };

    int getKeyType(const std::wstring&& keyString)
    {
        std::wstringstream ws;

        std::wstring validKeyString;

        std::copy_if(
            keyString.begin(), keyString.end(), 
            std::back_inserter(validKeyString),
            [](wchar_t letter) 
        {
            return letter != L'0';
        });

        ws << L"0x" << validKeyString;

        auto keycode = wcstol(ws.str().c_str(), NULL, 16);

        assert(keycode != 0);
        return static_cast<int>(keycode);
    }

    class AutomaticTyper : public std::thread
    {
    public:
        AutomaticTyper() : std::thread([this]()
        {
            while (true)
            {
                g_convar.wait(
                    std::unique_lock<std::mutex>(g_mutex),
                    [this] { return g_isStarted == true; });

                for (const auto& phoneme : *g_phonemes)
                {
                    generateKeyboardEvent(phoneme);
                    std::this_thread::sleep_for(std::chrono::milliseconds(5));
                }
            }
        })
        {
        }

    public:
        void perform()
        {
            g_isStarted.store(true);
            g_convar.notify_all();
        }

        void stop()
        {
            g_isStarted.store(false);
        }

    private:
        void generateKeyboardEvent(const wchar_t& letter)
        {
            switch (getType(letter))
            {
            case 0:
                uppercase(letter);
                break;
            case 1:
                keybd_event(letter - 32, 0, KEYEVENTF_EXTENDEDKEY, 0);
                break;
            case 2:
                keybd_event(letter, 0, KEYEVENTF_EXTENDEDKEY, 0);
                break;
            }
        }

        int getType(const wchar_t& letter) const 
        {
            if (letter >= 65 && letter <= 90)
            {
                return 0;   // uppercase
            }
            else if (letter >= 97 && letter <= 122)
            {
                return 1;   // lowercase
            }
            else
            {
                return 2;   // else
            }
        }

        void uppercase(const wchar_t& letter) const
        {
            keybd_event(VK_SHIFT, MapVirtualKey(VK_SHIFT, 0), KEYEVENTF_SCANCODE, 0);
            keybd_event(letter, 0, KEYEVENTF_EXTENDEDKEY, 0);
            keybd_event(VK_SHIFT, MapVirtualKey(VK_SHIFT, 0), KEYEVENTF_SCANCODE | KEYEVENTF_KEYUP, 0);
        }
    };

    class HotkeyManager
    {
    public:
        HotkeyManager(const HWND& hWnd) : win_handle_(hWnd)
        {
            RegisterHotKey(win_handle_, 100, MOD_NOREPEAT, VK_HOME);
            RegisterHotKey(win_handle_, 101, MOD_NOREPEAT, VK_END);
        }

        ~HotkeyManager()
        {
            UnregisterHotKey(win_handle_, 100);
            UnregisterHotKey(win_handle_, 101);
        }
    private:
        const HWND win_handle_;
    };

    class AutoTyperForm : public nana::form
    {
    public:
        AutoTyperForm()
        {
            init();
        }

        ~AutoTyperForm()
        {
        }

        void init()
        {
            layout_ = std::make_unique<nana::place>(*this);
            editor_ = std::make_shared<nana::textbox>(*this);
            buttons_[0] = std::make_unique<nana::button>(*this, "START (Home)");
            buttons_[1] = std::make_unique<nana::button>(*this, "STOP (End)");

            this->caption("Auto Typer");
            editor_->multi_lines(true);
            layout_->div("<><weight=90% vert <><weight=90% <><weight=100% vert <weight=70% editBox><weight=5%><buttons arrange=[48%,48%] gap=4%>><>><>><>");
            layout_->field("editBox") << *editor_;
            (*layout_)["buttons"] << *buttons_[0] << *buttons_[1];

            editor_->edited();

            layout_->collocate();
            this->show();
        }

        std::shared_ptr<nana::textbox> editor() const
        {
            return editor_;
        }

    private:
        std::unique_ptr<nana::place> layout_;
        std::shared_ptr<nana::textbox> editor_;
        std::unique_ptr<nana::button> buttons_[2];
    };

    class AutoTyperManager
    {
    public:
        AutoTyperManager()
        {
            init();
        }

        ~AutoTyperManager()
        {
            g_isStarted.store(false);
            auto_typer_.detach();
        }

    private:
        void init()
        {
            at::initTable();
            form_ = std::make_unique<AutoTyperForm>();

            HWND wd = reinterpret_cast<HWND>(nana::API::root(*form_));
            hotman_ = std::make_unique<at::HotkeyManager>(wd);

            receiver_ = std::make_unique<subclass>(*form_);
            phonemicizer_ = std::make_unique<at::Phonemicizer>();
            setupEvent();
        }

        void setupEvent()
        {
            receiver_->make_after(WM_HOTKEY, [&](UINT id, WPARAM wp, LPARAM lp, LRESULT* lr)
            {
                wchar_t keyString[10];
                wsprintfW(keyString, L"%X", lp);

                switch (at::getKeyType(keyString))
                {
                case VK_HOME:
                    at::Phonemicizer::perform(form_->editor()->caption_wstring());
                    auto_typer_.perform();
                    break;

                case VK_END:
                    auto_typer_.stop();
                    break;
                }
                return true;
            });
        }

    public:
        std::unique_ptr<AutoTyperForm> form_;
        std::unique_ptr<subclass> receiver_;
        std::unique_ptr<at::HotkeyManager> hotman_;
        std::unique_ptr<at::Phonemicizer> phonemicizer_;
        at::AutomaticTyper auto_typer_;
    };
}

int main()
{
    using namespace std;
    _wsetlocale(LC_ALL, L"");

    at::AutoTyperManager atm;
    nana::exec();

    return 0;
}