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

struct OpponentWallCallback :public btCollisionWorld::ContactResultCallback
{ OpponentWallCallback(TutorialApplication* ptr) : context(ptr) {}
    btScalar addSingleResult(btManifoldPoint& cp,
        const btCollisionObjectWrapper* colObj0Wrap,
        int partId0,
        int index0,
        const btCollisionObjectWrapper* colObj1Wrap,
        int partId1,
        int index1)
    {
        if (context->soundEnabled){
            Mix_PlayChannel(-1, context->wBounce, 0);
        }
        context->updateScore(0);
    }

    TutorialApplication* context;
};

struct PlayerWallCallback : public btCollisionWorld::ContactResultCallback
{
    PlayerWallCallback(TutorialApplication* ptr) : context(ptr) {}

    btScalar addSingleResult(btManifoldPoint& cp,
        const btCollisionObjectWrapper* colObj0Wrap,
        int partId0,
        int index0,
        const btCollisionObjectWrapper* colObj1Wrap,
        int partId1,
        int index1)
    {
        if (context->soundEnabled){
            Mix_PlayChannel(-1, context->wBounce, 0);
        }
        context->updateScore(1);
    }

    TutorialApplication* context;
};

struct Paddle1Callback : public btCollisionWorld::ContactResultCallback
{
    Paddle1Callback(TutorialApplication* ptr) : context(ptr) {}

    btScalar addSingleResult(btManifoldPoint& cp,
        const btCollisionObjectWrapper* colObj0Wrap,
        int partId0,
        int index0,
        const btCollisionObjectWrapper* colObj1Wrap,
        int partId1,
        int index1)
    {
        context->checkColor(1);
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

        pwcb = new PlayerWallCallback(this);
        owcb = new OpponentWallCallback(this);
        p1cb = new Paddle1Callback(this);
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
        pauseSheet = wmgr.createWindow("DefaultWindow", "CEGUIDemo/Sheet");


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

        // CEGUI::Window *gameQuit = wmgr.createWindow("TaharezLook/Button", "CEGUIDemo/QuitButton");
        // gameQuit->setText("Quit");
        // gameQuit->setSize(CEGUI::USize(CEGUI::UDim(0.15, 0), CEGUI::UDim(0.05, 0)));
        // gameQuit->addChild(gameQuit);
        // gameQuit->subscribeEvent(CEGUI::PushButton::EventClicked, 
        // CEGUI::SubscriberSlot(&TutorialApplication::quit, this));


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
        CEGUI::SubscriberSlot(&TutorialApplication::newGame, this));

        menuSheet->addChild(start);

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

        CEGUI::System::getSingleton().getDefaultGUIContext().setRootWindow(menuSheet);
        gameRunning = false;
}

bool TutorialApplication::setupSDL(){
    bool success = true;

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
    wBounce = Mix_LoadWAV( "sounds/bounce.wav" ); 
    if( wBounce == NULL ) { 
        printf( "Failed to load scratch sound effect! SDL_mixer Error: %s\n", Mix_GetError() ); 
        success = false;
    }

    wExplode = Mix_LoadWAV( "sounds/bbc_explode.wav" ); 
    if( wExplode == NULL ) { 
        printf( "Failed to load scratch sound effect! SDL_mixer Error: %s\n", Mix_GetError() ); 
        success = false;
    }

    return success;
}

void TutorialApplication::switchSound(){

    soundEnabled = !soundEnabled;
    std::string enabled = soundEnabled ? "ON" : "OFF";
    sound->setText("Sound: " + enabled);

}

