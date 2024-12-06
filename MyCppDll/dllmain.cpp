/////////////////////////////////////////////////////////////////////////////////////////
// Native C++ Test DLL
// by Giovanni Dicanio
//
// NOTE:
// This DLL is not meant to be used by external C/C++ code, so there is no
// separate header file (e.g. MyDll.h) exporting public interface function declarations
// and structs.
// This DLL is simply part of a VS solution meant to show how to pass a custom struct
// from C# to native C++ via PInvoke.
//
/////////////////////////////////////////////////////////////////////////////////////////


// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"


//
// Struct to be passed from the outside (e.g. C# application) to this native C++ DLL
//
struct MyCppDll_Data
{
    GUID Id;
    int Value;
    const wchar_t* Name;
};


//
// Anonymous namespace for private helper stuff
//
namespace {

//
// Simple RAII wrapper around ::CoTaskMemFree
//
class CAutoCoTaskMemFree
{
public:
    CAutoCoTaskMemFree() noexcept
        : m_ptr(nullptr)
    {
    }

    explicit CAutoCoTaskMemFree(void* ptr) noexcept
        : m_ptr(ptr)
    {
    }

    void* Get() const noexcept
    {
        return m_ptr;
    }

    bool IsNull() const noexcept
    {
        return m_ptr == nullptr;
    }

    void Attach(void* ptr) noexcept
    {
        if (ptr != m_ptr)
        {
            Free();
            m_ptr = ptr;
        }
    }

    [[nodiscard]] void* Detach() noexcept
    {
        void* ptr = m_ptr;
        m_ptr = nullptr;
        return ptr;
    }

    void Free() noexcept
    {
        if (m_ptr != nullptr)
        {
            ::CoTaskMemFree(m_ptr);
            m_ptr = nullptr;
        }

    }

    ~CAutoCoTaskMemFree() noexcept
    {
        Free();
    }


    // Ban copy
private:
    CAutoCoTaskMemFree(CAutoCoTaskMemFree const&) = delete;
    CAutoCoTaskMemFree& operator=(CAutoCoTaskMemFree const&) = delete;


private:
    void* m_ptr;
};

} // anonymous namespace


//
// Process the data passed from the outside as a const pointer to our custom struct
//
extern "C" HRESULT __stdcall MyCppDll_ProcessData(const MyCppDll_Data* pData)
{
    //
    // Check the input pointer
    //

    if (pData == nullptr)
    {
        return E_POINTER;
    }


    //
    // Convert the input GUID to a string
    //

    LPOLESTR pGuidString = nullptr;
    HRESULT hr = ::StringFromCLSID(pData->Id, &pGuidString);
    if (FAILED(hr))
    {
        return hr;
    }

    CAutoCoTaskMemFree autoFreeGuidString(pGuidString);


    //
    // Format a message to be shown to the user, displaying the data passed as input
    //

    CString strName;
    if (pData->Name != nullptr)
    {
        strName.Format(L"'%s'", pData->Name);
    }
    else
    {
        strName = L"<nullptr>";
    }

    CString msg;
    msg.Format(
        L"C++ DLL received the following data:\n\n Id = %s\n Value = %d\n Name = %s\n",
        pGuidString,
        pData->Value,
        strName.GetString()
    );

    MessageBox(nullptr, msg, L"MyCppDll", MB_OK);

    return S_OK;
}


BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    UNREFERENCED_PARAMETER(hModule);
    UNREFERENCED_PARAMETER(lpReserved);

    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

