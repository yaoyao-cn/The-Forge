#include <string>

#include "../Interfaces/IApp.h"
#include "../../Renderer/IRenderer.h"
#include "../Interfaces/IOperatingSystem.h"
#include "../Interfaces/ITime.h"
#include <rawfile/raw_file_manager.h>

/*****************************************************************************************/
// MARK: - Declare member 
/*****************************************************************************************/
static WindowDesc gWindow;
static bool windowReady = false;
static bool isActive = false;
static bool isLoaded = false;
static Timer g_deltaTimer;
static uint8_t gResetScenario = RESET_SCENARIO_NONE;

static CpuInfo gCpu;
extern bool gShowPlatformUI;
extern RendererApi gSelectedRendererApi;
extern IApp *pApp;
extern char *g_ePath;
extern char *g_iPath;
extern NativeResourceManager *g_assetManager;

extern uint32_t g_widthPixels;
extern uint32_t g_heightPixels;
extern float g_density;
extern uint32_t g_densityDpi;
extern float g_scaledDensity;
extern float g_xdpi;
extern float g_ydpi;

/************************************************************************/
// MARK: - Declare function
/************************************************************************/

void toggleFullscreen(WindowDesc *window)
{
	// No-op
}

void setWindowRect(WindowDesc *winDesc, const RectDesc *rect)
{
	// No-op
}
void toggleBorderless(WindowDesc *winDesc, unsigned width, unsigned height)
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
void showCursor(void)
{
	// No-op
}

void hideCursor(void)
{
	// No-op
}

bool isCursorInsideTrackingArea(void)
{
	return true;
}
void setResolution(const MonitorDesc *pMonitor, const Resolution *pRes)
{
	// No-op
}

void getRecommendedResolution(RectDesc *rect)
{
	*rect = {0, 0, ( int32_t ) g_widthPixels, ( int32_t ) g_heightPixels};
}

MonitorDesc *getMonitor(uint32_t index)
{
	return NULL;
}

uint32_t getMonitorCount(void)
{
	return 1;
}

#include <ace/xcomponent/native_interface_xcomponent.h>
/************************************************************************/
// MARK: - OHOS
/************************************************************************/

bool initBaseSubsystems()
{
	extern void platformInitWindowSystem(WindowDesc *);
	platformInitWindowSystem(&gWindow);
	pApp->pWindow = &gWindow;
	return true;
}

void updateBaseSubsystems(float deltaTime)
{
	extern void platformUpdateWindowSystem();
	platformUpdateWindowSystem();
}

void exitBaseSubsystems()
{
	extern void platformExitWindowSystem();
	platformExitWindowSystem();
}
namespace
{
	static bool initSystem()
	{
		initCpuInfo(&gCpu);
		initTimer(&g_deltaTimer);

		if (! initMemAlloc(pApp->GetName())) {
			LOGF(LogLevel::eERROR, " the-forge-app Error starting application");
		}

		struct FilePlatFormData
		{
			const char *ePath = g_ePath;
			const char *iPath = g_iPath;
			void *assetManager = g_assetManager;
		} platformData;

		FileSystemInitDesc fsDesc = {pApp->GetName(), &platformData};

		if (initFileSystem(&fsDesc)) {
			fsSetPathForResourceDir(pSystemFileIO, RM_DEBUG, RD_LOG, "");

			initLog(pApp->GetName(), DEFAULT_LOG_LEVEL);

			IApp::Settings *pSettings = &pApp->mSettings;
			RectDesc rect = {0, 0, ( int32_t ) g_widthPixels, ( int32_t ) g_heightPixels};
			pSettings->mWidth = getRectWidth(&rect);
			pSettings->mHeight = getRectHeight(&rect);

			return true;
		}

		LOGF(LogLevel::eERROR, " the-forge-app Error initFileSystemCB");
		return false;
	}

