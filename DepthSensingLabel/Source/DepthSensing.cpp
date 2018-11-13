#include "stdafx.h"
#include <algorithm>
#include <iomanip>
#include <utility>
#include <io.h>
#include "DepthSensing.h"

// Supervoxels--------------------------------------------------------------------------
#ifdef V
#undef V

#include "sparseGrid3d.h"

#include <pcl\console\parse.h>
#include <pcl\point_cloud.h>
#include <pcl\point_types.h>
#include <pcl\io\pcd_io.h>
#include <pcl\common\transforms.h>
#include <pcl\visualization\pcl_visualizer.h>
#include <pcl\segmentation\supervoxel_clustering.h>

typedef pcl::PointXYZRGBA          PointT;
typedef pcl::PointCloud<PointT>    PointCloudT;
typedef pcl::PointNormal           PointNT;
typedef pcl::PointCloud<PointNT>   PointNCloudT;
typedef pcl::PointXYZL             PointLT;
typedef pcl::PointCloud<PointLT>   PointLCloudT;

#endif
// Supervoxels--------------------------------------------------------------------------

// Graph Cut----------------------------------------------------------------------------
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <time.h>
#include "GCoptimization.h"
// Graph Cut----------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
// Global variables
//--------------------------------------------------------------------------------------
CDXUTDialogResourceManager	g_DialogResourceManager; // manager for shared resources of dialogs
CD3DSettingsDlg             g_D3DSettingsDlg;        // Device settings dialog
CDXUTDialog                 g_HUD;                   // manages the 3D   
CDXUTDialog                 g_SampleUI;              // dialog for sample specific controls

CDXUTTextHelper*            g_pTxtHelper = NULL;

CModelViewerCamera          g_Camera;                // A model viewing camera
DX11SceneRepHashSDF			g_SceneRepSDFLocal;
DX11SceneRepHashSDF			g_SceneRepSDFGlobal;	 // we had the idea once to use two hashes on the GPU, one inside, and one outside of the frustm; but not used atm
DX11SceneRepChunkGrid		g_SceneRepChunkGrid;
TrajectoryLogReader			g_TrajectoryLogReader;
DX11Sensor					g_Sensor;
bool						g_bRenderHelp = true;

std::vector<mat4f>          g_FrameMat4f;

//--------------------------------------------------------------------------------------
// Entry point to the program. Initializes everything and goes into a message processing 
// loop. Idle time is used to render the scene.
//--------------------------------------------------------------------------------------
int main(int argc, char** argv) 
{
	// Enable run-time memory check for debug builds.
#if defined(DEBUG) | defined(_DEBUG)
	_CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
#endif

	try 
	{
		StdOutputLogger::start(StdOutputLogger::LOGDEVICE_CONSOLE, StdOutputLogger::LOGDEVICE_CONSOLE);

		std::string fileNameDescGlobalApp;
		std::string fileNameDescGlobalTracking;

		std::cout << "usage: DepthSensing [fileNameDescGlobalApp] [fileNameDescGlobalTracking]" << std::endl;
		fileNameDescGlobalApp = "zParametersDefault.txt";
		fileNameDescGlobalTracking = "zParametersTrackingDefault.txt";
		std::cout << VAR_NAME(fileNameDescGlobalApp) << " = " << fileNameDescGlobalApp << std::endl;
		std::cout << VAR_NAME(fileNameDescGlobalTracking) << " = " << fileNameDescGlobalTracking << std::endl;
		
		//Read the global app state
		ParameterFile parameterFileGlobalApp(fileNameDescGlobalApp);
		GlobalAppState::getInstance().readMembers(parameterFileGlobalApp);

		GlobalAppState::getInstance().s_ImageReaderSensorSourcePath = std::string("D:/Chair/") + std::string(argv[1]) + std::string("/");
		GlobalAppState::getInstance().s_ImageReaderSensorNumFrames = atoi(argv[2]);

		//Read the global camera tracking state
		ParameterFile parameterFileGlobalTracking(fileNameDescGlobalTracking);
		GlobalCameraTrackingState::getInstance().readMembers(parameterFileGlobalTracking);

		// Set DXUT callbacks
		DXUTSetCallbackDeviceChanging( ModifyDeviceSettings );
		DXUTSetCallbackMsgProc( MsgProc );
		DXUTSetCallbackKeyboard( OnKeyboard );
		DXUTSetCallbackFrameMove( OnFrameMove );

		DXUTSetCallbackD3D11DeviceAcceptable( IsD3D11DeviceAcceptable );
		DXUTSetCallbackD3D11DeviceCreated( OnD3D11CreateDevice );
		DXUTSetCallbackD3D11SwapChainResized( OnD3D11ResizedSwapChain );
		DXUTSetCallbackD3D11FrameRender( OnD3D11FrameRender );
		DXUTSetCallbackD3D11SwapChainReleasing( OnD3D11ReleasingSwapChain );
		DXUTSetCallbackD3D11DeviceDestroyed( OnD3D11DestroyDevice );

		InitApp();

		DXUTInit( true, true );					// Parse the command line, show msgboxes on error, and an extra cmd line param to force REF for now
		DXUTSetCursorSettings( true, true );	// Show the cursor and clip it when in full screen
		DXUTCreateWindow( L"DepthSensing", false );

		//TODO MADDI FIGURE OUT WHEN THAT IS ACTUALLY REQUIRED
		//if(GlobalAppState::getInstance().s_sensorIdx == 0 || GlobalAppState::getInstance().s_sensorIdx == 3 || GlobalAppState::getInstance().s_sensorIdx == 7)	//this is just a weird thing...
		//if (GlobalAppState::getInstance().s_sensorIdx == 0)
		//{
		//	DXUTSetIsInGammaCorrectMode(false); // Gamma fix for Kinect 4 windows
		//}
		//else 
		//{
		//	DXUTSetIsInGammaCorrectMode(true);
		//}

		DXUTSetIsInGammaCorrectMode(false); // Gamma fix for Kinect 4 windows

		DXUTCreateDevice( D3D_FEATURE_LEVEL_11_0,  true, GlobalAppState::getInstance().s_outputWindowWidth, GlobalAppState::getInstance().s_outputWindowHeight);
		DXUTMainLoop(); // Enter into the DXUT render loop

		StdOutputLogger::stop();
	}
	catch(const std::exception& e)
	{
		MessageBoxA(NULL, e.what(), "Exception caught", MB_ICONERROR);
		exit(EXIT_FAILURE);
	}
	catch(...)
	{
		MessageBoxA(NULL, "UNKNOWN EXCEPTION", "Exception caught", MB_ICONERROR);
		exit(EXIT_FAILURE);
	}
	return DXUTGetExitCode();
}

//--------------------------------------------------------------------------------------
// Initialize the app 
//--------------------------------------------------------------------------------------
void InitApp()
{	
	// Initialize dialogs
	g_D3DSettingsDlg.Init( &g_DialogResourceManager );
	g_HUD.Init( &g_DialogResourceManager );
	g_SampleUI.Init( &g_DialogResourceManager );

	g_HUD.SetCallback( OnGUIEvent ); int iY = 20;
	g_HUD.AddButton( IDC_TOGGLEFULLSCREEN, L"Toggle full screen", 0, iY, 170, 22 );
	g_HUD.AddButton( IDC_TOGGLEREF, L"Toggle REF (F3)", 0, iY += 26, 170, 22, VK_F3 );
	g_HUD.AddButton( IDC_CHANGEDEVICE, L"Change device (F2)", 0, iY += 26, 170, 22, VK_F2 );

	g_SampleUI.SetCallback( OnGUIEvent ); iY = 10;

	D3DXVECTOR3 camPos(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 lookAt(0.0, 0.0f, 1.0f);

	g_Camera.SetProjParams(D3DX_PI/4.0f, 1.3, 0.3, 5.0f);
	g_Camera.SetViewParams(&camPos, &lookAt);

	TimingLog::resetTimings();
}

//--------------------------------------------------------------------------------------
// Called right before creating a D3D9 or D3D10 device, allowing the app to modify the device settings as needed
//--------------------------------------------------------------------------------------
bool CALLBACK ModifyDeviceSettings( DXUTDeviceSettings* pDeviceSettings, void* pUserContext )
{
	// For the first device created if its a REF device, optionally display a warning dialog box
	static bool s_bFirstTime = true;
	if ( s_bFirstTime )
	{
		s_bFirstTime = false;
		if( ( DXUT_D3D9_DEVICE == pDeviceSettings->ver && pDeviceSettings->d3d9.DeviceType == D3DDEVTYPE_REF ) ||
			( DXUT_D3D11_DEVICE == pDeviceSettings->ver && pDeviceSettings->d3d11.DriverType == D3D_DRIVER_TYPE_REFERENCE ) )
		{
			DXUTDisplaySwitchingToREFWarning( pDeviceSettings->ver );
		}
	}

	return true;
}

//--------------------------------------------------------------------------------------
// Handle updates to the scene
//--------------------------------------------------------------------------------------
void CALLBACK OnFrameMove( double fTime, float fElapsedTime, void* pUserContext )
{
	// Update the camera's position based on user input 
	g_Camera.FrameMove( fElapsedTime );
}

//--------------------------------------------------------------------------------------
// Render the help and statistics text
//--------------------------------------------------------------------------------------
void RenderText()
{
	g_pTxtHelper->Begin();
	g_pTxtHelper->SetInsertionPos( 2, 0 );
	g_pTxtHelper->SetForegroundColor( D3DXCOLOR( 1.0f, 1.0f, 0.0f, 1.0f ) );
	g_pTxtHelper->DrawTextLine( DXUTGetFrameStats( DXUTIsVsyncEnabled() ) );
	g_pTxtHelper->DrawTextLine( DXUTGetDeviceStats() );
	
	if (!g_bRenderHelp) 
	{
		g_pTxtHelper->SetForegroundColor( D3DXCOLOR( 1.0f, 1.0f, 1.0f, 1.0f ) );
		g_pTxtHelper->DrawTextLine(L"\tPress F1 for help");
	}
	g_pTxtHelper->End();

	if (g_bRenderHelp) 
	{
		RenderHelp();
	}
}

void RenderHelp() 
{
	g_pTxtHelper->Begin();
	g_pTxtHelper->SetInsertionPos( 2, 40 );
	g_pTxtHelper->SetForegroundColor( D3DXCOLOR( 1.0f, 0.0f, 0.0f, 1.0f ) );
	g_pTxtHelper->DrawTextLine( L"Controls " );
	g_pTxtHelper->DrawTextLine(L"  \tF1:\t Hide help");
	g_pTxtHelper->DrawTextLine(L"  \tF2:\t Screenshot");
	g_pTxtHelper->DrawTextLine(L"  \t'R':\t Reset scan");
	g_pTxtHelper->DrawTextLine(L"  \t'9':\t Extract geometry (Marching Cubes)");
	g_pTxtHelper->DrawTextLine(L"  \t'<tab>':\t Switch to free-view mode");
	g_pTxtHelper->DrawTextLine(L"  \t'1':\t Visualize input depth");
	g_pTxtHelper->DrawTextLine(L"  \t'2':\t Visualize input normals");
	g_pTxtHelper->DrawTextLine(L"  \t'3':\t Visualize reconstruction (default)");
	g_pTxtHelper->DrawTextLine(L"  \t'4':\t Visualize input color");
	g_pTxtHelper->DrawTextLine(L"  \t'5':\t Visualize phong shaded");
	g_pTxtHelper->DrawTextLine(L"  \t'6':\t Pause/continue the application");
	g_pTxtHelper->DrawTextLine(L"  \t'7':\t GPU hash statistics");
	g_pTxtHelper->DrawTextLine(L"  \t'8':\t Visualize input label");
	g_pTxtHelper->DrawTextLine(L"  \t'9':\t Visualize phong shaded label");
	
	WCHAR frameIndex[10];
	swprintf_s(frameIndex, L"%d", g_Sensor.GetFrameNumberDepth());
	WCHAR textShow[20] = L"\n  \tframe:";

	wcscat(textShow, frameIndex);
	g_pTxtHelper->DrawTextLine(textShow);

	g_pTxtHelper->End();
}

//--------------------------------------------------------------------------------------
// Handle messages to the application
//--------------------------------------------------------------------------------------
LRESULT CALLBACK MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, bool* pbNoFurtherProcessing, void* pUserContext )
{
	// Pass messages to dialog resource manager calls so GUI state is updated correctly
	*pbNoFurtherProcessing = g_DialogResourceManager.MsgProc( hWnd, uMsg, wParam, lParam );
	if ( *pbNoFurtherProcessing )
		return 0;

	// Pass messages to settings dialog if its active
	if ( g_D3DSettingsDlg.IsActive() )
	{
		g_D3DSettingsDlg.MsgProc( hWnd, uMsg, wParam, lParam );
		return 0;
	}

	// Give the dialogs a chance to handle the message first
	*pbNoFurtherProcessing = g_HUD.MsgProc( hWnd, uMsg, wParam, lParam );
	if ( *pbNoFurtherProcessing )
		return 0;
	*pbNoFurtherProcessing = g_SampleUI.MsgProc( hWnd, uMsg, wParam, lParam );
	if ( *pbNoFurtherProcessing )
		return 0;

	// Pass all remaining windows messages to camera so it can respond to user input
	g_Camera.HandleMessages( hWnd, uMsg, wParam, lParam );

	return 0;
}

