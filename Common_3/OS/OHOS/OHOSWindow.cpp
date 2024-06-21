#include <ctime>
#include <string>

#include "../Interfaces/IApp.h"
#include "../../Renderer/IRenderer.h"
#include "../Interfaces/IOperatingSystem.h"
#include "../Interfaces/ITime.h"
// NOTE: We have to define all functions so that their refernces in
// WindowSystem are properly resolved.

static uint8_t gResetScenario = RESET_SCENARIO_NONE;
/************************************************************************/
// MARK: - Define func
/************************************************************************/

void requestShutdown()
{
	LOGF(LogLevel::eERROR, "Cannot manually shutdown on HarmonyOS");
}

void onRequestReload()
{
	gResetScenario |= RESET_SCENARIO_RELOAD;
}

void onDeviceLost()
{
	// NOT SUPPORTED ON THIS PLATFORM
}

void onAPISwitch()
{
	gResetScenario |= RESET_SCENARIO_API_SWITCH;
}

void openWindow(const char *app_name, WindowDesc *winDesc)
{
	// No-op
}

void closeWindow(const WindowDesc *winDesc)
{
	// No-op
}

void setWindowRect(WindowDesc *winDesc, const RectDesc *rect)
{
	// No-op
}

void setWindowSize(WindowDesc *winDesc, unsigned width, unsigned height)
{
	// No-op
}

void toggleBorderless(WindowDesc *winDesc, unsigned width, unsigned height)
{
	// No-op
}

void toggleFullscreen(WindowDesc *window)
{
	// No-op
}

void showWindow(WindowDesc *winDesc)
{
	// No-op
}

void hideWindow(WindowDesc *winDesc)
{
	// No-op
}

void maximizeWindow(WindowDesc *winDesc)
{
	// No-op
}

void minimizeWindow(WindowDesc *winDesc)
{
	// No-op
}

void centerWindow(WindowDesc *winDesc)
{
	// No-op
}

/************************************************************************/
// MARK: -  CURSOR AND MOUSE HANDLING INTERFACE FUNCTIONS
/************************************************************************/

void *createCursor(const char *path)
{
	return NULL;
}

void setCursor(void *cursor)
{
	// No-op
}

void showCursor(void)
{
	// No-op
}

void hideCursor(void)
{
	// No-op
}

void captureCursor(WindowDesc *winDesc, bool bEnable)
{
	ASSERT(winDesc);

	if (winDesc->cursorCaptured != bEnable)
	{
		winDesc->cursorCaptured = bEnable;
	}
}

bool isCursorInsideTrackingArea(void)
{
	return true;
}

void setMousePositionRelative(const WindowDesc *winDesc, int32_t x, int32_t y)
{
	// No-op
}

void setMousePositionAbsolute(int32_t x, int32_t y)
{
	// No-op
}

void setResolution(const MonitorDesc *pMonitor, const Resolution *pRes)
{
	// No-op
}

bool getResolutionSupport(const MonitorDesc *pMonitor, const Resolution *pRes)
{
	return false;
}

/************************************************************************/
// MARK: - MONITOR AND RESOLUTION HANDLING INTERFACE FUNCTIONS
/************************************************************************/

MonitorDesc *getMonitor(uint32_t index)
{
	return NULL;
}

uint32_t getMonitorCount(void)
{
	return 1;
}

/************************************************************************/
// MARK: - STRUCTS
/************************************************************************/

struct DisplayMetrics
{
	uint32_t widthPixels;
	uint32_t heightPixels;
	float density;
	uint32_t densityDpi;
	float scaledDensity;
	float xdpi;
	float ydpi;
};

DisplayMetrics metrics = {};

void getDpiScale(float array[2])
{
	array[0] = metrics.scaledDensity;
	array[1] = metrics.scaledDensity;
}

void getRecommendedResolution(RectDesc *rect)
{
	*rect = {0, 0, (int32_t)metrics.widthPixels, (int32_t)metrics.heightPixels};
}

/*****************************************************************************************/
// MARK: - PLATFORM LAYER CORE SUBSYSTEMS
/*****************************************************************************************/
static bool gShowPlatformUI = true;
WindowDesc gWindow;
IApp *pWindowAppRef = NULL;
bool windowReady = false;
bool isActive = false;
bool isLoaded = false;
extern RendererApi gSelectedRendererApi;
static uint32_t gSelectedApiIndex = 0;
extern Timer gdeltaTimer;

