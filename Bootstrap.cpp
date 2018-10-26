/*-------------------------------------------------------------------------
This source file is a part of OGRE
(Object-oriented Graphics Rendering Engine)
For the latest info, see http://www.ogre3d.org/


Copyright (c) 2000-2013 Torus Knot Software Ltd
Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:


The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.


THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE
-------------------------------------------------------------------------*/

//! [starter]

#include <exception>
#include "Bootstrap.h"
#include <iostream>
#include <stdio.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <netdb.h>
#include <sys/uio.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <fstream>
#include <chrono>
#include <thread>

using namespace std;

struct packet;

struct ScoreCallback : public btCollisionWorld::ContactResultCallback
{ 
    ScoreCallback(TutorialApplication* ptr, int i) : context(ptr) {
    player = i;
}
    btScalar addSingleResult(btManifoldPoint& cp,
        const btCollisionObjectWrapper* colObj0Wrap,
        int partId0,
        int index0,
        const btCollisionObjectWrapper* colObj1Wrap,
        int partId1,
        int index1)
    {
        // if (context->soundEnabled){
        //     Mix_PlayChannel(-1, context->wBounce, 0);
        //    // context->ballTrigger = 3;
        // }
        context->updateScore(player);
    }

    TutorialApplication* context;
    int player;
};

struct PaddleCallback : public btCollisionWorld::ContactResultCallback
{
    PaddleCallback(TutorialApplication* ptr, int i) : context(ptr) {
        paddle = i;
    }

    btScalar addSingleResult(btManifoldPoint& cp,
        const btCollisionObjectWrapper* colObj0Wrap,
        int partId0,
        int index0,
        const btCollisionObjectWrapper* colObj1Wrap,
        int partId1,
        int index1)
    {
        if (context->soundEnabled){
            Mix_PlayChannel(-1, context->wPaddleHit, 0);
        }
        context->ballTrigger = context->delay;
        context->checkColor(paddle);
    }

    TutorialApplication* context;
    int paddle;
};

struct SideWallCallback : public btCollisionWorld::ContactResultCallback
{

    SideWallCallback(TutorialApplication* ptr) : context(ptr) {}

    btScalar addSingleResult(btManifoldPoint& cp,
        const btCollisionObjectWrapper* colObj0Wrap,
        int partId0,
        int index0,
        const btCollisionObjectWrapper* colObj1Wrap,
        int partId1,
        int index1)
    {
        
        if (context->collisionTimer==0){
            if(context->soundEnabled){
                Mix_PlayChannel(-1, context->wBounce, 0);
            }
            /*
            Ogre::Vector3 pos = context->ball->node->getPosition();
            btVector3 vel = context->ball->body->getLinearVelocity();
            Ogre::Vector3 vel2 = Ogre::Vector3(vel[0],vel[1],vel[2]);
            vel2 = vel2.normalise();
            Ogre::Vector3 pos2 = pos+vel2*.05;
            context->clang(pos2);
            */
            context->collisionTimer = 500;
        }
        

    }
    

    TutorialApplication* context;
};




TutorialApplication::TutorialApplication()
   : mRoot(0),
    mCamera(0),
    mSceneMgr(0),
    mWindow(0),
    mResourcesCfg(Ogre::StringUtil::BLANK),
    mPluginsCfg(Ogre::StringUtil::BLANK),
    mCursorWasVisible(false),
    mShutDown(false),
    mInputManager(0),
    mMouse(0),
    mKeyboard(0),
    mOverlaySystem(0)
{
  keys = new bool[8];

  playerScore = 0;
  opponentScore = 0;
  gameRunning = true;
  soundEnabled=true;
  musicEnabled=true;
  fireworksOn=false;

  delay = 15;
  collisionTimer = 0;

#if OGRE_PLATFORM == OGRE_PLATFORM_APPLE
    m_ResourcePath = Ogre::macBundlePath() + "/Contents/Resources/";
#else
    m_ResourcePath = "";
#endif
}

TutorialApplication::~TutorialApplication()
{
    //free sdl sounds
    Mix_FreeChunk( wBounce);
    Mix_FreeChunk( wExplode);
    wBounce = NULL;
    wExplode = NULL;
    Mix_Quit();
    SDL_Quit();
}


bool TutorialApplication::configure(void)
{
    // Show the configuration dialog and initialise the system.
    // You can skip this and use root.restoreConfig() to load configuration
    // settings if you were sure there are valid ones saved in ogre.cfg.
    if(mRoot->showConfigDialog())
    {
        // If returned true, user clicked OK so initialise.
        // Here we choose to let the system create a default rendering window by passing 'true'.
        mWindow = mRoot->initialise(true, "TutorialApplication Render Window");


        return true;
    }
    else
    {
        return false;
    }
}

void TutorialApplication::createBulletSim(){
          //Initialize all the physics objects
        //set up all the necessary collision objects
        collisionConfiguration = new btDefaultCollisionConfiguration();
        ///use the default collision dispatcher. For parallel processing you can use a diffent dispatcher (see Extras/BulletMultiThreaded)
        dispatcher = new    btCollisionDispatcher(collisionConfiguration);
        ///btDbvtBroadphase is a good general purpose broadphase. You can also try out btAxis3Sweep.
        overlappingPairCache = new btDbvtBroadphase();
        ///the default constraint solver. For parallel processing you can use a different solver (see Extras/BulletMultiThreaded)
        solver = new btSequentialImpulseConstraintSolver;
        dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher,overlappingPairCache,solver,collisionConfiguration);

        dynamicsWorld->setGravity(btVector3(0, 0, 0));
}