void TutorialApplication::startFireworks(){
    if (fireworksOn==false){
        //sunParticle->clear();
        mSceneMgr->destroyParticleSystem("Sun");
        sunParticle = mSceneMgr->createParticleSystem("Sun", "Space/Sun");
        Ogre::SceneNode* particleNode = mSceneMgr->getRootSceneNode()->createChildSceneNode("Fireworks");
        particleNode->attachObject(sunParticle);
        particleNode->setPosition(0,-40,-300);
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
    if (soundEnabled){
            Mix_PlayChannel(-1, wExplode, 0);
        }
    if (mSceneMgr->hasSceneNode("BallExplode"))
    {
        Ogre::SceneNode* itemNode = mSceneMgr->getSceneNode("BallExplode");
        itemNode->removeAndDestroyAllChildren();
        mSceneMgr->destroySceneNode(itemNode);    
    }
    
    

    mSceneMgr->destroyParticleSystem("Explode");
    ballParticle = mSceneMgr->createParticleSystem("Explode", "OOB");
    Ogre::SceneNode* particleNode = mSceneMgr->getRootSceneNode()->createChildSceneNode("BallExplode");
    particleNode->attachObject(ballParticle);

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
    lightNode->setPosition(0, 10, 0);

    // register our scene with the RTSS

    //! [lightingsset]
    mSceneMgr->setAmbientLight(Ogre::ColourValue(1, 1, 1));
    mSceneMgr->setShadowTechnique(Ogre::SHADOWTYPE_STENCIL_ADDITIVE);
    //! [lightingsset

    createBulletSim();
    createScene();
    setupGUI();
    setupSDL();
    createFrameListener();
    sunParticle = mSceneMgr->createParticleSystem("Sun", "Space/Sun");
    ballParticle = mSceneMgr->createParticleSystem("Explode", "OOB");
    
    //! [planesetmat] */
    return true;
}

void TutorialApplication::destroyScene(void)
{
}

void TutorialApplication::checkColor(int player){
    //reset();

    //std::cout << "Paddle collision" << std::endl;

    if (player){
        //std::cout << "Ball: " << ball->getColor() << std::endl;
        //std::cout << "Paddle1: " << paddle1->getColor() << std::endl;
        if (paddle1->getColor() != ball->getColor()){
            //std::cout << "updating score, paddle 1 was not correct color" << std::endl;
            updateScore(player);
        }
     
    } else {
                // std::cout << "Ball: " << ball->getColor() << std::endl;
           //std::cout << "Paddle2: " << paddle2->getColor() << std::endl;
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
        CEGUI::System::getSingleton().getDefaultGUIContext().setRootWindow(menuSheet);
        gameRunning = false;
    }

}

void TutorialApplication::reset(void){
    explode(ball->node->getPosition());
    ball->reset();
    ball->randomizeColor();
    paddle1->clearForce();
}

void TutorialApplication::newGame(void){
    //reset();
    CEGUI::System::getSingleton().getDefaultGUIContext().setRootWindow(gameSheet);
    gameRunning = true;
    playerScore = 0;
    opponentScore = 0;

    scoreBoard->setText(getScoreBoardText());
    ball->push();
}

void TutorialApplication::pauseGame(){
    gameRunning=false;
    CEGUI::System::getSingleton().getDefaultGUIContext().setRootWindow(pauseSheet);
}

void TutorialApplication::resumeGame(){
    gameRunning=true;
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
      case OIS::KC_W:  keys[0]=true; break;
      case OIS::KC_A:  keys[1]=true; break;
      case OIS::KC_S:  keys[2]=true; break;
      case OIS::KC_D:  keys[3]=true; break;
      case OIS::KC_E:  keys[4]=true; break;
      case OIS::KC_Q:  keys[5]=true; break;
      case OIS::KC_LEFT: keys[6]=true; break;
      case OIS::KC_RIGHT: keys[7]=true; break;
      case OIS::KC_R: reset(); break;
      case OIS::KC_SPACE: ball->push(); break;
      case OIS::KC_K: paddle1->changeColor(); break;
      case OIS::KC_F: startFireworks(); break;
      case OIS::KC_T: explode(Ogre::Vector3()); break;
    }

  CEGUI::GUIContext& context = CEGUI::System::getSingleton().getDefaultGUIContext();
  context.injectKeyDown((CEGUI::Key::Scan)arg.key);
  context.injectChar((CEGUI::Key::Scan)arg.text);
  return true;
}


bool TutorialApplication::keyReleased(const OIS::KeyEvent &arg)
{
  
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
    
    if(mCamera && ball){
        Ogre::Vector3 paddlePosition = paddle1->getNode()->getPosition();
        Ogre::Vector3 curPos = mCamNode->getPosition();
        Ogre::Vector3 desiredPos = Ogre::Vector3(paddlePosition.x, 30, paddlePosition.z*2);
        double diviser = 1; // make diviser > 1 for smooth camera, probably a number in the low hundereds maybe
        Ogre::Vector3 cameraPosition = Ogre::Vector3(curPos.x+(desiredPos.x-curPos.x)/diviser, paddlePosition.y, curPos.z+(desiredPos.z-curPos.z)/diviser);
        mCamNode->setPosition(cameraPosition);
        mCamera->lookAt(paddlePosition.x,paddlePosition.y,0);
    }


    //b->Ball::move(evt);
    // if (mCamera && ball){
    //   mCamera->lookAt(ball->getNode()->getPosition());
    // }

    // if (keys[0]){
    //     mCamNode->translate(0,0,-0.1);
    // }
    // if (keys[1]){
    //     mCamNode->translate(-0.1,0,0);
    // }
    // if (keys[2]){
    //     mCamNode->translate(0,0,0.1);
    // }
    // if (keys[3]){
    //     mCamNode->translate(0.1,0,0);
    // }
    // if (keys[4]){
    //     mCamNode->translate(0,0.1,0);
    // }
    // if (keys[5]){
    //     mCamNode->translate(0,-0.1,0);
    // }
    if (keys[6]){
       paddle1->moveLeft();
    }
    if (keys[7]){
       paddle1->moveRight();
    }

    if (paddle2 && ball){
        paddle2->updatePosition(ball->getNode()->getPosition());
    }

    paddle1->playerUpdatePosition(mMouse->getMouseState().X.rel, -mMouse->getMouseState().Y.rel);


    CEGUI::System::getSingleton().injectTimePulse(evt.timeSinceLastFrame);

    dynamicsWorld->stepSimulation(evt.timeSinceLastFrame,5);
    
    checkCollisions();
    
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

    return true;
}

void TutorialApplication::checkCollisions(){
    dynamicsWorld->contactPairTest(walls[4]->getRigidBody(), ball->getRigidBody(), *owcb);
    dynamicsWorld->contactPairTest(walls[5]->getRigidBody(), ball->getRigidBody(), *pwcb);
    dynamicsWorld->contactPairTest(paddle1->getRigidBody(), ball->getRigidBody(), *p1cb);
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