static void onFocusChanged(bool focused)
{
	if (pWindowAppRef == nullptr || !pWindowAppRef->mSettings.mInitialized)
	{
		return;
	}

	pWindowAppRef->mSettings.mFocused = focused;
}

bool initBaseSubsystems()
{
	// Not exposed in the interface files / app layer
	extern bool platformInitFontSystem();
	extern bool platformInitUserInterface();
	extern void platformInitLuaScriptingSystem();
	extern void platformInitWindowSystem(WindowDesc *);

	platformInitWindowSystem(&gWindow);
	pWindowAppRef->pWindow = &gWindow;

#ifdef ENABLE_FORGE_FONTS
	if (!platformInitFontSystem())
		return false;
#endif

#ifdef ENABLE_FORGE_UI
	if (!platformInitUserInterface())
		return false;
#endif

#ifdef ENABLE_FORGE_SCRIPTING
	platformInitLuaScriptingSystem();
#endif

	return true;
}

void updateBaseSubsystems(float deltaTime)
{
	// Not exposed in the interface files / app layer
	extern void platformUpdateLuaScriptingSystem();
	extern void platformUpdateUserInterface(float deltaTime);
	extern void platformUpdateWindowSystem();

	platformUpdateWindowSystem();

#ifdef ENABLE_FORGE_SCRIPTING
	platformUpdateLuaScriptingSystem();
#endif

#ifdef ENABLE_FORGE_UI
	platformUpdateUserInterface(deltaTime);
#endif
}

void exitBaseSubsystems()
{
	// Not exposed in the interface files / app layer
	extern void platformExitFontSystem();
	extern void platformExitUserInterface();
	extern void platformExitLuaScriptingSystem();
	extern void platformExitWindowSystem();

	platformExitWindowSystem();

#ifdef ENABLE_FORGE_UI
	platformExitUserInterface();
#endif

#ifdef ENABLE_FORGE_FONTS
	platformExitFontSystem();
#endif

#ifdef ENABLE_FORGE_SCRIPTING
	platformExitLuaScriptingSystem();
#endif
}

//------------------------------------------------------------------------
// MARK: - PLATFORM LAYER USER INTERFACE
//------------------------------------------------------------------------

void setupPlatformUI(int32_t width, int32_t height)
{
#ifdef ENABLE_FORGE_UI
	// WINDOW AND RESOLUTION CONTROL

	extern void platformSetupWindowSystemUI(IApp *);
	platformSetupWindowSystemUI(pApp);

	// VSYNC CONTROL

	UIComponentDesc UIComponentDesc = {};
	UIComponentDesc.mStartPosition = vec2(width * 0.4f, height * 0.90f);
	uiCreateComponent("VSync Control", &UIComponentDesc, &pToggleVSyncWindow);

	CheckboxWidget checkbox;
	checkbox.pData = &pApp->mSettings.mVSyncEnabled;
	UIWidget *pCheckbox = uiCreateComponentWidget(pToggleVSyncWindow, "Toggle VSync\t\t\t\t\t", &checkbox, WIDGET_TYPE_CHECKBOX);
	REGISTER_LUA_WIDGET(pCheckbox);

	gSelectedApiIndex = gSelectedRendererApi;

	static const char *pApiNames[] =
		{
#if defined(GLES)
			"GLES",
#endif
#if defined(VULKAN)
			"Vulkan",
#endif
		};
	uint32_t apiNameCount = sizeof(pApiNames) / sizeof(*pApiNames);

	if (apiNameCount > 1 && !gGLESUnsupported)
	{
		UIComponentDesc.mStartPosition = vec2(width * 0.4f, height * 0.01f);
		uiCreateComponent("API Switching", &UIComponentDesc, &pAPISwitchingWindow);

		// Select Api
		DropdownWidget selectApUIWidget;
		selectApUIWidget.pData = &gSelectedApiIndex;
		for (uint32_t i = 0; i < RENDERER_API_COUNT; ++i)
		{
			selectApUIWidget.mNames.push_back((char *)pApiNames[i]);
			selectApUIWidget.mValues.push_back(i);
		}
		pSelectApUIWidget = uiCreateComponentWidget(pAPISwitchingWindow, "Select API", &selectApUIWidget, WIDGET_TYPE_DROPDOWN);
		pSelectApUIWidget->pOnEdited = onAPISwitch;

#ifdef ENABLE_FORGE_SCRIPTING
		REGISTER_LUA_WIDGET(pSelectApUIWidget);
		LuaScriptDesc apiScriptDesc = {};
		apiScriptDesc.pScriptFileName = "Test_API_Switching.lua";
		luaDefineScripts(&apiScriptDesc, 1);
#endif
	}
#endif
}