void TutorialApplication::createScene()
    {
        //mSceneMgr->setSkyDome(true, "paddle/CloudySky", 5, 8);

        paddle1 = new Paddle(mSceneMgr, btVector3(0, 0, 40), true);
        collisionShapes.push_back(paddle1->getCollisionShape());
        dynamicsWorld->addRigidBody(paddle1->getRigidBody());

        paddle2 = new Paddle(mSceneMgr, btVector3(0, 0, -40), false);
        collisionShapes.push_back(paddle2->getCollisionShape());
        dynamicsWorld->addRigidBody(paddle2->getRigidBody());
   
        //make ball
        ball = new Ball(mSceneMgr, btVector3(0, 0, 0));
        collisionShapes.push_back(ball->getCollisionShape());
        //ballNode = ball->getNode();
        dynamicsWorld->addRigidBody(ball->getRigidBody());


        //make all the walls, code could probably be prettier lol 
        walls = new Wall*[4];
        //make ground
        walls[0] = new Wall(mSceneMgr, btVector3(0,-56,0), 0, 0, 0);
        collisionShapes.push_back(walls[0]->getCollisionShape());
        dynamicsWorld->addRigidBody(walls[0]->getRigidBody());

        walls[1] = new Wall(mSceneMgr, btVector3(50, -6, 0),0, 0, 90);
        collisionShapes.push_back(walls[1]->getCollisionShape());
        dynamicsWorld->addRigidBody(walls[1]->getRigidBody());

        walls[2] = new Wall(mSceneMgr, btVector3(-50, -6, 0), 0, 0, -90);
        collisionShapes.push_back(walls[2]->getCollisionShape());
        dynamicsWorld->addRigidBody(walls[2]->getRigidBody());

        walls[3] = new Wall(mSceneMgr, btVector3(0, 44, 0), 0, 0, 180);
        collisionShapes.push_back(walls[3]->getCollisionShape());
        dynamicsWorld->addRigidBody(walls[3]->getRigidBody());

        walls[4] = new Wall(mSceneMgr, btVector3(0, 0, -50), 0, -90, 0);
        collisionShapes.push_back(walls[4]->getCollisionShape());
        dynamicsWorld->addRigidBody(walls[4]->getRigidBody());

        walls[5] = new Wall(mSceneMgr, btVector3(0, 0, 50), 0, 90, 0);
        collisionShapes.push_back(walls[5]->getCollisionShape());
        dynamicsWorld->addRigidBody(walls[5]->getRigidBody());

        pwcb = new ScoreCallback(this, 1);
        owcb = new ScoreCallback(this, 0);
        p1cb = new PaddleCallback(this, 1);
        p2cb = new PaddleCallback(this, 0);
        swcb = new SideWallCallback(this);
  }


void TutorialApplication::setupResources(void)
{
    // Load resource paths from config file
    Ogre::ConfigFile cf;
    cf.load(mResourcesCfg);

    // Go through all sections & settings in the file
    Ogre::ConfigFile::SectionIterator seci = cf.getSectionIterator();

    Ogre::String secName, typeName, archName;
    while (seci.hasMoreElements())
    {
        secName = seci.peekNextKey();
        Ogre::ConfigFile::SettingsMultiMap *settings = seci.getNext();
        Ogre::ConfigFile::SettingsMultiMap::iterator i;
        for (i = settings->begin(); i != settings->end(); ++i)
        {
            typeName = i->first;
            archName = i->second;

#if OGRE_PLATFORM == OGRE_PLATFORM_APPLE
            // OS X does not set the working directory relative to the app.
            // In order to make things portable on OS X we need to provide
            // the loading with it's own bundle path location.
            if (!Ogre::StringUtil::startsWith(archName, "/", false)) // only adjust relative directories
                archName = Ogre::String(Ogre::macBundlePath() + "/" + archName);
#endif

            Ogre::ResourceGroupManager::getSingleton().addResourceLocation(
                archName, typeName, secName);
        }
    }

    Ogre::ResourceGroupManager::getSingleton().addResourceLocation("./materials", "FileSystem");
}

void TutorialApplication::chooseSceneManager(void)
{
    // Get the SceneManager, in this case a generic one
    mSceneMgr = mRoot->createSceneManager(Ogre::ST_GENERIC);

    // Initialize the OverlaySystem (changed for Ogre 1.9)
    mOverlaySystem = new Ogre::OverlaySystem();
    mSceneMgr->addRenderQueueListener(mOverlaySystem);
}

void TutorialApplication::createCamera(void)
{
    mCamera = mSceneMgr->createCamera("PlayerCam");
    mCamNode = mSceneMgr->getRootSceneNode()->createChildSceneNode();
    mCamNode->attachObject(mCamera);

    // Position it at 500 in Z direction
    mCamNode->setPosition(0, 0, 75);
    // Look back along -Z
    mCamera->setNearClipDistance(5);
}

void TutorialApplication::createViewports(void)
{
    // Create one viewport, entire window
    Ogre::Viewport* vp = mWindow->addViewport(mCamera);
    vp->setBackgroundColour(Ogre::ColourValue(0,0,0));

    // Alter the camera aspect ratio to match the viewport
    mCamera->setAspectRatio(Ogre::Real(vp->getActualWidth()) / Ogre::Real(vp->getActualHeight()));
}

void TutorialApplication::loadResources(void)
{
    std::cout << "Loading resources" <<std::endl;
    Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();
    std::cout << "initialized all resource groups" <<std::endl;
}

void TutorialApplication::createFrameListener(void)
{
    Ogre::LogManager::getSingletonPtr()->logMessage("*** Initializing OIS ***");
    OIS::ParamList pl;
    size_t windowHnd = 0;
    std::ostringstream windowHndStr;

    mWindow->getCustomAttribute("WINDOW", &windowHnd);
    windowHndStr << windowHnd;
    pl.insert(std::make_pair(std::string("WINDOW"), windowHndStr.str()));

    mInputManager = OIS::InputManager::createInputSystem(pl);

    mKeyboard = static_cast<OIS::Keyboard*>(mInputManager->createInputObject(OIS::OISKeyboard, true));
    mMouse = static_cast<OIS::Mouse*>(mInputManager->createInputObject(OIS::OISMouse, true));

    mMouse->setEventCallback(this);
    mKeyboard->setEventCallback(this);

    // Set initial mouse clipping size
    windowResized(mWindow);

    // Register as a Window listener
    Ogre::WindowEventUtilities::addWindowEventListener(mWindow, this);

    mRoot->addFrameListener(this);
}