//--------------------------------------------------------------------------------------
// Handle key presses
//--------------------------------------------------------------------------------------
void StopScanningAndExtractIsoSurfaceMC(std::string plyName)
{
	g_SceneRepChunkGrid.stopMultiThreading();

	Timer t;

	vec4f posWorld = g_SceneRepSDFLocal.GetLastRigidTransform()*GlobalAppState::getInstance().s_StreamingPos; // trans lags one frame
	vec3f posCamera(posWorld.x, posWorld.y, posWorld.z);

	mat4f rigidTransform = g_SceneRepSDFLocal.GetLastRigidTransform();
	//DX11MarchingCubesChunkGrid::extractIsoSurface(DXUTGetD3D11DeviceContext(), g_SceneRepChunkGrid, g_SceneRepSDFLocal, posCamera, GlobalAppState::getInstance().s_StreamingRadius, GlobalAppState::getInstance().s_ImageReaderSensorSourcePath + plyName, &rigidTransform);
	DX11MarchingCubesChunkGrid::extractIsoSurface(DXUTGetD3D11DeviceContext(), g_SceneRepChunkGrid, g_SceneRepSDFLocal, posCamera, GlobalAppState::getInstance().s_StreamingRadius, GlobalAppState::getInstance().s_ImageReaderSensorSourcePath + plyName, NULL);

	std::cout << "Mesh Processing Time " << t.getElapsedTime() << " seconds" << std::endl;

	g_SceneRepChunkGrid.startMultiThreading(DXUTGetD3D11DeviceContext());
}

/*
void StopScanningAndDumpVoxelHash() 
{
	g_SceneRepChunkGrid.stopMultiThreading();

	Timer t;
	std::cout << "dumping (local) voxel grid... ";

	vec4f posWorld = g_SceneRepSDFLocal.GetLastRigidTransform()*GlobalAppState::getInstance().s_StreamingPos; // trans lags one frame
	vec3f p(posWorld.x, posWorld.y, posWorld.z);

	g_SceneRepChunkGrid.DumpVoxelHash(DXUTGetD3D11DeviceContext(), GlobalAppState::getInstance().s_DumpVoxelGridFile + ".dump", g_SceneRepSDFLocal, p, GlobalAppState::getInstance().s_StreamingRadius);
	std::cout << "done!" << std::endl;

	std::cout << "Dump Voxel Hash Processing Time " << t.getElapsedTime() << " seconds" << std::endl;

	g_SceneRepChunkGrid.startMultiThreading(DXUTGetD3D11DeviceContext());
}
*/

void CALLBACK OnKeyboard( UINT nChar, bool bKeyDown, bool bAltDown, void* pUserContext )
{
	static int whichShot = 0;
	GlobalAppState::getInstance().s_bRenderModeChanged = true;

	if ( bKeyDown ) 
	{
		wchar_t sz[200];

		switch( nChar )
		{
		case VK_F1:
			g_bRenderHelp = !g_bRenderHelp;
			break;
		case VK_F2:
			swprintf_s(sz, 200, L"screenshot%d.bmp", whichShot++);
			DXUTSnapD3D11Screenshot(sz, D3DX11_IFF_BMP);

			//swprintf_s(sz, 200, L"depth%d.bmp", whichShot);
			//D3DX11SaveTextureToFileW(DXUTGetD3D11DeviceContext(), g_KinectSensor.GetHSVDepthFloatTexture(), D3DX11_IFF_BMP, sz); 
			//swprintf_s(sz, 200, L"color%d.bmp", whichShot++);
			//D3DX11SaveTextureToFileW(DXUTGetD3D11DeviceContext(), g_KinectSensor.GetColorTexture(), D3DX11_IFF_BMP, sz);

			break;
		case '\t':
			GlobalAppState::getInstance().s_RenderMode = (GlobalAppState::getInstance().s_RenderMode + 1) % 2;
			g_Camera.Reset();
			break;
		case 'B':
			GlobalAppState::getInstance().s_timingsStepsEnabled = !GlobalAppState::getInstance().s_timingsStepsEnabled;
			break;
		case 'N':
			GlobalAppState::getInstance().s_timingsTotalEnabled = !GlobalAppState::getInstance().s_timingsTotalEnabled;
			break;
		case 'K': 
			g_SceneRepChunkGrid.printStatistics();
			break;
		case 'L':
			GlobalAppState::getInstance().s_bEnableGlobalLocalStreaming = !GlobalAppState::getInstance().s_bEnableGlobalLocalStreaming;
			std::cout << "toggled global-local streaming" << std::endl;
			break;
		case 'M':
			std::cout << "Debugging SceneRepHash..." << std::endl;
			g_SceneRepSDFLocal.DebugHash();
			g_SceneRepChunkGrid.checkForDuplicates();
			break;
		case 'R':
			TimingLog::resetTimings();

			g_SceneRepSDFLocal.Reset(DXUTGetD3D11DeviceContext());
			g_SceneRepSDFGlobal.Reset(DXUTGetD3D11DeviceContext());

			g_SceneRepChunkGrid.Reset(DXUTGetD3D11DeviceContext());

			g_Sensor.reset();
			g_Camera.Reset();
			break;
		case 'P':	
			GlobalAppState::getInstance().s_useGradients = !GlobalAppState::getInstance().s_useGradients;
			if(GlobalAppState::getInstance().s_useGradients)
			{
				std::cout << "using gradients" << std::endl;
			}
			else
			{
				std::cout << "not using gradients" << std::endl;
			}
			break;
		case 'O':
			break;
		case 'T':
			GlobalAppState::getInstance().s_timingsDetailledEnabled = !GlobalAppState::getInstance().s_timingsDetailledEnabled;
			break;
		case 'I':
			GlobalAppState::getInstance().s_enableMultiLayerSplatting = !GlobalAppState::getInstance().s_enableMultiLayerSplatting;
			if (GlobalAppState::getInstance().s_enableMultiLayerSplatting) 
			{
				std::cout << "using multi layer splatting" << std::endl;
			} 
			else 
			{
				std::cout << "not using multi layer splatting" << std::endl;
			}
			break;
		case 'F':
			GlobalAppState::getInstance().s_bFilterKinectInputData = !GlobalAppState::getInstance().s_bFilterKinectInputData;
			std::cout << "Toggled filter kinect input data" << std::endl;
			break;
		case 'Y':
			GlobalAppState::getInstance().getDepthSensor()->toggleNearMode();
			std::cout << "Toggled near mode" << std::endl;
			break;
		case 'H':
			GlobalAppState::getInstance().getDepthSensor()->toggleAutoWhiteBalance();
			std::cout << "Toggled auto white balance" << std::endl;
			break;
		case 'C':
			GlobalAppState::getInstance().s_bRegistrationEnabled = !GlobalAppState::getInstance().s_bRegistrationEnabled;
			std::cout << "Toggled registration" << std::endl;
			break;
		case '0':
			GlobalAppState::getInstance().s_DisplayTexture = 0;
			std::cout << "Standard rendering" << std::endl;
			break;
		case '1':	
			GlobalAppState::getInstance().s_DisplayTexture = 1;
			std::cout << "Kinect Input Depth Float3" << std::endl;
			break;
		case '2':
			GlobalAppState::getInstance().s_DisplayTexture = 2;
			std::cout << "Kinect Input Normal Float3" << std::endl;
			break;
		case '3':
			GlobalAppState::getInstance().s_DisplayTexture = 3;
			std::cout << "Rendered Depth Float3" << std::endl;
			break;
		case '4':
			GlobalAppState::getInstance().s_DisplayTexture = 4;
			std::cout << "Kinect Input Colors" << std::endl;
			break;
		case '5':
			GlobalAppState::getInstance().s_DisplayTexture = 5;
			std::cout << "Rendered Phong Shaded" << std::endl;
			break;
		case '6':
			GlobalAppState::getInstance().s_bApplicationEnabled = !GlobalAppState::getInstance().s_bApplicationEnabled;
			if(GlobalAppState::getInstance().s_bApplicationEnabled) 
			{
				std::cout << "application started" << std::endl;
			} 
			else 
			{
				std::cout << "application stopped" << std::endl;
			}
			break;
		case '7':
			g_SceneRepSDFLocal.RunCompactifyForView(DXUTGetD3D11DeviceContext());
			std::cout << "local before: " << g_SceneRepSDFLocal.GetNumOccupiedHashEntries() << std::endl;
			DX11HistogramHashSDF::computeHistrogram(DXUTGetD3D11DeviceContext(), g_SceneRepSDFLocal.GetHashSRV(), g_SceneRepSDFLocal.MapAndGetConstantBuffer(DXUTGetD3D11DeviceContext()), g_SceneRepSDFLocal.GetHashNumBuckets(), g_SceneRepSDFLocal.GetHashBucketSize(), "localHash");
			break;
		case '8':
			{
				GlobalAppState::getInstance().s_DisplayTexture = 8;
				std::cout << "Rendered Input Labels" << std::endl;
				break;
				
				//if (GlobalAppState::getInstance().s_RecordData) 
				//{
				//	g_Sensor.saveRecordedFramesToFile(GlobalAppState::getInstance().s_RecordDataFile);
				//}
				//break;
			}
		case '9':
			{
				GlobalAppState::getInstance().s_DisplayTexture = 9;
				std::cout << "Rendered Phong Shaded Labels" << std::endl;
				break;
				
				//StopScanningAndExtractIsoSurfaceMC(std::string("scan.ply"));
				//break;
			}
		case 'G':
			GlobalAppState::getInstance().s_bEnableGarbageCollection = !GlobalAppState::getInstance().s_bEnableGarbageCollection;
			std::cout << "Toggled enable garbage collect" << std::endl;
		case 'D':
			{				
				std::string pointOut = GlobalAppState::getInstance().s_ImageReaderSensorSourcePath + "ply/" + "pointcloud" + std::to_string(g_Sensor.GetFrameNumberDepth()) + ".ply";
				g_Sensor.savePointCloud(pointOut);
			}
			break;
		case 'V':
			{
				// 2017.05.26
				//StopScanningAndDumpVoxelHash();
			}
		default:
			break;
		}
	}
}

//--------------------------------------------------------------------------------------
// Handles the GUI events
//--------------------------------------------------------------------------------------
void CALLBACK OnGUIEvent( UINT nEvent, int nControlID, CDXUTControl* pControl, void* pUserContext )
{
	GlobalAppState::getInstance().s_bRenderModeChanged = true;

	switch( nControlID )
	{
		// Standard DXUT controls
	case IDC_TOGGLEFULLSCREEN:
		DXUTToggleFullScreen(); 
		break;
	case IDC_TOGGLEREF:
		DXUTToggleREF(); 
		break;
	case IDC_CHANGEDEVICE:
		g_D3DSettingsDlg.SetActive( !g_D3DSettingsDlg.IsActive() ); 
		break;
	}
}

//--------------------------------------------------------------------------------------
// Reject any D3D11 devices that aren't acceptable by returning false
//--------------------------------------------------------------------------------------
bool CALLBACK IsD3D11DeviceAcceptable( const CD3D11EnumAdapterInfo *AdapterInfo, UINT Output, const CD3D11EnumDeviceInfo *DeviceInfo, DXGI_FORMAT BackBufferFormat, bool bWindowed, void* pUserContext )
{
	return true;
}

//--------------------------------------------------------------------------------------
// Create any D3D11 resources that aren't dependent on the back buffer
//--------------------------------------------------------------------------------------
HRESULT CALLBACK OnD3D11CreateDevice( ID3D11Device* pd3dDevice, const DXGI_SURFACE_DESC* pBackBufferSurfaceDesc, void* pUserContext )
{
	HRESULT hr = S_OK;

	ID3D11DeviceContext* pd3dImmediateContext = DXUTGetD3D11DeviceContext();
	V_RETURN( g_DialogResourceManager.OnD3D11CreateDevice( pd3dDevice, pd3dImmediateContext ) );
	V_RETURN( g_D3DSettingsDlg.OnD3D11CreateDevice( pd3dDevice ) );
	g_pTxtHelper = new CDXUTTextHelper( pd3dDevice, pd3dImmediateContext, &g_DialogResourceManager, 15 );

	if ( FAILED( GlobalAppState::getInstance().getDepthSensor()->createFirstConnected() ) )
	{
		MessageBox(NULL, L"No ready Depth Sensor found!", L"Error", MB_ICONHAND | MB_OK);
		throw MLIB_EXCEPTION("failed to create device");
		return S_FALSE;
	}

	V_RETURN(g_Sensor.OnD3D11CreateDevice(pd3dDevice, GlobalAppState::getInstance().getDepthSensor())); 

	V_RETURN(g_SceneRepSDFLocal.Init(pd3dDevice, false, GlobalAppState::getInstance().s_hashNumBucketsLocal, GlobalAppState::getInstance().s_hashBucketSizeLocal, GlobalAppState::getInstance().s_hashNumSDFBlocks, GlobalAppState::getInstance().s_virtualVoxelSize));
	V_RETURN(g_SceneRepSDFGlobal.Init(pd3dDevice, true, GlobalAppState::getInstance().s_hashNumBucketsGlobal, GlobalAppState::getInstance().s_hashBucketSizeGlobal, 0, GlobalAppState::getInstance().s_virtualVoxelSize));	//ATTENTION TAKE CARE FOR SCAN SIZE

	V_RETURN(g_SceneRepChunkGrid.Init(pd3dDevice, DXUTGetD3D11DeviceContext(), GlobalAppState::getInstance().s_voxelExtends, GlobalAppState::getInstance().s_gridDimensions, GlobalAppState::getInstance().s_minGridPos, GlobalAppState::getInstance().s_virtualVoxelSize, GlobalAppState::getInstance().s_initialChunkListSize));
	V_RETURN(g_SceneRepChunkGrid.Init());

	//static init
	V_RETURN(DX11QuadDrawer::OnD3D11CreateDevice(pd3dDevice));
	V_RETURN(DX11ImageHelper::OnD3D11CreateDevice(pd3dDevice));
	V_RETURN(DX11BuildLinearSystem::OnD3D11CreateDevice(pd3dDevice));
	V_RETURN(DX11PhongLighting::OnD3D11CreateDevice(pd3dDevice));
	V_RETURN(DX11CameraTrackingMultiRes::OnD3D11CreateDevice(pd3dDevice));
	V_RETURN(DX11NormalizeReduction::OnD3D11CreateDevice(pd3dDevice));
	V_RETURN(DX11RayCastingHashSDF::OnD3D11CreateDevice(pd3dDevice));
	V_RETURN(DX11SceneRepHashSDF::OnD3D11CreateDevice(pd3dDevice));
	V_RETURN(DX11MarchingCubesHashSDF::OnD3D11CreateDevice(pd3dDevice));
	V_RETURN(DX11HistogramHashSDF::OnD3D11CreateDevice(pd3dDevice));
	V_RETURN(DX11RayMarchingStepsSplatting::OnD3D11CreateDevice(pd3dDevice));
	V_RETURN(GlobalAppState::getInstance().OnD3D11CreateDevice(pd3dDevice));

	return hr;
}