void togglePlatformUI()
{
	gShowPlatformUI = pWindowAppRef->mSettings.mShowPlatformUI;

#ifdef ENABLE_FORGE_UI
	extern void platformToggleWindowSystemUI(bool);
	platformToggleWindowSystemUI(gShowPlatformUI);

	uiSetComponentActive(pToggleVSyncWindow, gShowPlatformUI);
	uiSetComponentActive(pAPISwitchingWindow, gShowPlatformUI);
#endif
}

#include <napi/native_api.h>
#include <ace/xcomponent/native_interface_xcomponent.h>
/************************************************************************/
// MARK: - OHOS
/************************************************************************/

void getDisplayMetrics(napi_env env, napi_callback_info info)
{
	// 获取arkts侧的系统库路径
	char path[64] = "@ohos.display";
	size_t typeLen = 0;
	napi_value string;
	napi_create_string_utf8(env, path, typeLen, &string);
	// 加载系统库
	napi_value sysModule;
	napi_load_module(env, path, &sysModule);
	// 获取系统库中的"getDefaultDisplaySync"方法
	napi_value func = nullptr;
	napi_get_named_property(env, sysModule, "getDefaultDisplaySync", &func);
	napi_value funcResult;
	napi_call_function(env, sysModule, func, 0, nullptr, &funcResult);

	auto Fn_GetValueByName = [&](const char *name)
	{
		napi_value naV = nullptr;
		napi_get_named_property(env, funcResult, name, &naV);
		double v;
		napi_get_value_double(env, naV, &v);
		return v;
	};
	metrics.widthPixels = Fn_GetValueByName("width");
	metrics.heightPixels = Fn_GetValueByName("height");
	metrics.density = Fn_GetValueByName("densityPixels");
	metrics.densityDpi = Fn_GetValueByName("densityDPI");
	metrics.scaledDensity = Fn_GetValueByName("scaledDensity");
	metrics.xdpi = Fn_GetValueByName("xDPI");
	metrics.ydpi = Fn_GetValueByName("yDPI");
}

static std::string GetComponentID(OH_NativeXComponent *nativeXComponent)
{
	// 获取native对象实例ID
	if (nativeXComponent)
	{
		int32_t ret;
		char idStr[OH_XCOMPONENT_ID_LEN_MAX + 1] = {};
		uint64_t idSize = OH_XCOMPONENT_ID_LEN_MAX + 1;
		if (OH_NATIVEXCOMPONENT_RESULT_SUCCESS ==
			OH_NativeXComponent_GetXComponentId(nativeXComponent, idStr, &idSize))
		{
			return idStr;
		}
	}
	return {};
}