void TutorialApplication::setupGUI(){
        //this function could be a lot prettier but I don't feel like rewriting it
        mRenderer = &CEGUI::OgreRenderer::bootstrapSystem();


        CEGUI::ImageManager::setImagesetDefaultResourceGroup("Imagesets");
        CEGUI::Font::setDefaultResourceGroup("Fonts");
        CEGUI::Scheme::setDefaultResourceGroup("Schemes");
        CEGUI::WidgetLookManager::setDefaultResourceGroup("LookNFeel");
        CEGUI::WindowManager::setDefaultResourceGroup("Layouts");

        CEGUI::SchemeManager::getSingleton().createFromFile("TaharezLook.scheme");
        CEGUI::System::getSingleton().getDefaultGUIContext().getMouseCursor().setDefaultImage("TaharezLook/MouseArrow");
        
        
        CEGUI::WindowManager &wmgr = CEGUI::WindowManager::getSingleton();
        gameSheet = wmgr.createWindow("DefaultWindow", "CEGUIDemo/Sheet");
        menuSheet = wmgr.createWindow("DefaultWindow", "CEGUIDemo/Sheet");
        selectGameSheet = wmgr.createWindow("DefaultWindow", "CEGUIDemo/Sheet");
        pauseSheet = wmgr.createWindow("DefaultWindow", "CEGUIDemo/Sheet");
        restartSheet = wmgr.createWindow("DefaultWindow", "CEGUIDemo/Sheet");


        //quit button-gois in ingame GUI and menu GUI
        CEGUI::Window *gameQuit = wmgr.createWindow("TaharezLook/Button", "CEGUIDemo/QuitButton");
        gameQuit->setText("Quit");
        gameQuit->setPosition(CEGUI::UVector2(CEGUI::UDim(0,0),CEGUI::UDim(0,0)));
        gameQuit->setSize(CEGUI::USize(CEGUI::UDim(0.15, 0), CEGUI::UDim(0.05, 0)));
        gameSheet->addChild(gameQuit);
        gameQuit->subscribeEvent(CEGUI::PushButton::EventClicked, 
        CEGUI::SubscriberSlot(&TutorialApplication::quit, this));

        CEGUI::Window *menuQuit = wmgr.createWindow("TaharezLook/Button", "CEGUIDemo/QuitButton");
        menuQuit->setText("Quit");
        menuQuit->setPosition(CEGUI::UVector2(CEGUI::UDim(0,0),CEGUI::UDim(0,0)));
        menuQuit->setSize(CEGUI::USize(CEGUI::UDim(0.15, 0), CEGUI::UDim(0.05, 0)));
        menuSheet->addChild(menuQuit);
        //CEGUI::System::getSingleton().getDefaultGUIContext().setRootWindow(sheet);

        menuQuit->subscribeEvent(CEGUI::PushButton::EventClicked, 
        CEGUI::SubscriberSlot(&TutorialApplication::quit, this));

        CEGUI::Window *pauseQuit = wmgr.createWindow("TaharezLook/Button", "CEGUIDemo/QuitButton");
        pauseQuit->setText("Quit");
        menuQuit->setPosition(CEGUI::UVector2(CEGUI::UDim(0,0),CEGUI::UDim(0,0)));
        pauseQuit->setSize(CEGUI::USize(CEGUI::UDim(0.15, 0), CEGUI::UDim(0.05, 0)));
        pauseSheet->addChild(pauseQuit);
        //CEGUI::System::getSingleton().getDefaultGUIContext().setRootWindow(sheet);

        pauseQuit->subscribeEvent(CEGUI::PushButton::EventClicked, 
        CEGUI::SubscriberSlot(&TutorialApplication::quit, this));


        scoreBoard = wmgr.createWindow("TaharezLook/Button", "CEGUIDemo/QuitButton");
        scoreBoard->setPosition(CEGUI::UVector2(CEGUI::UDim(0.85,0),CEGUI::UDim(0,0)));
        scoreBoard->setSize(CEGUI::USize(CEGUI::UDim(0.15, 0), CEGUI::UDim(0.15, 0)));
        scoreBoard->setText(getScoreBoardText());
        gameSheet->addChild(scoreBoard);
        //CEGUI::System::getSingleton().getDefaultGUIContext().setRootWindow(sheet);

        CEGUI::Window *pause = wmgr.createWindow("TaharezLook/Button", "CEGUIDemo/QuitButton");
        pause->setPosition(CEGUI::UVector2(CEGUI::UDim(0,0),CEGUI::UDim(0.05,0)));
        pause->setSize(CEGUI::USize(CEGUI::UDim(0.15, 0), CEGUI::UDim(0.05, 0)));
        pause->setText("Pause");
        pause->subscribeEvent(CEGUI::PushButton::EventClicked, 
        CEGUI::SubscriberSlot(&TutorialApplication::pauseGame, this));
        gameSheet->addChild(pause);


        CEGUI::Window *start = wmgr.createWindow("TaharezLook/Button", "CEGUIDemo/QuitButton");
        start->setPosition(CEGUI::UVector2(CEGUI::UDim(0.40,0),CEGUI::UDim(0.45,0)));
        start->setSize(CEGUI::USize(CEGUI::UDim(0.20, 0), CEGUI::UDim(0.10, 0)));
        start->setText("Start");
        start->subscribeEvent(CEGUI::PushButton::EventClicked, 
        CEGUI::SubscriberSlot(&TutorialApplication::selectGameType, this));

        menuSheet->addChild(start);

        CEGUI::Window *singleplayer = wmgr.createWindow("TaharezLook/Button", "CEGUIDemo/QuitButton");
        singleplayer->setPosition(CEGUI::UVector2(CEGUI::UDim(0.40,0),CEGUI::UDim(0.20,0)));
        singleplayer->setSize(CEGUI::USize(CEGUI::UDim(0.20, 0), CEGUI::UDim(0.10, 0)));
        singleplayer->setText("Singleplayer");
        singleplayer->subscribeEvent(CEGUI::PushButton::EventClicked, 
        CEGUI::SubscriberSlot(&TutorialApplication::newGame, this));
        selectGameSheet->addChild(singleplayer);
        
        CEGUI::Window *host= wmgr.createWindow("TaharezLook/Button", "CEGUIDemo/QuitButton");
        host->setPosition(CEGUI::UVector2(CEGUI::UDim(0.40,0),CEGUI::UDim(0.35,0)));
        host->setSize(CEGUI::USize(CEGUI::UDim(0.20, 0), CEGUI::UDim(0.10, 0)));
        host->setText("Host Game");
        host->subscribeEvent(CEGUI::PushButton::EventClicked, 
        CEGUI::SubscriberSlot(&TutorialApplication::hostGame, this));
        selectGameSheet->addChild(host);
        
        CEGUI::Window *join = wmgr.createWindow("TaharezLook/Button", "CEGUIDemo/QuitButton");
        join->setPosition(CEGUI::UVector2(CEGUI::UDim(0.40,0),CEGUI::UDim(0.50,0)));
        join->setSize(CEGUI::USize(CEGUI::UDim(0.20, 0), CEGUI::UDim(0.10, 0)));
        join->setText("Join Game");
        join->subscribeEvent(CEGUI::PushButton::EventClicked, 
        CEGUI::SubscriberSlot(&TutorialApplication::enterIP, this));
        selectGameSheet->addChild(join);

        //edit box to get entered IP address
        joinIP = static_cast<CEGUI::Editbox*>(wmgr.createWindow("TaharezLook/Editbox", "CEGUIDemo/QuitButton"));
        joinIP->setPosition(CEGUI::UVector2(CEGUI::UDim(0.40,0),CEGUI::UDim(0.60,0)));
        joinIP->setSize(CEGUI::USize(CEGUI::UDim(0.20, 0), CEGUI::UDim(0.10, 0)));
        //joinIP->setMaxTextLength((size_t)8);
        joinIP->setText("Enter server name");
        joinIP->setReadOnly(false);
        joinIP->setVisible(false);
        joinIP->subscribeEvent(CEGUI::Editbox::EventTextAccepted, 
            CEGUI::Event::Subscriber(&TutorialApplication::joinGame,this));
        selectGameSheet->addChild(joinIP);


        CEGUI::Window *resume = wmgr.createWindow("TaharezLook/Button", "CEGUIDemo/QuitButton");
        resume->setPosition(CEGUI::UVector2(CEGUI::UDim(0.40,0),CEGUI::UDim(0.45,0)));
        resume->setSize(CEGUI::USize(CEGUI::UDim(0.20, 0), CEGUI::UDim(0.10, 0)));
        resume->setText("Resume");
        resume->subscribeEvent(CEGUI::PushButton::EventClicked, 
        CEGUI::SubscriberSlot(&TutorialApplication::resumeGame, this));

        pauseSheet->addChild(resume);

        sound = wmgr.createWindow("TaharezLook/Button", "CEGUIDemo/QuitButton");
        sound->setPosition(CEGUI::UVector2(CEGUI::UDim(0,0),CEGUI::UDim(0.05,0)));
        sound->setSize(CEGUI::USize(CEGUI::UDim(0.15, 0), CEGUI::UDim(0.05, 0)));
        sound->setText("Sound: ON");
        sound->subscribeEvent(CEGUI::PushButton::EventClicked, 
        CEGUI::SubscriberSlot(&TutorialApplication::switchSound, this));
        pauseSheet->addChild(sound);

        bgm = wmgr.createWindow("TaharezLook/Button", "CEGUIDemo/QuitButton");
        bgm->setPosition(CEGUI::UVector2(CEGUI::UDim(0,0),CEGUI::UDim(0.10,0)));
        bgm->setSize(CEGUI::USize(CEGUI::UDim(0.15, 0), CEGUI::UDim(0.05, 0)));
        bgm->setText("Music: ON");
        bgm->subscribeEvent(CEGUI::PushButton::EventClicked, 
        CEGUI::SubscriberSlot(&TutorialApplication::switchMusic, this));
        pauseSheet->addChild(bgm);

        CEGUI::Window *restartQuit = wmgr.createWindow("TaharezLook/Button", "CEGUIDemo/QuitButton");
        restartQuit->setText("Quit");
        restartQuit->setPosition(CEGUI::UVector2(CEGUI::UDim(0,0),CEGUI::UDim(0,0)));
        restartQuit->setSize(CEGUI::USize(CEGUI::UDim(0.15, 0), CEGUI::UDim(0.05, 0)));
        restartSheet->addChild(restartQuit);
        restartQuit->subscribeEvent(CEGUI::PushButton::EventClicked, 
            CEGUI::SubscriberSlot(&TutorialApplication::quit, this));

        victoryText = wmgr.createWindow("TaharezLook/Button", "CEGUIDemo/QuitButton");
        victoryText->setPosition(CEGUI::UVector2(CEGUI::UDim(0.35, 0), CEGUI::UDim(0.30,0)));
        victoryText->setSize(CEGUI::USize(CEGUI::UDim(0.3, 0), CEGUI::UDim(0.1,0)));
        restartSheet->addChild(victoryText);

        CEGUI::Window *playAgain = wmgr.createWindow("TaharezLook/Button", "CEGUIDemo/QuitButton");
        playAgain->setText("Play Again?");
        playAgain->setPosition(CEGUI::UVector2(CEGUI::UDim(0.4,0),CEGUI::UDim(0.45,0)));
        playAgain->setSize(CEGUI::USize(CEGUI::UDim(0.2, 0), CEGUI::UDim(0.1, 0)));
        restartSheet->addChild(playAgain);
        playAgain->subscribeEvent(CEGUI::PushButton::EventClicked, 
            CEGUI::SubscriberSlot(&TutorialApplication::newGame, this));


        CEGUI::System::getSingleton().getDefaultGUIContext().setRootWindow(menuSheet);
        gameRunning = false;
}