	static std::string GetComponentID(OH_NativeXComponent *nativeXComponent)
	{
		// 获取native对象实例ID
		if (nativeXComponent) {
			int32_t ret;
			char idStr [OH_XCOMPONENT_ID_LEN_MAX + 1] = {};
			uint64_t idSize = OH_XCOMPONENT_ID_LEN_MAX + 1;
			if (OH_NATIVEXCOMPONENT_RESULT_SUCCESS == OH_NativeXComponent_GetXComponentId(nativeXComponent, idStr, &idSize)) {
				return idStr;
			}
		}
		return {};
	}

	static void onFocusChanged(bool focused)
	{
		if (pApp == nullptr || ! pApp->mSettings.mInitialized) {
			return;
		}
		pApp->mSettings.mFocused = focused;
	}

	static void OnFrameCB(OH_NativeXComponent *component, uint64_t timestamp, uint64_t targetTimestamp)
	{
		std::string id = GetComponentID(component);
		if (id.compare("The-Forge") == 0) {
			if (isActive && gResetScenario != RESET_SCENARIO_NONE) {
				if (gResetScenario & RESET_SCENARIO_RELOAD) {
					pApp->Unload();

					if (! pApp->Load()) {
						abort();
					}

					gResetScenario &= ~RESET_SCENARIO_RELOAD;
					return;
				}

				pApp->Unload();
				pApp->Exit();

				exitBaseSubsystems();

				gSelectedRendererApi = ( RendererApi ) 0;
				pApp->mSettings.mInitialized = false;

				{
					if (! initBaseSubsystems()) {
						abort();
					}

					Timer t;
					initTimer(&t);
					if (! pApp->Init()) {
						abort();
					}
					pApp->mSettings.mInitialized = true;

					if (! pApp->Load()) {
						abort();
					}

					LOGF(LogLevel::eINFO, "Application Reset %f", getTimerMSec(&t, false) / 1000.0f);
				}

				gResetScenario = RESET_SCENARIO_NONE;
				return;
			}

			if (! windowReady || ! isActive) {
				pApp->mSettings.mQuit = true;

				if (isLoaded && ! windowReady) {
					pApp->Unload();
					isLoaded = false;
				}

				usleep(1);
				return;
			}

			float deltaTime = getTimerMSec(&g_deltaTimer, true) / 1000.0f;
			// if framerate appears to drop below about 6, assume we're at a breakpoint and simulate 20fps.
			if (deltaTime > 0.15f)
				deltaTime = 0.05f;

			// UPDATE BASE INTERFACES
			updateBaseSubsystems(deltaTime);

			// UPDATE APP
			pApp->Update(deltaTime);
			pApp->Draw();
		}
	}