static void OnFrameCB(OH_NativeXComponent *component, uint64_t timestamp, uint64_t targetTimestamp)
{
	std::string id = GetComponentID(component);
	if (id.compare("The-Forge") == 0)
	{
		if (isActive && gResetScenario != RESET_SCENARIO_NONE)
		{
			if (gResetScenario & RESET_SCENARIO_RELOAD)
			{
				pWindowAppRef->Unload();

				if (!pWindowAppRef->Load())
				{
					abort();
				}

				gResetScenario &= ~RESET_SCENARIO_RELOAD;
				return;
			}

			pWindowAppRef->Unload();
			pWindowAppRef->Exit();

			exitBaseSubsystems();

			gSelectedRendererApi = (RendererApi)gSelectedApiIndex;
			pWindowAppRef->mSettings.mInitialized = false;

			{
				if (!initBaseSubsystems())
				{
					abort();
				}

				Timer t;
				initTimer(&t);
				if (!pWindowAppRef->Init())
				{
					abort();
				}

				setupPlatformUI(pWindowAppRef->mSettings.mWidth, pWindowAppRef->mSettings.mHeight);
				pWindowAppRef->mSettings.mInitialized = true;

				if (!pWindowAppRef->Load())
				{
					abort();
				}

				LOGF(LogLevel::eINFO, "Application Reset %f", getTimerMSec(&t, false) / 1000.0f);
			}

			gResetScenario = RESET_SCENARIO_NONE;
			return;
		}

		if (!windowReady || !isActive)
		{
			pWindowAppRef->mSettings.mQuit = true;

			if (isLoaded && !windowReady)
			{
				pWindowAppRef->Unload();
				isLoaded = false;
			}

			usleep(1);
			return;
		}

		float deltaTime = getTimerMSec(&gdeltaTimer, true) / 1000.0f;
		// if framerate appears to drop below about 6, assume we're at a breakpoint and simulate 20fps.
		if (deltaTime > 0.15f)
			deltaTime = 0.05f;

		// UPDATE BASE INTERFACES
		updateBaseSubsystems(deltaTime);

		// UPDATE APP
		pWindowAppRef->Update(deltaTime);
		pWindowAppRef->Draw();

		if (gShowPlatformUI != pWindowAppRef->mSettings.mShowPlatformUI)
		{
			togglePlatformUI();
		}
	}
}
static void OnSurfaceCreatedCB(OH_NativeXComponent *component, void *window)
{
	std::string id = GetComponentID(component);
	if (id.compare("The-Forge") == 0)
	{
		OH_NativeXComponent_RegisterOnFrameCallback(component, OnFrameCB);

		LOGF(LogLevel::eDEBUG, " the-forge-app init window");


		if (!pWindowAppRef->Init())
		{
			return;
		}

		if (!initBaseSubsystems())
			return;

		IApp::Settings *pSettings = &pWindowAppRef->mSettings;
		gWindow.handle.component = component;
		gWindow.windowedRect.left = 0;
		gWindow.windowedRect.top = 0;
		{
			uint64_t screenHeight;
			uint64_t screenWidth;
			if (OH_NATIVEXCOMPONENT_RESULT_SUCCESS !=
				OH_NativeXComponent_GetXComponentSize(component, window, &screenWidth,
													  &screenHeight))
			{
				LOGF(LogLevel::eERROR, " the-forge-app GetXComponentSize failed");
				return;
			}

			gWindow.windowedRect.right = screenWidth;
			gWindow.windowedRect.bottom = screenHeight;
			pSettings->mWidth = screenWidth;
			pSettings->mHeight = screenHeight;
		}
		gWindow.fullScreen = pSettings->mFullScreen;
		gWindow.maximized = false;
		gWindow.cursorCaptured = false;
		gWindow.handle.window = window;
		openWindow(pWindowAppRef->GetName(), &gWindow);

		if (!windowReady)
		{
			pWindowAppRef->Load();
			isLoaded = true;
		}

		// The window is being shown, mark it as ready.
		windowReady = true;
			isActive = true;
	}
}

static void OnSurfaceChangedCB(OH_NativeXComponent *component, void *window)
{
	std::string id = GetComponentID(component);
	if (id.compare("The-Forge") == 0)
	{

		uint64_t screenHeight;
		uint64_t screenWidth;
		if (OH_NATIVEXCOMPONENT_RESULT_SUCCESS !=
			OH_NativeXComponent_GetXComponentSize(component, window, &screenWidth,
												  &screenHeight))
		{
			LOGF(LogLevel::eERROR, " the-forge-app GetXComponentSize failed");
			return;
		}

		IApp::Settings *pSettings = &pWindowAppRef->mSettings;
		if (pSettings->mWidth != screenWidth || pSettings->mHeight != screenHeight)
		{
			gWindow.windowedRect.left = 0;
			gWindow.windowedRect.top = 0;
			gWindow.windowedRect.right = screenWidth;
			gWindow.windowedRect.bottom = screenHeight;

			pSettings->mWidth = screenWidth;
			pSettings->mHeight = screenHeight;

			onRequestReload();
		}
	}
}