bool TutorialApplication::setupSDL(){
    bool success = true;

    //code below modified from Lazy Foo productions http://lazyfoo.net/tutorials/SDL/21_sound_effects_and_music/index.php

    //Initialize SDL 
    if( SDL_Init( SDL_INIT_VIDEO | SDL_INIT_AUDIO ) < 0 ) { 
        printf( "SDL could not initialize! SDL Error: %s\n", SDL_GetError() ); 
        success = false; 
    }

    //initialize mixer
    if( Mix_OpenAudio( 44100, MIX_DEFAULT_FORMAT, 2, 2048 ) < 0 ) { 
        printf( "SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError() ); 
        success = false; 
    }

    //load sounds
    wBounce = Mix_LoadWAV( "sounds/boing.wav" ); 
    if( wBounce == NULL ) { 
        printf( "Failed to load scratch sound effect! SDL_mixer Error: %s\n", Mix_GetError() ); 
        success = false;
    }

    //Mix_VolumeChunk(wBounce, 128);

    wExplode = Mix_LoadWAV( "sounds/explosion.wav" ); 
    if( wExplode == NULL ) { 
        printf( "Failed to load scratch sound effect! SDL_mixer Error: %s\n", Mix_GetError() ); 
        success = false;
    }

    wPaddleHit= Mix_LoadWAV( "sounds/woodblock.wav" ); 
    if( wPaddleHit == NULL ) { 
        printf( "Failed to load scratch sound effect! SDL_mixer Error: %s\n", Mix_GetError() ); 
        success = false;
    }

    //Phillip Gross-eRHoLuNG,found on freemusicarchive.org so should be ok?
    wBGM = Mix_LoadMUS( "sounds/Phillip_Gross_-_03_-_eRHoLuNG.wav" ); 
    if( wBGM == NULL ) { 
        printf( "Failed to load music! SDL_mixer Error: %s\n", Mix_GetError() ); 
        success = false; 
    } 

    wVictory = Mix_LoadWAV( "sounds/player_win.wav" ); 
    if( wVictory == NULL ) { 
        printf( "Failed to load music! SDL_mixer Error: %s\n", Mix_GetError() ); 
        success = false; 
    } 

    //start music
    Mix_PlayMusic( wBGM, -1 );

    return success;
}

void TutorialApplication::switchSound(){

    soundEnabled = !soundEnabled;
    std::string enabled = soundEnabled ? "ON" : "OFF";
    sound->setText("Sound: " + enabled);
}

void TutorialApplication::switchMusic(){
    musicEnabled = !musicEnabled;
    std::string enabled = musicEnabled ? "ON" : "OFF";
    bgm->setText("Music: " + enabled);

    if (musicEnabled){
        Mix_ResumeMusic();
    } else {
         Mix_PauseMusic();
    }
}

void TutorialApplication::startFireworks(){
    if (fireworksOn==false){
        //sunParticle->clear();
        mSceneMgr->destroyParticleSystem("Sun");
        sunParticle = mSceneMgr->createParticleSystem("Sun", "Space/Sun");
        Ogre::SceneNode* particleNode = mSceneMgr->getRootSceneNode()->createChildSceneNode("Fireworks");
        particleNode->attachObject(sunParticle);
        particleNode->setPosition(0,-40,0);
        fireworksOn = true;
    } else {
        Ogre::SceneNode* itemNode = mSceneMgr->getSceneNode("Fireworks");
        //destroyAllAttachedMovableObjects(itemNode);
        itemNode->removeAndDestroyAllChildren();
        mSceneMgr->destroySceneNode(itemNode);
        fireworksOn = false;
    }
    
}

void TutorialApplication::explode(Ogre::Vector3 pos){
    if (mSceneMgr->hasSceneNode("BallExplode"))
    {
        Ogre::SceneNode* itemNode = mSceneMgr->getSceneNode("BallExplode");
        itemNode->removeAndDestroyAllChildren();
        mSceneMgr->destroySceneNode(itemNode);    
        if (soundEnabled){
            Mix_PlayChannel(-1, wExplode, 0);
        }
    }
    
    

    mSceneMgr->destroyParticleSystem("Explode");
    ballParticle = mSceneMgr->createParticleSystem("Explode", "OOB");
    Ogre::SceneNode* particleNode = mSceneMgr->getRootSceneNode()->createChildSceneNode("BallExplode");
    particleNode->attachObject(ballParticle);

    particleNode->setPosition(pos.x,pos.y, pos.z);
}

void TutorialApplication::clang(Ogre::Vector3 pos){
    if (mSceneMgr->hasSceneNode("BallClang"))
    {
        Ogre::SceneNode* itemNode = mSceneMgr->getSceneNode("BallClang");
        itemNode->removeAndDestroyAllChildren();
        mSceneMgr->destroySceneNode(itemNode);    
    }
    
    mSceneMgr->destroyParticleSystem("Clang");
    ballParticle = mSceneMgr->createParticleSystem("Clang", "Clang");
    Ogre::SceneNode* particleNode = mSceneMgr->getRootSceneNode()->createChildSceneNode("BallClang");
    particleNode->attachObject(clangParticle);

    particleNode->setPosition(pos.x,pos.y, pos.z);
}


bool TutorialApplication::setup()
{

    // do not forget to call the base first
    for (int i = 0; i < 6; i++){
        keys[i]=false;
    }
    //ApplicationContext::setup();
    //addInputListener(this);

    // get a pointer to the already created root
    mRoot = new Ogre::Root(mPluginsCfg);

    bool carryOn = configure();
      if (!carryOn) return false;

    setupResources();
    chooseSceneManager();
    createCamera();
    createViewports();
    Ogre::TextureManager::getSingleton().setDefaultNumMipmaps(5);

    //createResourceListener();
    loadResources();

    Ogre::Light* light = mSceneMgr->createLight("MainLight");
    light->setCastShadows(true);
    Ogre::SceneNode* lightNode = mSceneMgr->getRootSceneNode()->createChildSceneNode();
    lightNode->attachObject(light);
    lightNode->setPosition(0, 20, 75);

    // register our scene with the RTSS

    //! [lightingsset]
    mSceneMgr->setAmbientLight(Ogre::ColourValue(0.5, 0.5, 0.5));
    mSceneMgr->setShadowTechnique(Ogre::SHADOWTYPE_STENCIL_ADDITIVE);
    //! [lightingsset

    createBulletSim();
    createScene();
    setupGUI();
    setupSDL();
    createFrameListener();
    sunParticle = mSceneMgr->createParticleSystem("Sun", "Space/Sun");
    ballParticle = mSceneMgr->createParticleSystem("Explode", "OOB");
    //clangParticle = mSceneMgr->createParticleSystem("Clang", "Clang");
    
    //! [planesetmat] */
    return true;
}

void TutorialApplication::destroyScene(void)
{
}

void TutorialApplication::checkColor(int player){

    if (player){
        if (paddle1->getColor() != ball->getColor()){
            updateScore(player);
        }
     
    } else {
        if (paddle2->getColor() != ball->getColor()){
        updateScore(player);
        }
    }

}

void TutorialApplication::updateScore(int player){
    reset();

    if (player){
        opponentScore++;
    } else {
        playerScore++;
    }

    scoreBoard->setText(getScoreBoardText());

    if (playerScore == 7 || opponentScore == 7){
        //gameover

        std::string winner = (playerScore == 7) ? "Player 1" : "Player 2";

        victoryText->setText(winner + " wins!");

        if (playerScore == 7 && soundEnabled){
            Mix_PlayChannel(-1, wVictory, 0);
            startFireworks();
        }

        CEGUI::System::getSingleton().getDefaultGUIContext().getMouseCursor().show();
        CEGUI::System::getSingleton().getDefaultGUIContext().setRootWindow(restartSheet);
        gameRunning = false;
    }

}

void TutorialApplication::reset(void){
    explode(ball->node->getPosition());
    ball->reset();
    ball->randomizeColor();
    paddle2->opponentChangeColor(ball->getColor());
    paddle1->clearForce();
    paddle2->speed=20;
}

void TutorialApplication::selectGameType(){
    CEGUI::System::getSingleton().getDefaultGUIContext().setRootWindow(selectGameSheet);
    selectGameSheet->setMousePassThroughEnabled(true); // this is important!
}


void TutorialApplication::newGame(void){
    //reset();
    if (fireworksOn){
        startFireworks(); //turn the fireworks off if they are running
        reset();
    }

    CEGUI::System::getSingleton().getDefaultGUIContext().getMouseCursor().hide();
    CEGUI::System::getSingleton().getDefaultGUIContext().setRootWindow(gameSheet);
    gameRunning = true;
    playerScore = 0;
    opponentScore = 0;

    scoreBoard->setText(getScoreBoardText());
    paddle2->opponentChangeColor(ball->getColor());
    ball->push();
}
void TutorialApplication::sendPacket(TutorialApplication::packet packet){
    if (!isClient){
        sendToClient(packet);
    } else {
        sendToServer(packet);
    }
}

void TutorialApplication::sendToClient(TutorialApplication::packet packet){
    sendToSocket(packet, newSd);
}

void TutorialApplication::sendToServer(TutorialApplication::packet packet){
    sendToSocket(packet, clientSd);
}

void TutorialApplication::sendToSocket(TutorialApplication::packet packet, int socket){
    send(socket, &packet, sizeof(packet), 0);
}

TutorialApplication::packet* TutorialApplication::readPacket(){
    if (isClient){
        return readAsClient();
    } else {
        return readAsServer();
    }
}

TutorialApplication::packet* TutorialApplication::readAsClient(){
    return readFromSocket(clientSd);
}

TutorialApplication::packet* TutorialApplication::readAsServer(){
    return readFromSocket(newSd);
}

TutorialApplication::packet* TutorialApplication::readFromSocket(int socket){
    memset(&msg, 0, sizeof(msg));//clear the buffer
    recv(socket, (char*)&msg, sizeof(msg), 0);
    return (packet*)(&msg);
}

void TutorialApplication::hostGame(void){   
    //grab the port number
    //buffer to send and receive messages with
    
     
    //setup a socket and connection tools
    sockaddr_in servAddr;
    bzero((char*)&servAddr, sizeof(servAddr));
    servAddr.sin_family = AF_INET;
    servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servAddr.sin_port = htons(port);
 
    //open stream oriented socket with internet address
    //also keep track of the socket descriptor
    int serverSd = socket(AF_INET, SOCK_STREAM, 0);
    if(serverSd < 0)
    {
        cerr << "Error establishing the server socket" << endl;
        exit(0);
    }
    
    //bind the socket to its local address
    int bindStatus = bind(serverSd, (struct sockaddr*) &servAddr, 
        sizeof(servAddr));
    if(bindStatus < 0)
    {
        cerr << "Error binding socket to local address" << endl;
        exit(0);
    }
    cout << "Waiting for a client to connect..." << endl;
    //listen for up to 5 requests at a time
    listen(serverSd, 5);

    FD_ZERO(&read_fds);
    FD_SET(serverSd, &read_fds);
    timeout.tv_sec = 0;  // 1s timeout
    timeout.tv_usec = 0;
    checkForConnection = true;
}

void TutorialApplication::enterIP(void){
    joinIP->setVisible(true);
}

void TutorialApplication::joinGame(void){
    //we need 2 things: ip address and port number, in that order
    isClient = true;
    char *serverIp = "128.83.139.218";
    //create a message buffer 
    //setup a socket and connection tools 
    struct hostent* host = gethostbyname(serverIp);  
    bzero((char*)&sendSockAddr, sizeof(sendSockAddr)); 
    sendSockAddr.sin_family = AF_INET; 
    sendSockAddr.sin_addr.s_addr = 
        inet_addr(inet_ntoa(*(struct in_addr*)*host->h_addr_list));
    sendSockAddr.sin_port = htons(port);
    clientSd = socket(AF_INET, SOCK_STREAM, 0);
    checkForClientConnection = true;
}

void TutorialApplication::checkJoinConnection(void){
    cout << "Waiting on connect" << endl;
    int status = connect(clientSd,
                         (sockaddr*) &sendSockAddr, sizeof(sendSockAddr));
    if(status < 0)
    {
        cout << "Error connecting to socket!" << endl;
        return;
    }
    cout << "Connected to the server!" << endl;
    if(fcntl(clientSd, F_SETFL, fcntl(clientSd, F_GETFL) | O_NONBLOCK) < 0) {
    // handle error
    }
    struct timeval start1, end1;
    gettimeofday(&start1, NULL);
    idx = 0;
    /*
    while(1)
    {   
        packet apple;
        apple.a = idx;
        apple.b = idx*2;
        apple.c = idx*3;
        idx+=1;
        std::this_thread::sleep_for(std::chrono::milliseconds(1));

        cout << ">";
        string data;
        data = std::to_string(idx);
        //getline(cin, data);
        
        memset(&msg, 0, sizeof(msg));//clear the buffer
        strcpy(msg, data.c_str());
        if(data == "exit")
        {
            send(clientSd, (char*)&msg, strlen(msg), 0);
            break;
        }
        //send(clientSd, &apple, sizeof(apple), 0);
        sendPacket(apple);

        packet *  ptest = readPacket();
        cout << "From Server: " << ptest->b << endl;
        cout << ">";
    }
    gettimeofday(&end1, NULL);
    close(clientSd);
    */
    checkForClientConnection = false;
    newGame();
}

void TutorialApplication::checkConnection(void){
    int select_status = select(serverSd+1, &read_fds, NULL, NULL, &timeout);
    if(select_status == -1){

    }else if(select_status > 0){
        //connection is ready
        //receive a request from client using accept
        //we need a new address to connect with the client
        sockaddr_in newSockAddr;
        socklen_t newSockAddrSize = sizeof(newSockAddr);
        //accept, create a new socket descriptor to 
        //handle the new connection with client

        newSd = accept(serverSd, (sockaddr *)&newSockAddr, &newSockAddrSize);
        if(newSd < 0)
        {
            cerr << "Error accepting request from client!" << endl;
            exit(1);
        }

        cerr << "Error accepting request from client!" << endl;
        //Turn the socket to non blocking mode
        if(fcntl(serverSd, F_SETFL, fcntl(serverSd, F_GETFL) | O_NONBLOCK) < 0) {
        // handle error
        }

        cout << "Connected with client!" << endl;
        //lets keep track of the session time
        struct timeval start1, end1;
        gettimeofday(&start1, NULL);
        checkForConnection = false;
        //also keep track of the amount of data sent as well
        newGame();
    }else{
        //no connection still
        cout << "No connection yet" << endl;
        return;
    }
}



void TutorialApplication::pauseGame(){
    gameRunning=false;
    CEGUI::System::getSingleton().getDefaultGUIContext().getMouseCursor().show();
    CEGUI::System::getSingleton().getDefaultGUIContext().setRootWindow(pauseSheet);
}

void TutorialApplication::resumeGame(){
    gameRunning=true;
    CEGUI::System::getSingleton().getDefaultGUIContext().getMouseCursor().hide();
    CEGUI::System::getSingleton().getDefaultGUIContext().setRootWindow(gameSheet); 
}

std::string TutorialApplication::getScoreBoardText(void){

    std::stringstream stream;

    stream << "Player: " << playerScore <<
         "\nOpponent: " << opponentScore;

   return stream.str();
}


bool TutorialApplication::keyPressed(const OIS::KeyEvent &arg)
{
    //std::cerr << "Key! " << '\n';

    switch(arg.key){
      case OIS::KC_ESCAPE: if (gameRunning) pauseGame(); else mShutDown = true; break;
      case OIS::KC_W:  if (gameRunning) paddle1->changeColor(0); keys[0]=true; break;
      case OIS::KC_A:  if (gameRunning) paddle1->changeColor(1); keys[1]=true; break;
      case OIS::KC_S:  keys[2]=true; break;
      case OIS::KC_D:  if (gameRunning) paddle1->changeColor(2); keys[3]=true; break;
      case OIS::KC_E:  keys[4]=true; break;
      case OIS::KC_Q:  keys[5]=true; break;
      case OIS::KC_LEFT: keys[6]=true; break;
      case OIS::KC_RIGHT: keys[7]=true; break;
      case OIS::KC_R: reset(); break;
      case OIS::KC_SPACE: ball->push(); paddle2->opponentChangeColor(ball->getColor()); break;
      case OIS::KC_K: paddle1->changeColor(); break;
      case OIS::KC_F: startFireworks(); break;
      case OIS::KC_T: explode(Ogre::Vector3()); break;
      //case OIS::KC_G: clang(ball->node->getPosition()); break;
    }

  CEGUI::GUIContext& context = CEGUI::System::getSingleton().getDefaultGUIContext();
  context.injectKeyDown((CEGUI::Key::Scan)arg.key);
  context.injectChar((CEGUI::Key::Scan)arg.text);
  return true;
}


bool TutorialApplication::keyReleased(const OIS::KeyEvent &arg){
  
    //std::cerr << "Key! " << '\n';
    switch(arg.key){
      case OIS::KC_W:  keys[0]=false; break;
      case OIS::KC_A:  keys[1]=false; break;
      case OIS::KC_S:  keys[2]=false; break;
      case OIS::KC_D:  keys[3]=false; break;
      case OIS::KC_E:  keys[4]=false; break;
      case OIS::KC_Q:  keys[5]=false; break;
      case OIS::KC_LEFT: keys[6]=false; paddle1->stop(); break;
      case OIS::KC_RIGHT: keys[7]=false; paddle1->stop(); break;
    }  
    

    CEGUI::System::getSingleton().getDefaultGUIContext().injectKeyUp((CEGUI::Key::Scan)arg.key);
    return true;
}

bool TutorialApplication::frameRenderingQueued(const Ogre::FrameEvent& evt ){

    if(mWindow->isClosed())
      return false;

    if(mShutDown)
      return false;

    mKeyboard->capture();
    mMouse->capture();

    if (gameRunning){

        if (keys[6]){
           paddle1->moveLeft();
        }
        if (keys[7]){
           paddle1->moveRight();
        }

        if (paddle2 && ball){
            paddle2->updatePosition(ball->getNode()->getPosition());
        }

        //ball switches colors when it crosses center of gamefield
        if ((int)ball->getNode()->getPosition().z == 0){
            //ball->randomizeColor();
            paddle2->opponentChangeColor(ball->getColor());

        }

        double minY = walls[0]->wallPosition().getY();
        double maxY = walls[3]->wallPosition().getY();
        double minX = walls[2]->wallPosition().getX();
        double maxX = walls[1]->wallPosition().getX();



        CEGUI::System::getSingleton().injectTimePulse(evt.timeSinceLastFrame);

        dynamicsWorld->stepSimulation(evt.timeSinceLastFrame,5);
        
        checkCollisions();

        paddle1->playerUpdatePosition(mMouse->getMouseState().X.rel, -mMouse->getMouseState().Y.rel, minX, maxX, minY, maxY);

        btTransform transform;
        transform = paddle1->body->getWorldTransform();
        if(mCamera && ball){
            //Ogre::Vector3 paddlePosition = paddle1->getNode()->getPosition();
            Ogre::Vector3 paddlePosition = Ogre::Vector3(transform.getOrigin().getX(), transform.getOrigin().getY(), transform.getOrigin().getZ());
            //Ogre::Vector3 curPos = mCamNode->getPosition();
            //Ogre::Vector3 desiredPos = Ogre::Vector3(paddlePosition.x, 30, paddlePosition.z*2);
            //double diviser = 1; // make diviser > 1 for smooth camera, probably a number in the low hundereds maybe
            //Ogre::Vector3 cameraPosition = Ogre::Vector3(curPos.x+(desiredPos.x-curPos.x)/diviser, paddlePosition.y, curPos.z+(desiredPos.z-curPos.z)/diviser);
            Ogre::Vector3 cameraPosition = Ogre::Vector3(paddlePosition.x, paddlePosition.y+5, paddlePosition.z+40);
            mCamNode->setPosition(cameraPosition);
            mCamera->lookAt(paddlePosition.x,paddlePosition.y,0);
        }

        if (ballTrigger>0){
            ballTrigger-=1;
        } else if (ballTrigger==0) {
            ball->randomizeColor();
            ballTrigger-=1;
            ball->speedUp(1.12);
            paddle2->speedUp(1.12);
        }

        if (collisionTimer>0){
            collisionTimer-=1;
        }
        
        } 
        else {
                    //if game is paused or hasnt started, move camera with arrow keys
        if (keys[0]){
             mCamNode->translate(0,0,-0.1);
         }
         if (keys[1]){
             mCamNode->translate(-0.1,0,0);
        }
         if (keys[2]){
            mCamNode->translate(0,0,0.1);
         }
         if (keys[3]){
             mCamNode->translate(0.1,0,0);
         }
         if (keys[4]){
             mCamNode->translate(0,0.1,0);
        }
         if (keys[5]){
         mCamNode->translate(0,-0.1,0);
        }
    }

    if(checkForConnection){
        checkConnection();
    }else if(checkForClientConnection){
        checkJoinConnection();
    }

    idx+=1;
    //std::this_thread::sleep_for(std::chrono::milliseconds(1000));

        //server send info
    if (!isClient){
        packet p;
        p.valid = true;
        p.serverPaddlePos = paddle1->node->getPosition();
        sendPacket(p);
    
    } else {
        //client receive info
        packet *  ptest = readPacket();
        if (ptest->valid)
            paddle1->movePaddleLocation(ptest->serverPaddlePos);
    }

    

    //memset(&msg, 0, sizeof(msg));

    //send(newSd, &orange, sizeof(orange), 0);
    //sendPacket(orange);

    return true;
}

void TutorialApplication::checkCollisions(){
    dynamicsWorld->contactPairTest(walls[4]->getRigidBody(), ball->getRigidBody(), *owcb);
    dynamicsWorld->contactPairTest(walls[5]->getRigidBody(), ball->getRigidBody(), *pwcb);
    dynamicsWorld->contactPairTest(paddle1->getRigidBody(), ball->getRigidBody(), *p1cb);
    dynamicsWorld->contactPairTest(paddle2->getRigidBody(), ball->getRigidBody(), *p2cb);

    for (int i = 0; i < 4; i++){
      dynamicsWorld->contactPairTest(walls[i]->getRigidBody(), ball->getRigidBody(), *swcb);
    }

    //add all wall noise collisions
}

bool TutorialApplication::mouseMoved(const OIS::MouseEvent &arg)
{

    CEGUI::GUIContext& context = CEGUI::System::getSingleton().getDefaultGUIContext();
    context.injectMouseMove(arg.state.X.rel, arg.state.Y.rel);
    // Scroll wheel.
    if (arg.state.Z.rel)
      context.injectMouseWheelChange(arg.state.Z.rel / 120.0f);
    return true;
}
//---------------------------------------------------------------------------
bool TutorialApplication::mousePressed(const OIS::MouseEvent &arg, OIS::MouseButtonID id)
{
  CEGUI::GUIContext& context = CEGUI::System::getSingleton().getDefaultGUIContext();
  context.injectMouseButtonDown(convertButton(id));
  return true;
}
//---------------------------------------------------------------------------
bool TutorialApplication::mouseReleased(const OIS::MouseEvent &arg, OIS::MouseButtonID id)
{
    CEGUI::GUIContext& context = CEGUI::System::getSingleton().getDefaultGUIContext();
    context.injectMouseButtonUp(convertButton(id));
    return true;
}

CEGUI::MouseButton TutorialApplication::convertButton(OIS::MouseButtonID buttonID)
{
    switch (buttonID)
    {
    case OIS::MB_Left:
        return CEGUI::LeftButton;

    case OIS::MB_Right:
        return CEGUI::RightButton;

    case OIS::MB_Middle:
        return CEGUI::MiddleButton;

    default:
        return CEGUI::LeftButton;
    }
}

void TutorialApplication::go(void)
{
#ifdef _DEBUG
#ifndef OGRE_STATIC_LIB
    mResourcesCfg = m_ResourcePath + "resources_d.cfg";
    mPluginsCfg = m_ResourcePath + "plugins_d.cfg";
#else
    mResourcesCfg = "resources_d.cfg";
    mPluginsCfg = "plugins_d.cfg";
#endif
#else
#ifndef OGRE_STATIC_LIB
    mResourcesCfg = m_ResourcePath + "resources.cfg";
    mPluginsCfg = m_ResourcePath + "plugins.cfg";
#else
    mResourcesCfg = "resources.cfg";
    mPluginsCfg = "plugins.cfg";
#endif
#endif

    if (!setup())
        return;

    mRoot->startRendering();
    destroyScene();
}

// Adjust mouse clipping area
void TutorialApplication::windowResized(Ogre::RenderWindow* rw)
{
    unsigned int width, height, depth;
    int left, top;
    rw->getMetrics(width, height, depth, left, top);

    const OIS::MouseState &ms = mMouse->getMouseState();
    ms.width = width;
    ms.height = height;
}
//---------------------------------------------------------------------------
// Unattach OIS before window shutdown (very important under Linux)
void TutorialApplication::windowClosed(Ogre::RenderWindow* rw)
{
    // Only close for window that created OIS (the main window in these demos)
    if(rw == mWindow)
    {
        if(mInputManager)
        {
            mInputManager->destroyInputObject(mMouse);
            mInputManager->destroyInputObject(mKeyboard);

            OIS::InputManager::destroyInputSystem(mInputManager);
            mInputManager = 0;
        }
    }
}

bool TutorialApplication::quit(const CEGUI::EventArgs& e){
  mShutDown = true;
  return true;
}

 int main(int argc, char **argv)
 {
    TutorialApplication app;
    app.go();
    return 0;
}



//! [starter]