	static void OnSurfaceCreatedCB(OH_NativeXComponent *component, void *window)
	{
		std::string id = GetComponentID(component);
		if (id.compare("The-Forge") == 0) {
			OH_NativeXComponent_RegisterOnFrameCallback(component, OnFrameCB);

			LOGF(LogLevel::eDEBUG, " the-forge-app init window");
			if (! initSystem()) {
				return;
			}

			if (! pApp->Init()) {
				return;
			}

			if (! initBaseSubsystems())
				return;

			IApp::Settings *pSettings = &pApp->mSettings;
			// TODO:openWindow need clear
			
			gWindow.handle.component = component;
			gWindow.windowedRect.left = 0;
			gWindow.windowedRect.top = 0;
			{
				uint64_t screenHeight;
				uint64_t screenWidth;
				if (OH_NATIVEXCOMPONENT_RESULT_SUCCESS != OH_NativeXComponent_GetXComponentSize(component, window, &screenWidth, &screenHeight)) {
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
		
			if (! windowReady) {
				pApp->Load();
				isLoaded = true;
			}

			// The window is being shown, mark it as ready.
			windowReady = true;
			isActive = true;
		}
	}

	void onRequestReload()
	{
		gResetScenario |= RESET_SCENARIO_RELOAD;
	}

	static void OnSurfaceChangedCB(OH_NativeXComponent *component, void *window)
	{
		std::string id = GetComponentID(component);
		if (id.compare("The-Forge") == 0) {

			uint64_t screenHeight;
			uint64_t screenWidth;
			if (OH_NATIVEXCOMPONENT_RESULT_SUCCESS != OH_NativeXComponent_GetXComponentSize(component, window, &screenWidth, &screenHeight)) {
				LOGF(LogLevel::eERROR, " the-forge-app GetXComponentSize failed");
				return;
			}

			IApp::Settings *pSettings = &pApp->mSettings;
			if (pSettings->mWidth != screenWidth || pSettings->mHeight != screenHeight) {
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
		if (id.compare("The-Forge") == 0) {
			LOGF(LogLevel::eERROR, " the-forge-app term window");

			if (isLoaded)
				pApp->Unload();
			windowReady = false;
			// The window is being hidden or closed, clean it up.


			pApp->Exit();

			exitLog();

			exitFileSystem();

			exitMemAlloc();
		}
	}

	static void OnFocusEventCB(OH_NativeXComponent *component, void *window)
	{
		std::string id = GetComponentID(component);
		if (id.compare("The-Forge") == 0) {

			isActive = true;
			onFocusChanged(true);

			LOGF(LogLevel::eERROR, " the-forge-app resume app");
		}
	}

	static void OnBlurEventCB(OH_NativeXComponent *component, void *window)
	{
		std::string id = GetComponentID(component);
		if (id.compare("The-Forge") == 0) {
			isActive = false;
			onFocusChanged(false);

			LOGF(LogLevel::eERROR, " the-forge-app pause app");
		}
	}

	static void DispatchTouchEventCB(OH_NativeXComponent *component, void *window)
	{
		std::string id = GetComponentID(component);
		if (id.compare("The-Forge") == 0) {
			isActive = true;
		}
	}

	static void DispatchMouseEventCB(OH_NativeXComponent *component, void *window)
	{
		std::string id = GetComponentID(component);
		if (id.compare("The-Forge") == 0) {
			isActive = true;
		}
	}

	static void DispatchHoverEvent(OH_NativeXComponent *component, bool isHover)
	{
		std::string id = GetComponentID(component);
		if (id.compare("The-Forge") == 0) {
		}
	}

	static void OnKeyEventCB(OH_NativeXComponent *component, void *window)
	{
		std::string id = GetComponentID(component);
		if (id.compare("The-Forge") == 0) {
		}
	}

} // namespace

// 指针结构体自己进行内存管理，操作系统只存了指针
static OH_NativeXComponent_MouseEvent_Callback g_mouseCallback = {DispatchMouseEventCB, DispatchHoverEvent};
static OH_NativeXComponent_Callback g_instanceCallback = {OnSurfaceCreatedCB, OnSurfaceChangedCB, OnSurfaceDestroyedCB, DispatchTouchEventCB};

static bool RegisterCallback(OH_NativeXComponent *nativeXComponent)
{
	if (nativeXComponent) {
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
	if ((env == nullptr) || (exports == nullptr)) {

		LOGF(LogLevel::eERROR, " the-forge-app env or exports is null");
		return false;
	}

	// 获取XComponent实例的property。
	napi_value exportInstance = nullptr;
	if (napi_ok != napi_get_named_property(env, exports, OH_NATIVE_XCOMPONENT_OBJ, &exportInstance)) {
		LOGF(LogLevel::eERROR, " the-forge-app napi_get_named_property ret error");
		return false;
	}
	// 获取作为调用目标的C++实例
	OH_NativeXComponent *nativeXComponent = nullptr;
	if (napi_ok != napi_unwrap(env, exportInstance, reinterpret_cast<void **>(&nativeXComponent))) {
		LOGF(LogLevel::eERROR, " the-forge-app napi_unwrap ret error");
		return false;
	}

	// 获取native对象实例ID
	std::string id = GetComponentID(nativeXComponent);
	if (id.compare("The-Forge") == 0) {
		RegisterCallback(nativeXComponent);
	}
	return true;
}