static void OnSurfaceDestroyedCB(OH_NativeXComponent *component, void *window)
{
	std::string id = GetComponentID(component);
	if (id.compare("The-Forge") == 0)
	{
		LOGF(LogLevel::eERROR, " the-forge-app term window");

		if (isLoaded)
			pWindowAppRef->Unload();
		windowReady = false;
		// The window is being hidden or closed, clean it up.
	}
}

static void OnFocusEventCB(OH_NativeXComponent *component, void *window)
{
	std::string id = GetComponentID(component);
	if (id.compare("The-Forge") == 0)
	{

		isActive = true;
		onFocusChanged(true);

		LOGF(LogLevel::eERROR, " the-forge-app resume app");
	}
}

static void OnBlurEventCB(OH_NativeXComponent *component, void *window)
{
	std::string id = GetComponentID(component);
	if (id.compare("The-Forge") == 0)
	{
		isActive = false;
		onFocusChanged(false);

		LOGF(LogLevel::eERROR, " the-forge-app pause app");
	}
}

static void DispatchTouchEventCB(OH_NativeXComponent *component, void *window)
{
	std::string id = GetComponentID(component);
	if (id.compare("The-Forge") == 0)
	{
		isActive = true;
	}
}

static void DispatchMouseEventCB(OH_NativeXComponent *component, void *window)
{
	std::string id = GetComponentID(component);
	if (id.compare("The-Forge") == 0)
	{
		isActive = true;
	}
}

static void DispatchHoverEvent(OH_NativeXComponent *component, bool isHover)
{
	std::string id = GetComponentID(component);
	if (id.compare("The-Forge") == 0)
	{
	}
}

static void OnKeyEventCB(OH_NativeXComponent *component, void *window)
{
	std::string id = GetComponentID(component);
	if (id.compare("The-Forge") == 0)
	{
	}
}

// 指针结构体自己进行内存管理，操作系统只存了指针
static OH_NativeXComponent_MouseEvent_Callback g_mouseCallback = {DispatchMouseEventCB,
																  DispatchHoverEvent};
static OH_NativeXComponent_Callback g_instanceCallback = {
	OnSurfaceCreatedCB, OnSurfaceChangedCB, OnSurfaceDestroyedCB, DispatchTouchEventCB};

static bool RegisterCallback(OH_NativeXComponent *nativeXComponent)
{
	if (nativeXComponent)
	{
		OH_NativeXComponent_RegisterCallback(nativeXComponent, &g_instanceCallback);
		OH_NativeXComponent_RegisterMouseEventCallback(nativeXComponent, &g_mouseCallback);
		// 获得焦点
		OH_NativeXComponent_RegisterFocusEventCallback(nativeXComponent, OnFocusEventCB);
		// 失去焦点
		OH_NativeXComponent_RegisterBlurEventCallback(nativeXComponent, OnBlurEventCB);
		OH_NativeXComponent_RegisterKeyEventCallback(nativeXComponent, OnKeyEventCB);
		return true;
	}
	return false;
}

bool RegisterWindow(napi_env env, napi_value exports)
{
	if ((env == nullptr) || (exports == nullptr))
	{

		LOGF(LogLevel::eERROR, " the-forge-app env or exports is null");
		return false;
	}

	// 获取XComponent实例的property。
	napi_value exportInstance = nullptr;
	if (napi_ok !=
		napi_get_named_property(env, exports, OH_NATIVE_XCOMPONENT_OBJ, &exportInstance))
	{
		LOGF(LogLevel::eERROR, " the-forge-app napi_get_named_property ret error");
		return false;
	}
	// 获取作为调用目标的C++实例
	OH_NativeXComponent *nativeXComponent = nullptr;
	if (napi_ok !=
		napi_unwrap(env, exportInstance, reinterpret_cast<void **>(&nativeXComponent)))
	{
		LOGF(LogLevel::eERROR, " the-forge-app napi_unwrap ret error");
		return false;
	}

	// 获取native对象实例ID
	std::string id = GetComponentID(nativeXComponent);
	if (id.compare("The-Forge") == 0)
	{
		RegisterCallback(nativeXComponent);
	}
	return true;
}