//--------------------------------------------------------------------------------------   
// Release D3D11 resources created in OnD3D10CreateDevice 
//--------------------------------------------------------------------------------------
void CALLBACK OnD3D11DestroyDevice( void* pUserContext )
{
	g_DialogResourceManager.OnD3D11DestroyDevice();
	g_D3DSettingsDlg.OnD3D11DestroyDevice();
	DXUTGetGlobalResourceCache().OnDestroyDevice();
	SAFE_DELETE( g_pTxtHelper );

	g_Sensor.OnD3D11DestroyDevice();
	g_SceneRepSDFLocal.Destroy();
	g_SceneRepSDFGlobal.Destroy();

	g_SceneRepChunkGrid.Destroy(DXUTGetD3D11DeviceContext());

	//static delete
	DX11QuadDrawer::OnD3D11DestroyDevice();
	DX11ImageHelper::OnD3D11DestroyDevice();
	DX11BuildLinearSystem::OnD3D11DestroyDevice();
	DX11PhongLighting::OnD3D11DestroyDevice();
	DX11CameraTrackingMultiRes::OnD3D11DestroyDevice();
	DX11NormalizeReduction::OnD3D11DestroyDevice();
	DX11SceneRepHashSDF::OnD3D11DestroyDevice();
	DX11RayCastingHashSDF::OnD3D11DestroyDevice();
	DX11MarchingCubesHashSDF::OnD3D11DestroyDevice();
	DX11HistogramHashSDF::OnD3D11DestroyDevice();
	DX11RayMarchingStepsSplatting::OnD3D11DestroyDevice();

	GlobalAppState::getInstance().OnD3D11DestroyDevice();
}

//--------------------------------------------------------------------------------------
// Create any D3D11 resources that depend on the back buffer
//--------------------------------------------------------------------------------------
HRESULT CALLBACK OnD3D11ResizedSwapChain( ID3D11Device* pd3dDevice, IDXGISwapChain* pSwapChain, const DXGI_SURFACE_DESC* pBackBufferSurfaceDesc, void* pUserContext )
{
	HRESULT hr = S_OK;

	V_RETURN( g_DialogResourceManager.OnD3D11ResizedSwapChain( pd3dDevice, pBackBufferSurfaceDesc ) );
	V_RETURN( g_D3DSettingsDlg.OnD3D11ResizedSwapChain( pd3dDevice, pBackBufferSurfaceDesc ) );

	// Setup the camera's projection parameters
	float fAspectRatio = pBackBufferSurfaceDesc->Width / ( FLOAT )pBackBufferSurfaceDesc->Height;
	g_Camera.SetProjParams( D3DX_PI / 4, fAspectRatio, 0.3f, 5.0f );
	g_Camera.SetWindow( pBackBufferSurfaceDesc->Width, pBackBufferSurfaceDesc->Height );
	g_Camera.SetButtonMasks( MOUSE_MIDDLE_BUTTON, MOUSE_WHEEL, MOUSE_LEFT_BUTTON );

	g_HUD.SetLocation( pBackBufferSurfaceDesc->Width - 170, 0 );
	g_HUD.SetSize( 170, 170 );
	g_SampleUI.SetLocation( pBackBufferSurfaceDesc->Width - 170, pBackBufferSurfaceDesc->Height - 300 );
	g_SampleUI.SetSize( 170, 300 );

	return hr;
}

//--------------------------------------------------------------------------------------
// Release D3D11 resources created in OnD3D10ResizedSwapChain 
//--------------------------------------------------------------------------------------
void CALLBACK OnD3D11ReleasingSwapChain( void* pUserContext )
{
	g_DialogResourceManager.OnD3D11ReleasingSwapChain();
}

