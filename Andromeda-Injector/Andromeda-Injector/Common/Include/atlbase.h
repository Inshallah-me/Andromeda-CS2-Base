#pragma once

// Minimal atlbase.h shim for CI builds that lack the ATL component.
// BlackBone's ImageNET.h / PDBHelper.h only need CComPtr to parse.
// The real ATL implementation ships prebuilt in BlackBone.lib, so this
// shim is only required at compile time and is never exercised at runtime.

#include <cstddef>

template <class T>
class CComPtr
{
public:
    CComPtr() noexcept : m_p(nullptr) {}
    CComPtr(std::nullptr_t) noexcept : m_p(nullptr) {}
    CComPtr(T* p) noexcept : m_p(p) { if (m_p) m_p->AddRef(); }
    CComPtr(const CComPtr& other) noexcept : m_p(other.m_p) { if (m_p) m_p->AddRef(); }
    CComPtr(CComPtr&& other) noexcept : m_p(other.m_p) { other.m_p = nullptr; }
    ~CComPtr() noexcept { Release(); }

    CComPtr& operator=(const CComPtr& other) noexcept
    {
        if (this != &other)
        {
            if (other.m_p) other.m_p->AddRef();
            Release();
            m_p = other.m_p;
        }
        return *this;
    }

    CComPtr& operator=(T* p) noexcept
    {
        if (m_p != p)
        {
            if (p) p->AddRef();
            Release();
            m_p = p;
        }
        return *this;
    }

    void Release() noexcept
    {
        if (m_p)
        {
            m_p->Release();
            m_p = nullptr;
        }
    }

    T* operator->() const noexcept { return m_p; }
    T** operator&() noexcept { return &m_p; }
    explicit operator bool() const noexcept { return m_p != nullptr; }
    T* get() const noexcept { return m_p; }

    T* m_p;
};
