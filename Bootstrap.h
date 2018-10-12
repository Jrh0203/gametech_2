/*
-----------------------------------------------------------------------------
Filename:    BaseApplication.h
-----------------------------------------------------------------------------

This source file is part of the
   ___                 __    __ _ _    _
  /___\__ _ _ __ ___  / / /\ \ (_) | _(_)
 //  // _` | '__/ _ \ \ \/  \/ / | |/ / |
/ \_// (_| | | |  __/  \  /\  /| |   <| |
\___/ \__, |_|  \___|   \/  \/ |_|_|\_\_|
      |___/
Tutorial Framework (for Ogre 1.9)
http://www.ogre3d.org/wiki/
-----------------------------------------------------------------------------
*/

#ifndef __Bootstrap_h_
#define __Bootstrap_h_

#include <string>
#include <sstream>

#include <OgreEntity.h>
#include <OgreCamera.h>
#include <OgreLogManager.h>
#include <OgreRoot.h>
#include <OgreViewport.h>
#include <OgreSceneManager.h>
#include <OgreRenderWindow.h>
#include <OgreConfigFile.h> 

#include <OgreWindowEventUtilities.h>
#include <OgreFrameListener.h>
#include <Overlay/OgreOverlaySystem.h>

#include "btBulletCollisionCommon.h"
#include "btBulletDynamicsCommon.h"

//#include "Object.cpp"
#include "Paddle.cpp"
#include "Ball.cpp"
#include "Wall.cpp"

#if OGRE_PLATFORM == OGRE_PLATFORM_APPLE
#  include <OIS/OISEvents.h>
#  include <OIS/OISInputManager.h>
#  include <OIS/OISKeyboard.h>
#  include <OIS/OISMouse.h>

#else
#  include <OISEvents.h>
#  include <OISInputManager.h>
#  include <OISKeyboard.h>
#  include <OISMouse.h>

//CEGUI STUFF
#include <CEGUI/CEGUI.h>
#include <CEGUI/RendererModules/Ogre/Renderer.h>

#include "SDL/SDL.h"
#include "SDL/SDL_mixer.h"

#endif

#ifdef OGRE_STATIC_LIB
#  define OGRE_STATIC_GL
#  if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
#    define OGRE_STATIC_Direct3D9
// D3D10 will only work on vista, so be careful about statically linking
#    if OGRE_USE_D3D10
#      define OGRE_STATIC_Direct3D10
#    endif
#  endif
#  define OGRE_STATIC_BSPSceneManager
#  define OGRE_STATIC_ParticleFX
#  define OGRE_STATIC_CgProgramManager
#  ifdef OGRE_USE_PCZ
#    define OGRE_STATIC_PCZSceneManager
#    define OGRE_STATIC_OctreeZone
#  else
#    define OGRE_STATIC_OctreeSceneManager
#  endif
#  include "OgreStaticPluginLoader.h"
#endif

//---------------------------------------------------------------------------

struct OpponentWallCallback;
struct PlayerWallCallback;
struct Paddle1Callback;
struct Paddle2Callback;

class TutorialApplication : 
    public Ogre::FrameListener, public Ogre::WindowEventListener, 
    public OIS::KeyListener, public OIS::MouseListener
{
public:
    TutorialApplication(void);
    ~TutorialApplication(void);

    void go(void);
    void updateScore(int player);
    void checkColor(int player);


    Mix_Chunk *wBounce;
    Mix_Chunk *wPaddleHit;
    bool soundEnabled;

    Paddle *paddle1;
    Paddle *paddle2;
    Wall **walls;
    Ball *ball;

    int ballTrigger = -1;

protected:
    bool configure(void);
    void chooseSceneManager(void);
    void createCamera(void);
    void createFrameListener(void);
    void createScene(void); 
    void destroyScene(void);
    void createViewports(void);
    void setupResources(void);
    void loadResources(void);
    bool frameRenderingQueued(const Ogre::FrameEvent& evt);
    void setupGUI();

    bool keyPressed(const OIS::KeyEvent &arg);
    bool keyReleased(const OIS::KeyEvent &arg);
    void createBulletSim(void);
    bool mouseMoved(const OIS::MouseEvent &arg);
    bool mousePressed(const OIS::MouseEvent &arg, OIS::MouseButtonID id);
    bool mouseReleased(const OIS::MouseEvent &arg, OIS::MouseButtonID id);
    void checkCollisions(void);

    void newGame(void);
    void pauseGame(void);
    void resumeGame(void);
    void reset(void);
    bool setup(void);


    void startFireworks();
    void explode(const Ogre::Vector3);


    bool quit(const CEGUI::EventArgs& e);

    std::string getScoreBoardText(void);
    bool setupSDL(void);
    void switchSound(void);


    // Adjust mouse clipping area
    void windowResized(Ogre::RenderWindow* rw);
    // Unattach OIS before window shutdown (very important under Linux)
    void windowClosed(Ogre::RenderWindow* rw);

    Ogre::Root*                 mRoot;
    Ogre::Camera*               mCamera;
    Ogre::SceneNode*            mCamNode;
    Ogre::SceneManager*         mSceneMgr;
    Ogre::RenderWindow*         mWindow;
    Ogre::String                mResourcesCfg;
    Ogre::String                mPluginsCfg;

    Ogre::OverlaySystem*        mOverlaySystem;

    bool                        mCursorWasVisible;	// Was cursor visible before dialog appeared?
    bool                        mShutDown;

    //OIS Input devices
    OIS::InputManager*          mInputManager;
    OIS::Mouse*                 mMouse;
    OIS::Keyboard*              mKeyboard;

    // Added for Mac compatibility
    Ogre::String                 m_ResourcePath;

    bool* keys;
    bool fireworksOn;
    Ogre::ParticleSystem* sunParticle;
    Ogre::ParticleSystem* ballParticle;

    CEGUI::MouseButton convertButton(OIS::MouseButtonID buttonID);

    //physics
    btDefaultCollisionConfiguration* collisionConfiguration;
    btCollisionDispatcher* dispatcher;
    btBroadphaseInterface* overlappingPairCache;
    btSequentialImpulseConstraintSolver* solver;
    btDiscreteDynamicsWorld* dynamicsWorld;
    btCollisionShape* groundShape;
    btAlignedObjectArray<btCollisionShape*> collisionShapes;

    //game objects
    

    //score
    int rounds;
    int playerScore;
    int opponentScore;

    bool gameRunning;

    //GUI
    CEGUI::OgreRenderer* mRenderer;

    CEGUI::Window *menuSheet;
    CEGUI::Window *gameSheet;
    CEGUI::Window *pauseSheet;
    CEGUI::Window *restartSheet;
    CEGUI::Window *scoreBoard;
    CEGUI::Window *sound;
    CEGUI::Window *victoryText;

    PlayerWallCallback* pwcb;
    OpponentWallCallback* owcb;
    Paddle1Callback* p1cb;
    Paddle2Callback* p2cb;

    Mix_Chunk *wExplode;
    Mix_Music *wBGM;



#ifdef OGRE_STATIC_LIB
    Ogre::StaticPluginLoader m_StaticPluginLoader;
#endif
};


//---------------------------------------------------------------------------

#endif // #ifndef __Bootstrap_h_

//---------------------------------------------------------------------------
