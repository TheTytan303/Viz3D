#pragma once
#define GFX_THROW_FAILED(hrcall) if(FAILED(hr=(hrcall))) throw Graphics::HrException(__LINE__,__FILE__,hr)
#define GFX_DEVICE_REMOVED_EXCEPT(hr) Graphics::DeviceRemovedException(__LINE__,__FILE__,(hr))
//#define COUT_HRFAIL(hrcall) if(FAILED(hr=(hrcall))) std::count<<__LINE__<<" | " <<__FILE__<<std::endl;
#define GFX_EXCEPT(hr) Graphics::HrException(__LINE__, __FILE__, (hr))
#define GFX_THROW_NOINFO(hrcall) if( FAILED( hr = (hrcall) ) ) throw Graphics::HrException( __LINE__,__FILE__,hr )
//#define GFX_THROW_NOINFO(hrcall) if(FAILED (hr = (hrcall)) htrow Graphic

//#define GFX_EXCEPT(ht) Graphics::HrException(__LINE__, __FILE__, (hr),infoManager.GetMessages() )
#define GFX_THROW_INFO(hrcall) infoManager.Set(); if (FAILED(hr = (hrcall))) throw GFX_EXCEPT(hr)
//#define GFX_THROW_INFO_ONLY(call) infoManager.Set(); (call); {auto v = infoManager.GetMessages(); if(!v.empty()) {throw Graphics::InfoException( __LINE__,__FILE__,v);}}


//error exception helper macro
#define CWND_EXCEPT( hr ) Window::Exception( __LINE__,__FILE__,hr )
#define CWND_LAST_EXCEPT() Window::Exception( __LINE__,__FILE__,GetLastError() )

#ifdef NDEBUG
#define INFOMAN(gfx) HRESULT hr
#else
#define INFOMAN(gfx) HRESULT hr; DxgiInfoManager& infoManager = GetInfoManager((gfx))
#endif