void DumpAllRendering( ID3D11DeviceContext* pd3dImmediateContext )
{
	unsigned int frameNumber = g_Sensor.GetFrameNumberDepth();

	std::string baseFolder = "output\\";
	CreateDirectory(L"output", NULL);
	CreateDirectory(L"output\\reconstruction", NULL);
	CreateDirectory(L"output\\reconstructionColor", NULL);
	CreateDirectory(L"output\\reconstructionLabel", NULL);
	CreateDirectory(L"output\\color", NULL);
	CreateDirectory(L"output\\normal", NULL);
	CreateDirectory(L"output\\depth", NULL);
	CreateDirectory(L"output\\label", NULL);

	std::stringstream ssFrameNumber;	unsigned int numCountDigits = 6;
	for (unsigned int i = std::max(1u, (unsigned int)std::ceilf(std::log10f((float)frameNumber+1))); i < numCountDigits; i++) ssFrameNumber << "0";
	ssFrameNumber << frameNumber;

	{
		//reconstruction (normal)
		static float ClearColor[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
		ID3D11RenderTargetView* pRTV = DXUTGetD3D11RenderTargetView();
		ID3D11DepthStencilView* pDSV = DXUTGetD3D11DepthStencilView();
		pd3dImmediateContext->ClearRenderTargetView(pRTV, ClearColor);
		pd3dImmediateContext->ClearDepthStencilView(pDSV, D3D11_CLEAR_DEPTH, 1.0f, 0);
		DX11PhongLighting::render(pd3dImmediateContext, DX11RayCastingHashSDF::getPositonsImageSRV(), DX11RayCastingHashSDF::getNormalsImageSRV(), DX11RayCastingHashSDF::getColorsImageSRV(), DX11RayCastingHashSDF::getSSAOMapFilteredSRV(), false, false);

		std::string fileName = baseFolder + "reconstruction\\" + ssFrameNumber.str() + ".png";
		std::wstring fileNameW(fileName.begin(), fileName.end());
		DXUTSnapD3D11Screenshot(fileNameW.c_str(), D3DX11_IFF_PNG);
	}
	{
		//reconstruction (color)
		static float ClearColor[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
		ID3D11RenderTargetView* pRTV = DXUTGetD3D11RenderTargetView();
		ID3D11DepthStencilView* pDSV = DXUTGetD3D11DepthStencilView();
		pd3dImmediateContext->ClearRenderTargetView(pRTV, ClearColor);
		pd3dImmediateContext->ClearDepthStencilView(pDSV, D3D11_CLEAR_DEPTH, 1.0f, 0);
		DX11PhongLighting::render(pd3dImmediateContext, DX11RayCastingHashSDF::getPositonsImageSRV(), DX11RayCastingHashSDF::getNormalsImageSRV(), DX11RayCastingHashSDF::getColorsImageSRV(), DX11RayCastingHashSDF::getSSAOMapFilteredSRV(), true, true);

		std::string fileName = baseFolder + "reconstructionColor\\" + ssFrameNumber.str() + ".png";
		std::wstring fileNameW(fileName.begin(), fileName.end());
		DXUTSnapD3D11Screenshot(fileNameW.c_str(), D3DX11_IFF_PNG);
	}
	{
		//reconstruction (label)
		static float ClearColor[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
		ID3D11RenderTargetView* pRTV = DXUTGetD3D11RenderTargetView();
		ID3D11DepthStencilView* pDSV = DXUTGetD3D11DepthStencilView();
		pd3dImmediateContext->ClearRenderTargetView(pRTV, ClearColor);
		pd3dImmediateContext->ClearDepthStencilView(pDSV, D3D11_CLEAR_DEPTH, 1.0f, 0);
		DX11PhongLighting::render(pd3dImmediateContext, DX11RayCastingHashSDF::getPositonsImageSRV(), DX11RayCastingHashSDF::getNormalsImageSRV(), DX11RayCastingHashSDF::getLabelsImageSRV(), DX11RayCastingHashSDF::getSSAOMapFilteredSRV(), true, true);

		std::string fileName = baseFolder + "reconstructionLabel\\" + ssFrameNumber.str() + ".png";
		std::wstring fileNameW(fileName.begin(), fileName.end());
		DXUTSnapD3D11Screenshot(fileNameW.c_str(), D3DX11_IFF_PNG);
	}
	{
		//input color
		static float ClearColor[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
		ID3D11RenderTargetView* pRTV = DXUTGetD3D11RenderTargetView();
		ID3D11DepthStencilView* pDSV = DXUTGetD3D11DepthStencilView();
		pd3dImmediateContext->ClearRenderTargetView(pRTV, ClearColor);
		pd3dImmediateContext->ClearDepthStencilView(pDSV, D3D11_CLEAR_DEPTH, 1.0f, 0);
		DX11QuadDrawer::RenderQuad(pd3dImmediateContext, g_Sensor.GetColorSRV());
		std::string fileName = baseFolder + "color\\" + ssFrameNumber.str() + ".png";
		std::wstring fileNameW(fileName.begin(), fileName.end());
		DXUTSnapD3D11Screenshot(fileNameW.c_str(), D3DX11_IFF_PNG);
	}
	{
		//input depth
		static float ClearColor[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
		ID3D11RenderTargetView* pRTV = DXUTGetD3D11RenderTargetView();
		ID3D11DepthStencilView* pDSV = DXUTGetD3D11DepthStencilView();
		pd3dImmediateContext->ClearRenderTargetView(pRTV, ClearColor);
		pd3dImmediateContext->ClearDepthStencilView(pDSV, D3D11_CLEAR_DEPTH, 1.0f, 0);
		DX11QuadDrawer::RenderQuad(pd3dImmediateContext, g_Sensor.GetHSVDepthFloat4SRV(), 1.0f);
		std::string fileName = baseFolder + "depth\\" + ssFrameNumber.str() + ".png";
		std::wstring fileNameW(fileName.begin(), fileName.end());
		DXUTSnapD3D11Screenshot(fileNameW.c_str(), D3DX11_IFF_PNG);
	}
	{
		//input normal
		static float ClearColor[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
		ID3D11RenderTargetView* pRTV = DXUTGetD3D11RenderTargetView();
		ID3D11DepthStencilView* pDSV = DXUTGetD3D11DepthStencilView();
		pd3dImmediateContext->ClearRenderTargetView(pRTV, ClearColor);
		pd3dImmediateContext->ClearDepthStencilView(pDSV, D3D11_CLEAR_DEPTH, 1.0f, 0);
		DX11QuadDrawer::RenderQuad(pd3dImmediateContext, g_Sensor.GetNormalFloat4SRV(), 1.0f);
		std::string fileName = baseFolder + "normal\\" + ssFrameNumber.str() + ".png";
		std::wstring fileNameW(fileName.begin(), fileName.end());
		DXUTSnapD3D11Screenshot(fileNameW.c_str(), D3DX11_IFF_PNG);
	}
	{
		//input label
		static float ClearColor[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
		ID3D11RenderTargetView* pRTV = DXUTGetD3D11RenderTargetView();
		ID3D11DepthStencilView* pDSV = DXUTGetD3D11DepthStencilView();
		pd3dImmediateContext->ClearRenderTargetView(pRTV, ClearColor);
		pd3dImmediateContext->ClearDepthStencilView(pDSV, D3D11_CLEAR_DEPTH, 1.0f, 0);
		DX11QuadDrawer::RenderQuad(pd3dImmediateContext, g_Sensor.GetLabelSRV(), 1.0f);
		std::string fileName = baseFolder + "label\\" + ssFrameNumber.str() + ".png";
		std::wstring fileNameW(fileName.begin(), fileName.end());
		DXUTSnapD3D11Screenshot(fileNameW.c_str(), D3DX11_IFF_PNG);
	}
}

//--------------------------------------------------------------------------------------
// Render the scene using the D3D11 device
//--------------------------------------------------------------------------------------
void CALLBACK OnD3D11FrameRender( ID3D11Device* pd3dDevice, ID3D11DeviceContext* pd3dImmediateContext, double fTime, float fElapsedTime, void* pUserContext )
{	
	// screen shot
	if (false)
	{
		static int frameIndex = 0;

		WCHAR fileName[100];
		size_t origsize = strlen(GlobalAppState::getInstance().s_ImageReaderSensorSourcePath.c_str()) + 1;
		size_t convertedChars = 0;
		mbstowcs_s(&convertedChars, fileName, origsize, GlobalAppState::getInstance().s_ImageReaderSensorSourcePath.c_str(), _TRUNCATE);

		//wcscat(fileName, L"screenshot/");

		WCHAR fileIndex[10];
		swprintf_s(fileIndex, L"%07d", frameIndex);
		wcscat(fileName, fileIndex);
		wcscat(fileName, L".jpg");

		//CaptureAnImage(DXUTGetHWND(), fileName);
		DXUTSnapD3D11Screenshot(fileName, D3DX11_IFF_JPG);

		frameIndex++;
	}
	// screen shot
	
	if (GlobalAppState::getInstance().s_bApplicationEnabled)
	{
		g_SceneRepSDFLocal.SetEnableGarbageCollect(GlobalAppState::getInstance().s_bEnableGarbageCollection);
	
		if (GlobalAppState::getInstance().s_bRenderModeChanged)
		{
			TimingLog::resetTimings();
			GlobalAppState::getInstance().s_bRenderModeChanged = false;
		}

		// If the settings dialog is being shown, then render it instead of rendering the app's scene
		//if ( g_D3DSettingsDlg.IsActive() )
		//{
		//	g_D3DSettingsDlg.OnRender( fElapsedTime );
		//	return;
		//}

		g_Sensor.setFiterDepthValues(GlobalAppState::getInstance().s_bFilterKinectInputData, 2.5f, 0.03f);

		// Clear the back buffer
		static float ClearColor[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
		ID3D11RenderTargetView* pRTV = DXUTGetD3D11RenderTargetView();
		ID3D11DepthStencilView* pDSV = DXUTGetD3D11DepthStencilView();
		pd3dImmediateContext->ClearRenderTargetView(pRTV, ClearColor);
		pd3dImmediateContext->ClearDepthStencilView(pDSV, D3D11_CLEAR_DEPTH, 1.0f, 0);

		// if we have received any valid new depth data we may need to draw
		if (GlobalAppState::getInstance().s_timingsTotalEnabled) 
		{
			GlobalAppState::getInstance().WaitForGPU();
			GlobalAppState::getInstance().s_Timer.start();
		}

		if (GlobalAppState::getInstance().s_timingsStepsEnabled) 
		{
			GlobalAppState::getInstance().WaitForGPU();
			GlobalAppState::getInstance().s_Timer.start();
		}

  		HRESULT hr0 = g_Sensor.processDepth(pd3dImmediateContext); // shouldn't hr0 and h1 be used to check if new work has to be done? registration etc
		HRESULT hr1 = g_Sensor.processColor(pd3dImmediateContext);
		HRESULT hr2 = g_Sensor.processLabel(pd3dImmediateContext);
		//HRESULT hr3 = g_Sensor.processPreDepth(pd3dImmediateContext);
		//HRESULT hr4 = g_Sensor.processPreColor(pd3dImmediateContext);
		HRESULT hr5 = g_Sensor.processPreLabel(pd3dImmediateContext);

		if (hr0 == S_OK)
		{
			//bool printFrameNumbers = true;
			//if (printFrameNumbers)
			//{
			//	std::cout << g_Sensor.GetFrameNumberDepth() << std::endl;
			//}

			if (GlobalAppState::getInstance().s_DataDumpDepthData) 
			{
				std::stringstream ss;	ss << GlobalAppState::getInstance().s_DataDumpPath;
				for (unsigned int i = std::max(1u,g_Sensor.GetFrameNumberDepth()); i < 1000000; i *= 10) ss << "0";
				ss << g_Sensor.GetFrameNumberDepth() << ".mbindepth";
				std::cout << "Dumping " << ss.str() << std::endl;
				g_Sensor.writeDepthDataToFile(ss.str());
			}

			if (GlobalAppState::getInstance().s_DataDumpColorData) 
			{
				std::stringstream ss;	ss << GlobalAppState::getInstance().s_DataDumpPath;
				for (unsigned int i = std::max(1u,g_Sensor.GetFrameNumberDepth()); i < 1000000; i *= 10) ss << "0";
				ss << g_Sensor.GetFrameNumberDepth() << ".binRGB";
				std::cout << "Dumping " << ss.str() << std::endl;
				g_Sensor.writeColorDataToFile(ss.str());
			}

			//if (GlobalAppState::getInstance().s_DataDumpLabelData)
			//{
			//	std::stringstream ss;	ss << GlobalAppState::getInstance().s_DataDumpPath;
			//	for (unsigned int i = std::max(1u, g_Sensor.GetFrameNumberDepth()); i < 1000000; i *= 10) ss << "0";
			//	ss << g_Sensor.GetFrameNumberDepth() << ".binLabel";
			//	std::cout << "Dumping " << ss.str() << std::endl;
			//	g_Sensor.writeLabelDataToFile(ss.str());
			//}

			if (GlobalAppState::getInstance().s_RecordData) 
			{
				g_Sensor.recordFrame();
			}
		}

		// dumps data at a specific frame number (if specified)
		if (GlobalAppState::getInstance().s_ImageReaderSensorNumFrames > 0) 
		{
			if (g_Sensor.GetFrameNumberDepth() == GlobalAppState::getInstance().s_ImageReaderSensorNumFrames - 1)
			{	
				vec4ui colors[LABEL_TEXTURE_NUM*LABEL_TEXTURE_CHANNEL];
				if (GlobalAppState::getInstance().s_labelNum == 3)
				{
					// Table
					colors[0] = vec4ui(225, 225, 225, 255); colors[1] = vec4ui(255, 219, 100, 255); colors[2]  = vec4ui(157, 197, 233, 255); colors[3]  = vec4ui(255, 255, 255, 255);
					colors[4] = vec4ui(255, 255, 255, 255); colors[5] = vec4ui(255, 255, 255, 255); colors[6]  = vec4ui(255, 255, 255, 255); colors[7]  = vec4ui(255, 255, 255, 255);
					colors[8] = vec4ui(255, 255, 255, 255); colors[9] = vec4ui(255, 255, 255, 255); colors[10] = vec4ui(255, 255, 255, 255); colors[11] = vec4ui(255, 255, 255, 255);
				}
				else if (GlobalAppState::getInstance().s_labelNum == 5)
				{
					// Chair
					colors[0] = vec4ui(225, 225, 225, 255); colors[1] = vec4ui(243, 177, 133, 255); colors[2]  = vec4ui(255, 219, 100, 255); colors[3]  = vec4ui(157, 233, 225, 255);
					colors[4] = vec4ui(157, 197, 233, 255); colors[5] = vec4ui(255, 255, 255, 255); colors[6]  = vec4ui(255, 255, 255, 255); colors[7]  = vec4ui(255, 255, 255, 255);
					colors[8] = vec4ui(255, 255, 255, 255); colors[9] = vec4ui(255, 255, 255, 255); colors[10] = vec4ui(255, 255, 255, 255); colors[11] = vec4ui(255, 255, 255, 255);
				}
				else if (GlobalAppState::getInstance().s_labelNum == 7)
				{
					// Motorcycle
					colors[0] = vec4ui(225, 225, 225, 255); colors[1] = vec4ui(157, 197, 233, 255); colors[2]  = vec4ui(243, 177, 133, 255); colors[3]  = vec4ui(233, 157, 165, 255);
					colors[4] = vec4ui(255, 219, 100, 255); colors[5] = vec4ui(157, 233, 225, 255); colors[6]  = vec4ui(167, 211, 143, 255); colors[7]  = vec4ui(255, 255, 255, 255);
					colors[8] = vec4ui(255, 255, 255, 255); colors[9] = vec4ui(255, 255, 255, 255); colors[10] = vec4ui(255, 255, 255, 255); colors[11] = vec4ui(255, 255, 255, 255);
				}
				
				// Marching cubes--------------------------------------------------------------------------------------------------
				if (true)
				{
					StopScanningAndExtractIsoSurfaceMC(std::string("scan.ply"));
				}
				// Marching cubes--------------------------------------------------------------------------------------------------

				if (false)
				{
					g_SceneRepChunkGrid.stopMultiThreading();
					g_SceneRepChunkGrid.StreamOutToCPUAll(pd3dImmediateContext, g_SceneRepSDFLocal);
					
					// Supervoxels-----------------------------------------------------------------------------------------------------
					std::vector<vec3i>& s_meshVoxels = DX11MarchingCubesHashSDF::getVirtualVoxels();
					std::vector<vec3i>& s_meshLabels = DX11MarchingCubesHashSDF::getLabels();

					std::vector<bool> isToSegment;
					std::vector<bool> isNotToSegment;
					std::unordered_map<vec3i, int, std::hash<vec3i>> pointIndicesMap;

					for (int i = 0; i < s_meshVoxels.size(); i++)
					{
						std::unordered_map<vec3i, int, std::hash<vec3i>>::iterator it = pointIndicesMap.find(s_meshVoxels[i]);
						if (it == pointIndicesMap.end())
						{
							isToSegment.push_back(1);
							pointIndicesMap.insert(std::make_pair(s_meshVoxels[i], i));
						}
						else
						{
							isToSegment.push_back(0);
						}

						isNotToSegment.push_back(0);
					}

					// Supervoxel clusters map
					std::map<uint32_t, pcl::Supervoxel<PointT>::Ptr> supervoxel_clusters;
					// Supervoxel adjacency multimap
					std::multimap<uint32_t, uint32_t> supervoxel_adjacency;

					// Labeled segmentation cloud
					std::vector<std::vector<vec3i>> labeledSegCloud;
					// Labeled segmentation cloud distribution
					std::vector<std::vector<float>> labelDistOfSegCloud;

					// Labeled segmentation cloud indices
					std::vector<std::vector<int>> pointsIndicesOfSegCloud;

					int maxSupervoxelLabel = 0;
					int stepCounter = 0;

					bool disable_transform = true;

					float voxel_resolution   = 0.008f;
					float seed_resolution    = 0.80f;
					float color_importance   = 0.01f;
					float spatial_importance = 0.40f;
					float normal_importance  = 1.00f;
						
					do 
					{
						supervoxel_clusters.clear();
						supervoxel_adjacency.clear();

						labeledSegCloud.clear();
						labelDistOfSegCloud.clear();
						pointsIndicesOfSegCloud.clear();
						
						std::vector<vec3i> meshVoxels;
						std::vector<vec3i> meshLabels;

						for (int i = 0; i < s_meshVoxels.size(); i++)
						{
							if (isToSegment[i])
							{
								meshVoxels.push_back(s_meshVoxels[i]);
								meshLabels.push_back(s_meshLabels[i]);
							}
						}
							
						PointCloudT::Ptr cloud = boost::shared_ptr<PointCloudT>(new PointCloudT());
						cloud->width = meshVoxels.size();
						cloud->height = 1;
						cloud->is_dense = false;
						cloud->points.resize(cloud->width*cloud->height);

						for (int i = 0; i < cloud->points.size(); i++)
						{
							cloud->points[i].x = (float)meshVoxels[i].x*GlobalAppState::getInstance().s_virtualVoxelSize;
							cloud->points[i].y = (float)meshVoxels[i].y*GlobalAppState::getInstance().s_virtualVoxelSize;
							cloud->points[i].z = (float)meshVoxels[i].z*GlobalAppState::getInstance().s_virtualVoxelSize;

							int mlabel[LABEL_TEXTURE_NUM*LABEL_TEXTURE_CHANNEL]; int last;

							last = meshLabels[i].x;
							mlabel[0]  = last & 0x000000ff;
							last >>= 0x8;
							mlabel[1]  = last & 0x000000ff;
							last >>= 0x8;
							mlabel[2]  = last & 0x000000ff;
							last >>= 0x8;
							mlabel[3]  = last & 0x000000ff;

							last = meshLabels[i].y;
							mlabel[4]  = last & 0x000000ff;
							last >>= 0x8;
							mlabel[5]  = last & 0x000000ff;
							last >>= 0x8;
							mlabel[6]  = last & 0x000000ff;
							last >>= 0x8;
							mlabel[7]  = last & 0x000000ff;

							last = meshLabels[i].z;
							mlabel[8]  = last & 0x000000ff;
							last >>= 0x8;
							mlabel[9]  = last & 0x000000ff;
							last >>= 0x8;
							mlabel[10] = last & 0x000000ff;
							last >>= 0x8;
							mlabel[11] = last & 0x000000ff;

							unsigned int maxIndex = 0; float maxLabel = 0.0f;
							for (int j = 0; j < GlobalAppState::getInstance().s_labelNum; j++)
							{
								if ((float)mlabel[j]/255.0f > maxLabel)
								{
									maxIndex = j;
									maxLabel = (float)mlabel[j]/255.0f;
								}
							}

							unsigned int vColor = 0;
							vColor |= colors[maxIndex].w & 0x000000ff;
							vColor <<= 8;
							vColor |= colors[maxIndex].x & 0x000000ff;
							vColor <<= 8;
							vColor |= colors[maxIndex].y & 0x000000ff;
							vColor <<= 8;
							vColor |= colors[maxIndex].z & 0x000000ff;

							cloud->points[i].rgba = vColor;
						}

						////////////////////////////////////////////////////////////
						// Supervoxels
						////////////////////////////////////////////////////////////
						pcl::SupervoxelClustering<PointT> super(voxel_resolution, seed_resolution);

						if (disable_transform)
							super.setUseSingleCameraTransform(false);

						super.setInputCloud(cloud);
						super.setColorImportance(color_importance);
						super.setSpatialImportance(spatial_importance);
						super.setNormalImportance(normal_importance);

						std::cout << "Extracting supervoxels!*********************" << std::endl;
						super.extract(supervoxel_clusters);
						std::cout << "Found " << supervoxel_clusters.size() << " supervoxels" << std::endl;
						maxSupervoxelLabel = super.getMaxLabel();
						std::cout << "Max label: " << maxSupervoxelLabel << std::endl << std::endl;

						pcl::PointCloud<pcl::PointXYZL>::Ptr labeledCloud = super.getLabeledCloud();

						/*if (false)
						{
							boost::shared_ptr<pcl::visualization::PCLVisualizer> viewer(new pcl::visualization::PCLVisualizer("3D Viewer"));
							viewer->initCameraParameters();
							
							int v1(0);
							viewer->createViewPort(0.0, 0.0, 0.5, 1.0, v1);
							viewer->setBackgroundColor(1.0, 1.0, 1.0, v1);
							viewer->addText("cloud", 15, 15, "v1 text", v1);
							viewer->addPointCloud(cloud, "cloud", v1);

							int v2(0);
							viewer->createViewPort(0.5, 0.0, 1.0, 1.0, v2);
							viewer->setBackgroundColor(1.0, 1.0, 1.0, v2);
							viewer->addText("labeled cloud", 15, 15, "v2 text", v2);
							viewer->addPointCloud(labeledCloud, "labeled cloud", v2);

							viewer->setPointCloudRenderingProperties(pcl::visualization::PCL_VISUALIZER_POINT_SIZE, 2, "cloud");
							viewer->setPointCloudRenderingProperties(pcl::visualization::PCL_VISUALIZER_POINT_SIZE, 2, "labeled cloud");

							std::multimap<uint32_t, uint32_t>::iterator label_itr = supervoxel_adjacency.begin();
							for ( ; label_itr != supervoxel_adjacency.end(); )
							{
								uint32_t supervoxel_label = label_itr->first;
								pcl::Supervoxel<PointT>::Ptr supervoxel = supervoxel_clusters.at(supervoxel_label);

								PointCloudT adjacent_supervoxel_centers;
								std::multimap<uint32_t, uint32_t>::iterator adjacent_itr = supervoxel_adjacency.equal_range(supervoxel_label).first;
								for ( ; adjacent_itr != supervoxel_adjacency.equal_range(supervoxel_label).second; ++adjacent_itr)
								{
									pcl::Supervoxel<PointT>::Ptr neighbor_supervoxel = supervoxel_clusters.at(adjacent_itr->second);
									adjacent_supervoxel_centers.push_back(neighbor_supervoxel->centroid_);
								}

								std::stringstream ss;
								ss << "supervoxel_" << supervoxel_label;

								addSupervoxelConnectionsToViewer(supervoxel->centroid_, adjacent_supervoxel_centers, ss.str(), viewer, v2);

								label_itr = supervoxel_adjacency.upper_bound(supervoxel_label);
							}

							while (!viewer->wasStopped())
							{	
								viewer->spinOnce(100);
							}
						}*/

						labeledSegCloud.resize(super.getMaxLabel()+1);
						for (pcl::PointCloud<pcl::PointXYZL>::iterator it = labeledCloud->begin(); it != labeledCloud->end(); it++)
						{
							int px = std::round(it->x/GlobalAppState::getInstance().s_virtualVoxelSize);
							int py = std::round(it->y/GlobalAppState::getInstance().s_virtualVoxelSize);
							int pz = std::round(it->z/GlobalAppState::getInstance().s_virtualVoxelSize);

							labeledSegCloud[it->label].push_back(vec3i(px, py, pz));
						}

						int zeroSizeLabel = 0;
						for (int i = 0; i < super.getMaxLabel()+1; i++)
						{
							if (labeledSegCloud[i].size() == 0)
								zeroSizeLabel += 1;
						}
						std::cout << "Zero size label: " << zeroSizeLabel << std::endl;
						std::cout << "Seed resolution: " << seed_resolution << std::endl << std::endl;

						int *partLabelNumOfSegCloud = new int[super.getMaxLabel()+1];
						for (int i = 0; i < super.getMaxLabel()+1; i++)
						{
							partLabelNumOfSegCloud[i] = 0;
						}

						float *partLabelConfidenceOfSegCloud = new float[super.getMaxLabel()+1];
						for (int i = 0; i < super.getMaxLabel()+1; i++)
						{
							partLabelConfidenceOfSegCloud[i] = 0.0f;
						}

						labelDistOfSegCloud.resize(super.getMaxLabel()+1);
						pointsIndicesOfSegCloud.resize(super.getMaxLabel()+1);

						for (int i = 0; i < super.getMaxLabel()+1; i++)
						{
							float flabel[LABEL_TEXTURE_NUM*LABEL_TEXTURE_CHANNEL] = { 0.0f };
							for (int j = 0; j < labeledSegCloud[i].size(); j++)
							{
								std::unordered_map<vec3i, int, std::hash<vec3i>>::iterator it = pointIndicesMap.find(labeledSegCloud[i][j]);
								if (it != pointIndicesMap.end())
								{
									int mlabel[LABEL_TEXTURE_NUM*LABEL_TEXTURE_CHANNEL]; int last;
										
									last = s_meshLabels[it->second].x;
									mlabel[0] = last  & 0x000000ff;
									last >>= 0x8;
									mlabel[1] = last  & 0x000000ff;
									last >>= 0x8;
									mlabel[2] = last  & 0x000000ff;
									last >>= 0x8;
									mlabel[3] = last  & 0x000000ff;

									last = s_meshLabels[it->second].y;
									mlabel[4] = last  & 0x000000ff;
									last >>= 0x8;
									mlabel[5] = last  & 0x000000ff;
									last >>= 0x8;
									mlabel[6] = last  & 0x000000ff;
									last >>= 0x8;
									mlabel[7] = last  & 0x000000ff;

									last = s_meshLabels[it->second].z;
									mlabel[8] = last  & 0x000000ff;
									last >>= 0x8;
									mlabel[9] = last  & 0x000000ff;
									last >>= 0x8;
									mlabel[10] = last & 0x000000ff;
									last >>= 0x8;
									mlabel[11] = last & 0x000000ff;

									unsigned int maxIndex = 0; float maxLabel = 0.0f;
									for (int k = 0; k < GlobalAppState::getInstance().s_labelNum; k++)
									{
										if ((float)mlabel[k]/255.0f > maxLabel)
										{
											maxIndex = k;
											maxLabel = (float)mlabel[k]/255.0f;
										}
									}

									if (maxIndex != 0)
									{
										partLabelNumOfSegCloud[i] += 1;
									}

									for (int k = 0; k < GlobalAppState::getInstance().s_labelNum; k++)
									{
										flabel[k] += (float)mlabel[k]/255.0f;
									}

									pointsIndicesOfSegCloud[i].push_back(it->second);
								}
							}

							if (pointsIndicesOfSegCloud[i].size() != 0)
							{
								// Normalize
								for (int j = 0; j < GlobalAppState::getInstance().s_labelNum; j++)
								{
									flabel[j] /= (float)pointsIndicesOfSegCloud[i].size();

									labelDistOfSegCloud[i].push_back(flabel[j]);
								}

								float entropy = 0.0f;
								for (int j = 0; j < GlobalAppState::getInstance().s_labelNum; j++)
								{
									float p = flabel[j];

									if (p != 0.0f)
										entropy -= p*log(p);
								}

								float confidence = 1-entropy/log(GlobalAppState::getInstance().s_labelNum);

								partLabelConfidenceOfSegCloud[i] = confidence;
							}
						}

						// Connected components
						std::cout << "Getting supervoxel adjacency" << std::endl << std::endl;
						super.getSupervoxelAdjacency(supervoxel_adjacency);

						std::vector<int> connectedComponents;
						for (int i = 0; i < super.getMaxLabel()+1; i++)
						{
							connectedComponents.push_back(-1);
						}

						int componentID = 0;
						for (int i = 0; i < super.getMaxLabel()+1; i++)
						{
							if (connectedComponents[i] == -1)
							{
								connectedComponents[i] = componentID;

								std::vector<int> curComponent; curComponent.push_back(i);
								for (int j = 0; j < curComponent.size(); j++)
								{
									int labelID = curComponent[j];

									std::multimap<uint32_t, uint32_t>::iterator label_itr = supervoxel_adjacency.find(labelID);
									if (label_itr != supervoxel_adjacency.end())
									{
										uint32_t supervoxel_label = label_itr->first;

										std::multimap<uint32_t, uint32_t>::iterator adjacent_itr = supervoxel_adjacency.equal_range(supervoxel_label).first;
										for ( ; adjacent_itr != supervoxel_adjacency.equal_range(supervoxel_label).second; ++adjacent_itr)
										{
											if (connectedComponents[adjacent_itr->second] == -1)
											{
												connectedComponents[adjacent_itr->second] = componentID;
												curComponent.push_back(adjacent_itr->second);
											}
										}
									}
								}
								componentID++;
							}
						}
						// Connected components

						// Find the connected component which contains the object
						int *partLabelNumOfConnectedComponent = new int[componentID];
						int *pointNumOfConnectedComponent = new int[componentID];
						for (int i = 0; i < componentID; i++)
						{
							partLabelNumOfConnectedComponent[i] = 0;
							pointNumOfConnectedComponent[i] = 0;
						}

						for (int i = 0; i < super.getMaxLabel()+1; i++)
						{
							partLabelNumOfConnectedComponent[connectedComponents[i]] += partLabelNumOfSegCloud[i];
							pointNumOfConnectedComponent[connectedComponents[i]] += pointsIndicesOfSegCloud[i].size();
						}						

						int maxComponentID = 0, maxPartLabelNum = 0;
						for (int i = 0; i < componentID; i++)
						{
							if (partLabelNumOfConnectedComponent[i] > maxPartLabelNum)
							{
								maxComponentID = i;
								maxPartLabelNum = partLabelNumOfConnectedComponent[i];
							}
						}

						for (int i = 0; i < super.getMaxLabel()+1; i++)
						{
							if (connectedComponents[i] != maxComponentID)
							{
								for (int j = 0; j < pointsIndicesOfSegCloud[i].size(); j++)
								{
									isToSegment[pointsIndicesOfSegCloud[i][j]] = 0;
									isNotToSegment[pointsIndicesOfSegCloud[i][j]] = 1;
								}
							}
							else
							{
								if (labeledSegCloud[i].size() != 0)
								{
									float partLabelPercentage = (float)partLabelNumOfSegCloud[i]/(float)labeledSegCloud[i].size();

									if (partLabelPercentage < 0.05)    //customize 0.05
									{
										std::vector<float> surroundingPartLabelPercentage;
										
										std::multimap<uint32_t, uint32_t>::iterator label_itr = supervoxel_adjacency.find(i);
										if (label_itr != supervoxel_adjacency.end())
										{
											uint32_t supervoxel_label = label_itr->first;

											std::multimap<uint32_t, uint32_t>::iterator adjacent_itr = supervoxel_adjacency.equal_range(supervoxel_label).first;
											for ( ; adjacent_itr != supervoxel_adjacency.equal_range(supervoxel_label).second; ++adjacent_itr)
											{
												surroundingPartLabelPercentage.push_back((float)partLabelNumOfSegCloud[adjacent_itr->second]/(float)labeledSegCloud[adjacent_itr->second].size());
											}
										}

										int hPercentageNum = 0;
										for (int j = 0; j < surroundingPartLabelPercentage.size(); j++)
										{
											if (surroundingPartLabelPercentage[j] >= 0.05)    //customize
												hPercentageNum += 1;
										}
										
										if ((float)hPercentageNum/(float)surroundingPartLabelPercentage.size() < 0.5)    //customize 0.5
										{
											for (int j = 0; j < pointsIndicesOfSegCloud[i].size(); j++)
											{
												isToSegment[pointsIndicesOfSegCloud[i][j]] = 0;
												isNotToSegment[pointsIndicesOfSegCloud[i][j]] = 1;
											}
										}
									}
								}
							}
						}
						// Find the connected component which contains the object

						int hConfidenceNum = 0;
						for (std::map<uint32_t, pcl::Supervoxel<PointT>::Ptr>::iterator it = supervoxel_clusters.begin(); it != supervoxel_clusters.end(); it++)
						{
							if (partLabelConfidenceOfSegCloud[it->first] > 0.85f)    //customize
								hConfidenceNum += 1;
						}

						bool breakSupervoxel = false;
						if (((float)hConfidenceNum/(float)(supervoxel_clusters.size())) > 0.85f)    //customize
							breakSupervoxel = true;

						std::cout << "High confidence num: " << hConfidenceNum << std::endl;
						std::cout << "Supervoxel clusters size: " << supervoxel_clusters.size() << std::endl;
						
						delete[] partLabelNumOfSegCloud;
						delete[] partLabelConfidenceOfSegCloud;
						delete[] partLabelNumOfConnectedComponent;
						delete[] pointNumOfConnectedComponent;

						seed_resolution /= 2.0f;
						stepCounter++;

						// Break
						//if (breakSupervoxel)
						//	break;
						if (seed_resolution < 4*voxel_resolution)
							break;
					} while (true);
					// Supervoxels-----------------------------------------------------------------------------------------------------

					// Graph Cut-------------------------------------------------------------------------------------------------------
					int num_pixels = maxSupervoxelLabel+1;
					int num_labels = GlobalAppState::getInstance().s_labelNum-1;

					int *graphCutData = new int[num_pixels*num_labels];
					for (int i = 0; i < num_pixels; i++)
					{
						if (labelDistOfSegCloud[i].size() != 0)
						{
							std::vector<float> labelDist = labelDistOfSegCloud[i];
							for (int j = 0; j < num_labels; j++)
							{
								graphCutData[i*num_labels+j] = (int)(255*(1-labelDist[0]-labelDist[j+1]));
							}
						}
						else
						{
							for (int j = 0; j < num_labels; j++)
							{
								graphCutData[i*num_labels+j] = 0;
							}
						}
					}

					int *graphCutSmooth = new int[num_labels*num_labels];
					for (int i = 0; i < num_labels; i++)
					{
						for (int j = 0; j < num_labels; j++)
						{
							if (i == j)
							{
								graphCutSmooth[i+j*num_labels] = 0;
							}
							else
							{
								graphCutSmooth[i+j*num_labels] = 128;
							}
						}
					}

					int *graphCutResult = new int[num_pixels];

					GCoptimizationGeneralGraph *gc = new GCoptimizationGeneralGraph(num_pixels, num_labels);
					// Set data cost
					gc->setDataCost(graphCutData);
					// Set smooth cost and neighbors
					gc->setSmoothCost(graphCutSmooth);

					// Set neighbors
					std::multimap<uint32_t, uint32_t>::iterator label_itr = supervoxel_adjacency.begin();
					for ( ; label_itr != supervoxel_adjacency.end(); )
					{
						uint32_t supervoxel_label = label_itr->first;

						std::multimap<uint32_t, uint32_t>::iterator adjacent_itr = supervoxel_adjacency.equal_range(supervoxel_label).first;
						for ( ; adjacent_itr != supervoxel_adjacency.equal_range(supervoxel_label).second; ++adjacent_itr)
						{
							if (supervoxel_label < adjacent_itr->second)
								gc->setNeighbors(supervoxel_label, adjacent_itr->second);
						}

						label_itr = supervoxel_adjacency.upper_bound(supervoxel_label);
					}

					std::cout << "Before optimization energy is " << gc->compute_energy() << std::endl;
					gc->expansion(2);
					std::cout << "After optimization energy is " << gc->compute_energy() << std::endl;

					for (int i = 0; i < num_pixels; i++)
						graphCutResult[i] = gc->whatLabel(i);

					//if (false)
					//{
					//	std::vector<vec3i> meshVoxels;
					//	std::vector<vec3i> meshLabels;

					//	std::vector<vec3i> meshGCVoxels;
					//	std::vector<vec4i> meshGCLabels;

					//	for (std::map<uint32_t, pcl::Supervoxel<PointT>::Ptr>::iterator it = supervoxel_clusters.begin(); it != supervoxel_clusters.end(); it++)
					//	{
					//		for (int j = 0; j < pointsIndicesOfSegCloud[it->first].size(); j++)
					//		{
					//			meshVoxels.push_back(s_meshVoxels[pointsIndicesOfSegCloud[it->first][j]]);
					//			meshLabels.push_back(s_meshLabels[pointsIndicesOfSegCloud[it->first][j]]);

					//			meshGCVoxels.push_back(s_meshVoxels[pointsIndicesOfSegCloud[it->first][j]]);
					//			meshGCLabels.push_back(colors[graphCutResult[it->first]+1]);
					//		}
					//	}

					//	// cloud
					//	PointCloudT::Ptr cloud = boost::shared_ptr<PointCloudT>(new PointCloudT());
					//	cloud->width = meshVoxels.size();
					//	cloud->height = 1;
					//	cloud->is_dense = false;
					//	cloud->points.resize(cloud->width*cloud->height);

					//	for (int i = 0; i < cloud->points.size(); i++)
					//	{
					//		cloud->points[i].x = (float)meshVoxels[i].x*GlobalAppState::getInstance().s_virtualVoxelSize;
					//		cloud->points[i].y = (float)meshVoxels[i].y*GlobalAppState::getInstance().s_virtualVoxelSize;
					//		cloud->points[i].z = (float)meshVoxels[i].z*GlobalAppState::getInstance().s_virtualVoxelSize;

					//		int mlabel[LABEL_TEXTURE_NUM*LABEL_TEXTURE_CHANNEL]; int last;

					//		last = meshLabels[i].x;
					//		mlabel[0] = last & 0x000000ff;
					//		last >>= 0x8;
					//		mlabel[1] = last & 0x000000ff;
					//		last >>= 0x8;
					//		mlabel[2] = last & 0x000000ff;
					//		last >>= 0x8;
					//		mlabel[3] = last & 0x000000ff;

					//		last = meshLabels[i].y;
					//		mlabel[4] = last & 0x000000ff;
					//		last >>= 0x8;
					//		mlabel[5] = last & 0x000000ff;
					//		last >>= 0x8;
					//		mlabel[6] = last & 0x000000ff;
					//		last >>= 0x8;
					//		mlabel[7] = last & 0x000000ff;

					//		last = meshLabels[i].z;
					//		mlabel[8] = last & 0x000000ff;
					//		last >>= 0x8;
					//		mlabel[9] = last & 0x000000ff;
					//		last >>= 0x8;
					//		mlabel[10] = last & 0x000000ff;
					//		last >>= 0x8;
					//		mlabel[11] = last & 0x000000ff;

					//		unsigned int maxIndex = 0; float maxLabel = 0.0f;
					//		for (int j = 0; j < GlobalAppState::getInstance().s_labelNum; j++)
					//		{
					//			if ((float)mlabel[j]/255.0f > maxLabel)
					//			{
					//				maxIndex = j;
					//				maxLabel = (float)mlabel[j]/255.0f;
					//			}
					//		}

					//		unsigned int vColor = 0;
					//		vColor |= colors[maxIndex].w & 0x000000ff;
					//		vColor <<= 8;
					//		vColor |= colors[maxIndex].x & 0x000000ff;
					//		vColor <<= 8;
					//		vColor |= colors[maxIndex].y & 0x000000ff;
					//		vColor <<= 8;
					//		vColor |= colors[maxIndex].z & 0x000000ff;

					//		cloud->points[i].rgba = vColor;
					//	}
					//	
					//	// labeled cloud
					//	PointCloudT::Ptr labeledCloud = boost::shared_ptr<PointCloudT>(new PointCloudT());
					//	labeledCloud->width = meshGCVoxels.size();
					//	labeledCloud->height = 1;
					//	labeledCloud->is_dense = false;
					//	labeledCloud->points.resize(labeledCloud->width*labeledCloud->height);

					//	for (int i = 0; i < labeledCloud->points.size(); i++)
					//	{
					//		labeledCloud->points[i].x = (float)meshGCVoxels[i].x*GlobalAppState::getInstance().s_virtualVoxelSize;
					//		labeledCloud->points[i].y = (float)meshGCVoxels[i].y*GlobalAppState::getInstance().s_virtualVoxelSize;
					//		labeledCloud->points[i].z = (float)meshGCVoxels[i].z*GlobalAppState::getInstance().s_virtualVoxelSize;

					//		unsigned int vColor = 0;
					//		vColor |= (unsigned int)meshGCLabels[i].w & 0x000000ff;
					//		vColor <<= 8;
					//		vColor |= (unsigned int)meshGCLabels[i].x & 0x000000ff;
					//		vColor <<= 8;
					//		vColor |= (unsigned int)meshGCLabels[i].y & 0x000000ff;
					//		vColor <<= 8;
					//		vColor |= (unsigned int)meshGCLabels[i].z & 0x000000ff;

					//		labeledCloud->points[i].rgba = vColor;
					//	}
					//	
					//	boost::shared_ptr<pcl::visualization::PCLVisualizer> viewer(new pcl::visualization::PCLVisualizer("3D Viewer"));
					//	viewer->initCameraParameters();

					//	/*int v1(0);
					//	viewer->createViewPort(0.0, 0.0, 0.5, 1.0, v1);
					//	viewer->setBackgroundColor(1.0, 1.0, 1.0, v1);
					//	viewer->addText("cloud", 15, 15, "v1 text", v1);
					//	viewer->addPointCloud(cloud, "cloud", v1);*/

					//	int v2(0);
					//	//viewer->createViewPort(0.5, 0.0, 1.0, 1.0, v2);
					//	viewer->createViewPort(0.0, 0.0, 1.0, 1.0, v2);
					//	viewer->setBackgroundColor(1.0, 1.0, 1.0, v2);
					//	viewer->addText("labeled cloud", 15, 15, "v2 text", v2);
					//	viewer->addPointCloud(labeledCloud, "labeled cloud", v2);

					//	viewer->setPointCloudRenderingProperties(pcl::visualization::PCL_VISUALIZER_POINT_SIZE, 2, "cloud");
					//	viewer->setPointCloudRenderingProperties(pcl::visualization::PCL_VISUALIZER_POINT_SIZE, 2, "labeled cloud");

					//	while (!viewer->wasStopped())
					//	{
					//		viewer->spinOnce(100);
					//	}
					//}
					// Graph Cut-------------------------------------------------------------------------------------------------------

					// Set voxel-------------------------------------------------------------------------------------------------------
					std::vector<ChunkDesc*>& chunkGrid = g_SceneRepChunkGrid.getGrid();

					for (int i = 0; i < s_meshVoxels.size(); i++)
					{
						if (isNotToSegment[i])
						{
							vec3i voxelPos = s_meshVoxels[i];
							vec3f worldPos = VoxelUtilHelper::virtualVoxelPosToWorld(voxelPos);

							vec3i chunkPos = VoxelUtilHelper::worldToChunk(worldPos);
							unsigned int chunkIndex = VoxelUtilHelper::linearizeChunkPos(chunkPos);

							if (chunkGrid[chunkIndex] == NULL)
								continue;

							std::vector<SDFDesc>& descs = chunkGrid[chunkIndex]->getSDFBlockDescs();
							vec3i blockPos = VoxelUtilHelper::virtualVoxelPosToSDFBlock(voxelPos);

							unsigned int blockIndex;
							for (int j = 0; j < descs.size(); j++)
							{
								if (descs[j].pos == blockPos)
								{
									blockIndex = j;

									SDFBlock& block = chunkGrid[chunkIndex]->getSDFBlock(blockIndex);

									unsigned int voxelIndex = VoxelUtilHelper::virtualVoxelPosToLocalSDFBlockIndex(voxelPos);
									unsigned int vLabel[12] = { 0 }; vLabel[0] = 255;

									VoxelUtilHelper::setVoxelLabel(block, voxelIndex, vLabel);

									break;
								}
							}
						}
					}
					
					for (std::map<uint32_t, pcl::Supervoxel<PointT>::Ptr>::iterator it = supervoxel_clusters.begin(); it != supervoxel_clusters.end(); it++)
					{
						for (int i = 0; i < pointsIndicesOfSegCloud[it->first].size(); i++)
						{
							vec3i voxelPos = s_meshVoxels[pointsIndicesOfSegCloud[it->first][i]];
							vec3f worldPos = VoxelUtilHelper::virtualVoxelPosToWorld(voxelPos);

							vec3i chunkPos = VoxelUtilHelper::worldToChunk(worldPos);
							unsigned int chunkIndex = VoxelUtilHelper::linearizeChunkPos(chunkPos);

							if (chunkGrid[chunkIndex] == NULL)
								continue;

							std::vector<SDFDesc>& descs = chunkGrid[chunkIndex]->getSDFBlockDescs();
							vec3i blockPos = VoxelUtilHelper::virtualVoxelPosToSDFBlock(voxelPos);

							unsigned int blockIndex;
							for (int j = 0; j < descs.size(); j++)
							{
								if (descs[j].pos == blockPos)
								{
									blockIndex = j;

									SDFBlock& block = chunkGrid[chunkIndex]->getSDFBlock(blockIndex);

									unsigned int voxelIndex = VoxelUtilHelper::virtualVoxelPosToLocalSDFBlockIndex(voxelPos);
									unsigned int vLabel[12] = { 0 }; vLabel[graphCutResult[it->first] + 1] = 255;

									VoxelUtilHelper::setVoxelLabel(block, voxelIndex, vLabel);

									break;
								}
							}
						}
					}
					// Set voxel-------------------------------------------------------------------------------------------------------

					delete[] graphCutData;
					delete[] graphCutSmooth;
					delete[] graphCutResult;

					delete gc;

					g_SceneRepChunkGrid.startMultiThreading(DXUTGetD3D11DeviceContext());

					StopScanningAndExtractIsoSurfaceMC(std::string("scanGraphCut.ply"));
				}
				
				// Re-project------------------------------------------------------------------------------------------------------
				if (true)
				{
					// Model label
					ID3D11Texture2D** m_modelLabelsTextureFloat42D = new ID3D11Texture2D*[LABEL_TEXTURE_CHANNEL*LABEL_TEXTURE_NUM];
					ID3D11ShaderResourceView** m_modelLabelsTextureFloat4SRV = new ID3D11ShaderResourceView*[LABEL_TEXTURE_CHANNEL*LABEL_TEXTURE_NUM];
					ID3D11UnorderedAccessView** m_modelLabelsTextureFloat4UAV = new ID3D11UnorderedAccessView*[LABEL_TEXTURE_CHANNEL*LABEL_TEXTURE_NUM];

					D3D11_TEXTURE2D_DESC texDesc = { 0 };
					texDesc.Width = GlobalAppState::getInstance().s_windowWidth;
					texDesc.Height = GlobalAppState::getInstance().s_windowHeight;
					texDesc.MipLevels = 1;
					texDesc.ArraySize = 1;
					texDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
					texDesc.SampleDesc.Count = 1;
					texDesc.SampleDesc.Quality = 0;
					texDesc.Usage = D3D11_USAGE_DEFAULT;
					texDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_UNORDERED_ACCESS;
					texDesc.CPUAccessFlags = 0;
					texDesc.MiscFlags = 0;
					
					for (unsigned int i = 0; i < LABEL_TEXTURE_NUM*LABEL_TEXTURE_CHANNEL; i++)
					{
						pd3dDevice->CreateTexture2D(&texDesc, NULL, &m_modelLabelsTextureFloat42D[i]);
						pd3dDevice->CreateShaderResourceView(m_modelLabelsTextureFloat42D[i], NULL, &m_modelLabelsTextureFloat4SRV[i]);
						pd3dDevice->CreateUnorderedAccessView(m_modelLabelsTextureFloat42D[i], NULL, &m_modelLabelsTextureFloat4UAV[i]);
					}

					// Colored label
					ID3D11Texture2D* m_coloredLabelTexture = NULL;
					ID3D11ShaderResourceView* m_coloredLabelTextureSRV = NULL;
					ID3D11UnorderedAccessView* m_coloredLabelTextureUAV = NULL;

					D3D11_TEXTURE2D_DESC coloredLabelTexDesc = { 0 };
					coloredLabelTexDesc.Width = GlobalAppState::getInstance().s_windowWidth;
					coloredLabelTexDesc.Height = GlobalAppState::getInstance().s_windowHeight;
					coloredLabelTexDesc.MipLevels = 1;
					coloredLabelTexDesc.ArraySize = 1;
					coloredLabelTexDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
					coloredLabelTexDesc.SampleDesc.Count = 1;
					coloredLabelTexDesc.SampleDesc.Quality = 0;
					coloredLabelTexDesc.Usage = D3D11_USAGE_DEFAULT;
					coloredLabelTexDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_UNORDERED_ACCESS;
					coloredLabelTexDesc.CPUAccessFlags = 0;
					coloredLabelTexDesc.MiscFlags = 0;

					pd3dDevice->CreateTexture2D(&coloredLabelTexDesc, NULL, &m_coloredLabelTexture);
					pd3dDevice->CreateShaderResourceView(m_coloredLabelTexture, NULL, &m_coloredLabelTextureSRV);
					pd3dDevice->CreateUnorderedAccessView(m_coloredLabelTexture, NULL, &m_coloredLabelTextureUAV);

					// Display image
					ID3D11Texture2D* m_dispImageTextureFloat42D = NULL;
					ID3D11ShaderResourceView* m_dispImageTextureFloat4SRV = NULL;
					ID3D11UnorderedAccessView* m_dispImageTextureFloat4UAV = NULL;

					D3D11_TEXTURE2D_DESC dispDesc = { 0 };
					dispDesc.Width = GlobalAppState::getInstance().s_windowWidth;
					dispDesc.Height = GlobalAppState::getInstance().s_windowHeight;
					dispDesc.MipLevels = 1;
					dispDesc.ArraySize = 1;
					dispDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
					dispDesc.SampleDesc.Count = 1;
					dispDesc.SampleDesc.Quality = 0;
					dispDesc.Usage = D3D11_USAGE_DEFAULT;
					dispDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_UNORDERED_ACCESS;
					dispDesc.CPUAccessFlags = 0;
					dispDesc.MiscFlags = 0;

					pd3dDevice->CreateTexture2D(&dispDesc, NULL, &m_dispImageTextureFloat42D);
					pd3dDevice->CreateShaderResourceView(m_dispImageTextureFloat42D, NULL, &m_dispImageTextureFloat4SRV);
					pd3dDevice->CreateUnorderedAccessView(m_dispImageTextureFloat42D, NULL, &m_dispImageTextureFloat4UAV);
					
					g_SceneRepChunkGrid.stopMultiThreading();

					g_SceneRepChunkGrid.StreamOutToCPUAll(pd3dImmediateContext, g_SceneRepSDFLocal);
					g_SceneRepChunkGrid.StreamInToGPUAll(pd3dImmediateContext, g_SceneRepSDFLocal);
						
					for (int i = 0; i < g_FrameMat4f.size(); i++)
					{
						DX11RayCastingHashSDF::Render(pd3dImmediateContext, g_SceneRepSDFLocal.GetHashSRV(), g_SceneRepSDFLocal.GetHashSRV(), g_SceneRepSDFLocal.GetSDFBlocksSDFSRV(), g_SceneRepSDFLocal.GetSDFBlocksRGBWSRV(), g_SceneRepSDFLocal.GetSDFBlocksLabelSRV(), g_SceneRepSDFLocal.GetSDFBlocksLabelFrequencySRV(), g_SceneRepSDFLocal.GetHashBucketSize()*g_SceneRepSDFLocal.GetHashNumBuckets(), DXUTGetWindowWidth(), DXUTGetWindowHeight(), &g_FrameMat4f[i], g_SceneRepSDFLocal.MapAndGetConstantBuffer(pd3dImmediateContext));

						for (unsigned int j = 0; j < LABEL_TEXTURE_NUM*LABEL_TEXTURE_CHANNEL; j++)
						{
							DX11ImageHelper::applySplitLabelToFloat(pd3dImmediateContext, DX11RayCastingHashSDF::getLabelsImageSRV(), m_modelLabelsTextureFloat4UAV[j], GlobalAppState::getInstance().s_windowWidth, GlobalAppState::getInstance().s_windowHeight, GlobalAppState::getInstance().s_labelNum, j);
						}

						Eigen::Vector4i color[12];
						color[0] = Eigen::Vector4i(colors[0].x, colors[0].y, colors[0].z, colors[0].w); color[1] = Eigen::Vector4i(colors[1].x, colors[1].y, colors[1].z, colors[1].w); color[2] = Eigen::Vector4i(colors[2].x, colors[2].y, colors[2].z, colors[2].w);     color[3] = Eigen::Vector4i(colors[3].x, colors[3].y, colors[3].z, colors[3].w);
						color[4] = Eigen::Vector4i(colors[4].x, colors[4].y, colors[4].z, colors[4].w); color[5] = Eigen::Vector4i(colors[5].x, colors[5].y, colors[5].z, colors[5].w); color[6] = Eigen::Vector4i(colors[6].x, colors[6].y, colors[6].z, colors[6].w);     color[7] = Eigen::Vector4i(colors[7].x, colors[7].y, colors[7].z, colors[7].w);
						color[8] = Eigen::Vector4i(colors[8].x, colors[8].y, colors[8].z, colors[8].w); color[9] = Eigen::Vector4i(colors[9].x, colors[9].y, colors[9].z, colors[9].w); color[10] = Eigen::Vector4i(colors[10].x, colors[10].y, colors[10].z, colors[10].w); color[11] = Eigen::Vector4i(colors[11].x, colors[11].y, colors[11].z, colors[11].w);

						DX11ImageHelper::applyModelLabelToRGBW(pd3dImmediateContext, DX11RayCastingHashSDF::getPositonsImageSRV(), m_modelLabelsTextureFloat4SRV, m_coloredLabelTextureUAV, color, GlobalAppState::getInstance().s_windowWidth, GlobalAppState::getInstance().s_windowHeight, GlobalAppState::getInstance().s_labelNum);
						DX11ImageHelper::applyConvertLabel(pd3dImmediateContext, m_coloredLabelTextureSRV, m_dispImageTextureFloat4UAV, GlobalAppState::getInstance().s_windowWidth, GlobalAppState::getInstance().s_windowHeight);

						WCHAR fileName[100];
						size_t origsize = strlen(GlobalAppState::getInstance().s_ImageReaderSensorSourcePath.c_str()) + 1;
						size_t convertedChars = 0;
						mbstowcs_s(&convertedChars, fileName, origsize, GlobalAppState::getInstance().s_ImageReaderSensorSourcePath.c_str(), _TRUNCATE);

						WCHAR fileIndex[10];
						swprintf_s(fileIndex, L"%07d", i+1);
						wcscat(fileName, fileIndex);
						wcscat(fileName, L"modelColoredL.jpg");
						D3DX11SaveTextureToFile(pd3dImmediateContext, m_dispImageTextureFloat42D, D3DX11_IFF_JPG, fileName);
					}

					g_SceneRepChunkGrid.StreamOutToCPUAll(pd3dImmediateContext, g_SceneRepSDFLocal);
					g_SceneRepChunkGrid.startMultiThreading(DXUTGetD3D11DeviceContext());

					for (unsigned int i = 0; i < LABEL_TEXTURE_NUM*LABEL_TEXTURE_CHANNEL; i++)
					{
						SAFE_RELEASE(m_modelLabelsTextureFloat42D[i]);
						SAFE_RELEASE(m_modelLabelsTextureFloat4SRV[i]);
						SAFE_RELEASE(m_modelLabelsTextureFloat4UAV[i]);
					}

					SAFE_DELETE_ARRAY(m_modelLabelsTextureFloat42D);
					SAFE_DELETE_ARRAY(m_modelLabelsTextureFloat4SRV);
					SAFE_DELETE_ARRAY(m_modelLabelsTextureFloat4UAV);

					SAFE_RELEASE(m_dispImageTextureFloat42D);
					SAFE_RELEASE(m_dispImageTextureFloat4SRV);
					SAFE_RELEASE(m_dispImageTextureFloat4UAV);

					SAFE_RELEASE(m_coloredLabelTexture);
					SAFE_RELEASE(m_coloredLabelTextureSRV);
					SAFE_RELEASE(m_coloredLabelTextureUAV);
				}
				// Re-project------------------------------------------------------------------------------------------------------
				
				exit(0);
			}
		}

		if (GlobalAppState::getInstance().s_timingsStepsEnabled) 
		{
			GlobalAppState::getInstance().WaitForGPU();
			GlobalAppState::getInstance().s_Timer.stop();
			TimingLog::countTimeMisc++;
			TimingLog::totalTimeMisc += GlobalAppState::getInstance().s_Timer.getElapsedTimeMS();
		}

		HRESULT hr = S_OK;

		if (GlobalAppState::getInstance().s_DisplayTexture == 0 || GlobalAppState::getInstance().s_DisplayTexture == 3 || GlobalAppState::getInstance().s_DisplayTexture == 5 || GlobalAppState::getInstance().s_DisplayTexture == 9)
		{
			// Voxel Hashing
			mat4f trans = g_SceneRepSDFLocal.GetLastRigidTransform();
			g_FrameMat4f.push_back(trans);

			if (GlobalAppState::getInstance().s_RenderMode == RENDERMODE_VIEW)
			{
				D3DXMATRIX view = *g_Camera.GetViewMatrix();
				D3DXMatrixInverse(&view, NULL, &view);
				D3DXMatrixTranspose(&view, &view);
				trans = trans * *(mat4f*)&view;

				vec4f posWorld = trans*GlobalAppState::getInstance().s_StreamingPos; // trans laggs one frame *trans
				vec3f p(posWorld.x, posWorld.y, posWorld.z);
			}

			if (!(GlobalAppState::getInstance().s_RenderMode == RENDERMODE_INTEGRATE && GlobalAppState::getInstance().s_bRegistrationEnabled && hr0 == S_OK)) 
			{
				if (GlobalAppState::getInstance().s_timingsDetailledEnabled) 
				{
					GlobalAppState::getInstance().WaitForGPU();
					GlobalAppState::getInstance().s_Timer.start();
				}

				if (GlobalAppState::getInstance().s_bEnableGlobalLocalStreaming) 
				{
					g_SceneRepSDFLocal.RemoveAndIntegrateToOther(pd3dImmediateContext, &g_SceneRepSDFGlobal, &trans, true);
					g_SceneRepSDFGlobal.RemoveAndIntegrateToOther(pd3dImmediateContext, &g_SceneRepSDFLocal, &trans, false);
				}

				if (GlobalAppState::getInstance().s_timingsDetailledEnabled) 
				{
					GlobalAppState::getInstance().WaitForGPU();
					GlobalAppState::getInstance().s_Timer.stop();

					TimingLog::countRemoveAndIntegrate++;
					TimingLog::totalTimeRemoveAndIntegrate += GlobalAppState::getInstance().s_Timer.getElapsedTimeMS();
				}
			}

			if (GlobalAppState::getInstance().s_RenderMode == RENDERMODE_VIEW) 
			{
				g_SceneRepSDFLocal.RunCompactifyForView(pd3dImmediateContext);
				g_SceneRepSDFGlobal.RunCompactifyForView(pd3dImmediateContext);
			}

			if (GlobalAppState::getInstance().s_timingsStepsEnabled) 
			{
				GlobalAppState::getInstance().WaitForGPU();
				GlobalAppState::getInstance().s_Timer.start();
			}

			DX11RayCastingHashSDF::Render(pd3dImmediateContext, g_SceneRepSDFLocal.GetHashSRV(), g_SceneRepSDFLocal.GetHashSRV(), g_SceneRepSDFLocal.GetSDFBlocksSDFSRV(), g_SceneRepSDFLocal.GetSDFBlocksRGBWSRV(), g_SceneRepSDFLocal.GetSDFBlocksLabelSRV(), g_SceneRepSDFLocal.GetSDFBlocksLabelFrequencySRV(), g_SceneRepSDFLocal.GetHashBucketSize()*g_SceneRepSDFLocal.GetHashNumBuckets(), DXUTGetWindowWidth(), DXUTGetWindowHeight(), &trans, g_SceneRepSDFLocal.MapAndGetConstantBuffer(pd3dImmediateContext));
			DX11RayCastingHashSDF::RenderStereo(pd3dImmediateContext, g_SceneRepSDFLocal.GetHashSRV(), g_SceneRepSDFLocal.GetHashCompactifiedSRV(), g_SceneRepSDFLocal.GetSDFBlocksSDFSRV(), g_SceneRepSDFLocal.GetSDFBlocksRGBWSRV(),  g_SceneRepSDFLocal.GetSDFBlocksLabelSRV(), g_SceneRepSDFLocal.GetSDFBlocksLabelFrequencySRV(), g_SceneRepSDFLocal.GetNumOccupiedHashEntries(), GlobalAppState::getInstance().s_windowWidthStereo, GlobalAppState::getInstance().s_windowHeightStereo, &trans, g_SceneRepSDFLocal.MapAndGetConstantBuffer(pd3dImmediateContext));

			if (GlobalAppState::getInstance().s_timingsStepsEnabled)
			{
				GlobalAppState::getInstance().WaitForGPU();
				GlobalAppState::getInstance().s_Timer.stop();

				TimingLog::countTimeRender++;
				TimingLog::totalTimeRender += GlobalAppState::getInstance().s_Timer.getElapsedTimeMS();
			}

			if (GlobalAppState::getInstance().s_DisplayTexture == 0)
			{
				DX11PhongLighting::render(pd3dImmediateContext, DX11RayCastingHashSDF::getPositonsImageSRV(), DX11RayCastingHashSDF::getNormalsImageSRV(), DX11RayCastingHashSDF::getColorsImageSRV(), DX11RayCastingHashSDF::getSSAOMapFilteredSRV(), false, false);
			}
			else if (GlobalAppState::getInstance().s_DisplayTexture == 3)
			{
				DX11PhongLighting::render(pd3dImmediateContext, DX11RayCastingHashSDF::getPositonsImageSRV(), DX11RayCastingHashSDF::getNormalsImageSRV(), DX11RayCastingHashSDF::getColorsImageSRV(), DX11RayCastingHashSDF::getSSAOMapFilteredSRV(), false, true);
			}
			else if (GlobalAppState::getInstance().s_DisplayTexture == 5)
			{
				DX11PhongLighting::render(pd3dImmediateContext, DX11RayCastingHashSDF::getPositonsImageSRV(), DX11RayCastingHashSDF::getNormalsImageSRV(), DX11RayCastingHashSDF::getColorsImageSRV(), DX11RayCastingHashSDF::getSSAOMapFilteredSRV(), true, true);
				// input color
				//DX11PhongLighting::render(pd3dImmediateContext, DX11RayCastingHashSDF::getPositonsImageSRV(), DX11RayCastingHashSDF::getNormalsImageSRV(), g_Sensor.GetColorSRV(), DX11RayCastingHashSDF::getSSAOMapFilteredSRV(), true, true);
				//DX11QuadDrawer::RenderQuad(pd3dImmediateContext, DX11RayCastingHashSDF::getColorsImageSRV());
			}
			else if (GlobalAppState::getInstance().s_DisplayTexture == 9)
			{
				DX11PhongLighting::render(pd3dImmediateContext, DX11RayCastingHashSDF::getPositonsImageSRV(), DX11RayCastingHashSDF::getNormalsImageSRV(), DX11RayCastingHashSDF::getLabelsImageSRV(), DX11RayCastingHashSDF::getSSAOMapFilteredSRV(), true, true);
				// input label
				//DX11PhongLighting::render(pd3dImmediateContext, DX11RayCastingHashSDF::getPositonsImageSRV(), DX11RayCastingHashSDF::getNormalsImageSRV(), g_Sensor.GetLabelSRV(), DX11RayCastingHashSDF::getSSAOMapFilteredSRV(), true, true);
				//DX11QuadDrawer::RenderQuad(pd3dImmediateContext, DX11RayCastingHashSDF::getLabelsImageSRV());
			}
				
			bool trackingLost = false;
				
			if (GlobalAppState::getInstance().s_RenderMode == RENDERMODE_INTEGRATE && GlobalAppState::getInstance().s_bRegistrationEnabled && hr0 == S_OK)
			{
				mat4f transformation; transformation.setIdentity();
		
				if (g_SceneRepSDFLocal.GetNumIntegratedImages() > 0)
				{
					if (GlobalAppState::getInstance().s_timingsStepsEnabled) 
					{
						GlobalAppState::getInstance().WaitForGPU();
						GlobalAppState::getInstance().s_Timer.start();
					}

					if (GlobalAppState::getInstance().s_usePreComputedCameraTrajectory) 
					{
						transformation = g_Sensor.getRigidTransform();	//should map from the current frame to the base frame 
					} 
					else 
					{
						mat4f deltaEstimate; deltaEstimate.setIdentity();
						
						transformation = DX11CameraTrackingMultiRes::applyCT
							(pd3dImmediateContext,
							g_Sensor.GetDepthFloat4SRV(),
							g_Sensor.GetNormalFloat4SRV(),
							g_Sensor.GetColorSRV(),
							g_Sensor.GetLabelSRV(),
							DX11RayCastingHashSDF::getPositonsImageSRV(),
							DX11RayCastingHashSDF::getNormalsImageSRV(),
							DX11RayCastingHashSDF::getColorsImageSRV(),
							//g_Sensor.GetPreColorSRV(),
							DX11RayCastingHashSDF::getLabelsImageSRV(),
							//g_Sensor.GetPreLabelSRV(),
							g_SceneRepSDFLocal.GetLastRigidTransform(),
							GlobalCameraTrackingState::getInstance().s_maxInnerIter,
							GlobalCameraTrackingState::getInstance().s_maxOuterIter,
							GlobalCameraTrackingState::getInstance().s_distThres,
							GlobalCameraTrackingState::getInstance().s_normalThres,
							GlobalCameraTrackingState::getInstance().s_colorThres,
							GlobalCameraTrackingState::getInstance().s_labelThres,
							100.0f, 3.0f,
							deltaEstimate,
							GlobalAppState::getInstance().getDepthSensor()->getIntrinsics().fx,
							GlobalAppState::getInstance().getDepthSensor()->getIntrinsics().fy,
							GlobalCameraTrackingState::getInstance().s_residualEarlyOut,
							NULL //&g_ICPErrorLog with an error log, it will be much slower since all steps are executed twice)
							);
		 			}

					if (GlobalAppState::getInstance().s_timingsStepsEnabled) 
					{
						GlobalAppState::getInstance().WaitForGPU();
						GlobalAppState::getInstance().s_Timer.stop();
						TimingLog::countTimeTrack++;
						TimingLog::totalTimeTrack += GlobalAppState::getInstance().s_Timer.getElapsedTimeMS();
					}

					if (transformation(0, 0) == -std::numeric_limits<float>::infinity())
					{
						// TODO MADDI do something reasonable here... (also check whether tracking lost is accurate)
						std::cout << "Tracking lost" << std::endl;
						while(1);

						transformation = g_SceneRepSDFLocal.GetLastRigidTransform();
					
						g_SceneRepSDFLocal.Reset(DXUTGetD3D11DeviceContext());
						g_SceneRepSDFGlobal.Reset(DXUTGetD3D11DeviceContext());	
						g_SceneRepChunkGrid.Reset(DXUTGetD3D11DeviceContext());
						g_Camera.Reset();

						return;
					}
				}

				if (GlobalAppState::getInstance().s_DataDumpRigidTransform) 
				{
					std::stringstream ss;	ss << GlobalAppState::getInstance().s_DataDumpPath;
					for (unsigned int i = std::max(1u,g_Sensor.GetFrameNumberDepth()); i < 1000000; i *= 10) ss << "0";
					ss << g_Sensor.GetFrameNumberDepth() << ".matrix";
					std::cout << "Dumping " << ss.str() << std::endl;
					transformation.saveMatrixToFile(ss.str());
				}

				if (GlobalAppState::getInstance().s_RecordData) 
				{
					g_Sensor.recordTrajectory(transformation);
				}

				if (GlobalAppState::getInstance().s_timingsDetailledEnabled) 
				{
					GlobalAppState::getInstance().WaitForGPU();
					GlobalAppState::getInstance().s_Timer.start();
				}

				if (GlobalAppState::getInstance().s_bEnableGlobalLocalStreaming) 
				{
					g_SceneRepSDFLocal.RemoveAndIntegrateToOther(pd3dImmediateContext, &g_SceneRepSDFGlobal, &transformation, true);
					g_SceneRepSDFGlobal.RemoveAndIntegrateToOther(pd3dImmediateContext, &g_SceneRepSDFLocal, &transformation, false);
				}

				if (GlobalAppState::getInstance().s_timingsDetailledEnabled) 
				{
					GlobalAppState::getInstance().WaitForGPU();
					GlobalAppState::getInstance().s_Timer.stop();
					TimingLog::countRemoveAndIntegrate++;
					TimingLog::totalTimeRemoveAndIntegrate += GlobalAppState::getInstance().s_Timer.getElapsedTimeMS();
				}

				vec4f posWorld = transformation*GlobalAppState::getInstance().s_StreamingPos; // trans laggs one frame *trans
				vec3f p(posWorld.x, posWorld.y, posWorld.z);
				
				if (GlobalAppState::getInstance().s_timingsStepsEnabled) 
				{
					GlobalAppState::getInstance().WaitForGPU();
					GlobalAppState::getInstance().s_Timer.start();
				}

				g_SceneRepChunkGrid.StreamOutToCPUPass0GPU(DXUTGetD3D11DeviceContext(), g_SceneRepSDFLocal, p, GlobalAppState::getInstance().s_StreamingRadius, true, true);
				g_SceneRepChunkGrid.StreamInToGPUPass1GPU(DXUTGetD3D11DeviceContext(), g_SceneRepSDFLocal, true);
				
				if (GlobalAppState::getInstance().s_timingsStepsEnabled) 
				{
					GlobalAppState::getInstance().WaitForGPU();
					GlobalAppState::getInstance().s_Timer.stop();
					
					TimingLog::totalTimeMisc += GlobalAppState::getInstance().s_Timer.getElapsedTimeMS();
				}
			
				if (!GlobalAppState::getInstance().s_bDisableIntegration)
				{
					if (GlobalAppState::getInstance().s_timingsStepsEnabled) 
					{
						GlobalAppState::getInstance().WaitForGPU();
						GlobalAppState::getInstance().s_Timer.start();
					}

					g_SceneRepSDFLocal.Integrate(pd3dImmediateContext, g_Sensor.GetDepthFErodedSRV(), g_Sensor.GetColorSRV(), g_Sensor.GetLabelSRV(), g_SceneRepChunkGrid.getBitMask(pd3dImmediateContext), &transformation);

					if (GlobalAppState::getInstance().s_timingsStepsEnabled) 
					{
						GlobalAppState::getInstance().WaitForGPU();
						GlobalAppState::getInstance().s_Timer.stop();
						TimingLog::countTimeSceneUpdate++;
						TimingLog::totalTimeSceneUpdate += GlobalAppState::getInstance().s_Timer.getElapsedTimeMS();
					}
				}
				else
				{
					g_SceneRepSDFLocal.RunCompactifyForView(pd3dImmediateContext);
				}
			}
		}
		else if (GlobalAppState::getInstance().s_DisplayTexture == 1)
		{
			DX11QuadDrawer::RenderQuad(pd3dImmediateContext, g_Sensor.GetHSVDepthFloat4SRV(), 1.0f);
		}
		else if (GlobalAppState::getInstance().s_DisplayTexture == 2)
		{
			DX11QuadDrawer::RenderQuad(pd3dImmediateContext, g_Sensor.GetNormalFloat4SRV());
		}
		else if (GlobalAppState::getInstance().s_DisplayTexture == 4)
		{
			DX11QuadDrawer::RenderQuad(pd3dImmediateContext, g_Sensor.GetColorSRV());
		}
		else if (GlobalAppState::getInstance().s_DisplayTexture == 8)
		{
			DX11QuadDrawer::RenderQuad(pd3dImmediateContext, g_Sensor.GetLabelSRV());
		}

		if (GlobalAppState::getInstance().s_timingsTotalEnabled) 
		{
			GlobalAppState::getInstance().WaitForGPU();
			GlobalAppState::getInstance().s_Timer.stop();

			if (hr0 == S_OK)
			{
				double elapsedTime = GlobalAppState::getInstance().s_Timer.getElapsedTimeMS();
				
				TimingLog::totalTimeAllAvgArray[TimingLog::countTotalTimeAll % BENCHMARK_SAMPLES] = elapsedTime;
				TimingLog::totalTimeAllWorst = std::max(elapsedTime, TimingLog::totalTimeAllWorst);
				TimingLog::totalTimeAll += elapsedTime;
				TimingLog::totalTimeSquaredAll += (elapsedTime*elapsedTime);
				TimingLog::countTotalTimeAll = (TimingLog::countTotalTimeAll + 1);
			}
		}

		if (hr0 == S_OK && GlobalAppState::getInstance().s_DumpAllRendering) 
		{
			DumpAllRendering(pd3dImmediateContext);
		}
	}

	if (GlobalAppState::getInstance().s_DumpVoxelGridFrames > 0 && (g_Sensor.GetFrameNumberDepth() % GlobalAppState::getInstance().s_DumpVoxelGridFrames) == 0) 
	{	
		std::cout << "dumping voxel grid... ";
		vec4f pos = g_SceneRepSDFLocal.GetLastRigidTransform()*GlobalAppState::getInstance().s_StreamingPos;
		
		g_SceneRepSDFLocal.DumpHashToDisk(GlobalAppState::getInstance().s_DumpVoxelGridFile + std::to_string(g_Sensor.GetFrameNumberDepth()) + ".dump", 
			                              GlobalAppState::getInstance().s_StreamingRadius - sqrt(3.0f), pos.getPoint3d());
		
		std::cout << "done!" << std::endl;
	}

	TimingLog::printTimings();
		
	if (true)
	{	
		DXUT_BeginPerfEvent( DXUT_PERFEVENTCOLOR, L"HUD / Stats" );
		g_HUD.OnRender( fElapsedTime );
		g_SampleUI.OnRender( fElapsedTime );
		RenderText();
		DXUT_EndPerfEvent();
	}
}