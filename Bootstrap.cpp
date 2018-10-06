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

Ogre::ManualObject* TutorialApplication::createCubeMesh(Ogre::String name, Ogre::String matName) {

   Ogre::ManualObject* cube = new Ogre::ManualObject(name);

   cube->begin(matName);

   cube->position(0.5,-0.5,1.0);cube->normal(0.408248,-0.816497,0.408248);cube->textureCoord(1,0);
   cube->position(-0.5,-0.5,0.0);cube->normal(-0.408248,-0.816497,-0.408248);cube->textureCoord(0,1);
   cube->position(0.5,-0.5,0.0);cube->normal(0.666667,-0.333333,-0.666667);cube->textureCoord(1,1);
   cube->position(-0.5,-0.5,1.0);cube->normal(-0.666667,-0.333333,0.666667);cube->textureCoord(0,0);
   cube->position(0.5,0.5,1.0);cube->normal(0.666667,0.333333,0.666667);cube->textureCoord(1,0);
   cube->position(-0.5,-0.5,1.0);cube->normal(-0.666667,-0.333333,0.666667);cube->textureCoord(0,1);
   cube->position(0.5,-0.5,1.0);cube->normal(0.408248,-0.816497,0.408248);cube->textureCoord(1,1);
   cube->position(-0.5,0.5,1.0);cube->normal(-0.408248,0.816497,0.408248);cube->textureCoord(0,0);
   cube->position(-0.5,0.5,0.0);cube->normal(-0.666667,0.333333,-0.666667);cube->textureCoord(0,1);
   cube->position(-0.5,-0.5,0.0);cube->normal(-0.408248,-0.816497,-0.408248);cube->textureCoord(1,1);
   cube->position(-0.5,-0.5,1.0);cube->normal(-0.666667,-0.333333,0.666667);cube->textureCoord(1,0);
   cube->position(0.5,-0.5,0.0);cube->normal(0.666667,-0.333333,-0.666667);cube->textureCoord(0,1);
   cube->position(0.5,0.5,0.0);cube->normal(0.408248,0.816497,-0.408248);cube->textureCoord(1,1);
   cube->position(0.5,-0.5,1.0);cube->normal(0.408248,-0.816497,0.408248);cube->textureCoord(0,0);
   cube->position(0.5,-0.5,0.0);cube->normal(0.666667,-0.333333,-0.666667);cube->textureCoord(1,0);
   cube->position(-0.5,-0.5,0.0);cube->normal(-0.408248,-0.816497,-0.408248);cube->textureCoord(0,0);
   cube->position(-0.5,0.5,1.0);cube->normal(-0.408248,0.816497,0.408248);cube->textureCoord(1,0);
   cube->position(0.5,0.5,0.0);cube->normal(0.408248,0.816497,-0.408248);cube->textureCoord(0,1);
   cube->position(-0.5,0.5,0.0);cube->normal(-0.666667,0.333333,-0.666667);cube->textureCoord(1,1);
   cube->position(0.5,0.5,1.0);cube->normal(0.666667,0.333333,0.666667);cube->textureCoord(0,0);

   cube->triangle(0,1,2);      cube->triangle(3,1,0);
   cube->triangle(4,5,6);      cube->triangle(4,7,5);
   cube->triangle(8,9,10);      cube->triangle(10,7,8);
   cube->triangle(4,11,12);   cube->triangle(4,13,11);
   cube->triangle(14,8,12);   cube->triangle(14,15,8);
   cube->triangle(16,17,18);   cube->triangle(16,19,17);
   cube->end();

   return cube;
}



Ogre::ManualObject* TutorialApplication::createPaddleMesh(Ogre::String name, Ogre::String matName) {

   Ogre::ManualObject* paddle = new Ogre::ManualObject(name);

   paddle->begin(matName);

   paddle->position(0.5,-0.5,1.0);paddle->normal(0.408248,-0.816497,0.408248);paddle->textureCoord(1,0);
   paddle->position(-0.5,-0.5,0.0);paddle->normal(-0.408248,-0.816497,-0.408248);paddle->textureCoord(0,1);
   paddle->position(0.5,-0.5,0.0);paddle->normal(0.666667,-0.333333,-0.666667);paddle->textureCoord(1,1);
   paddle->position(-0.5,-0.5,1.0);paddle->normal(-0.666667,-0.333333,0.666667);paddle->textureCoord(0,0);
   paddle->position(0.5,0.5,1.0);paddle->normal(0.666667,0.333333,0.666667);paddle->textureCoord(1,0);
   paddle->position(-0.5,-0.5,1.0);paddle->normal(-0.666667,-0.333333,0.666667);paddle->textureCoord(0,1);
   paddle->position(0.5,-0.5,1.0);paddle->normal(0.408248,-0.816497,0.408248);paddle->textureCoord(1,1);
   paddle->position(-0.5,0.5,1.0);paddle->normal(-0.408248,0.816497,0.408248);paddle->textureCoord(0,0);
   paddle->position(-0.5,0.5,0.0);paddle->normal(-0.666667,0.333333,-0.666667);paddle->textureCoord(0,1);
   paddle->position(-0.5,-0.5,0.0);paddle->normal(-0.408248,-0.816497,-0.408248);paddle->textureCoord(1,1);
   paddle->position(-0.5,-0.5,1.0);paddle->normal(-0.666667,-0.333333,0.666667);paddle->textureCoord(1,0);
   paddle->position(0.5,-0.5,0.0);paddle->normal(0.666667,-0.333333,-0.666667);paddle->textureCoord(0,1);
   paddle->position(0.5,0.5,0.0);paddle->normal(0.408248,0.816497,-0.408248);paddle->textureCoord(1,1);
   paddle->position(0.5,-0.5,1.0);paddle->normal(0.408248,-0.816497,0.408248);paddle->textureCoord(0,0);
   paddle->position(0.5,-0.5,0.0);paddle->normal(0.666667,-0.333333,-0.666667);paddle->textureCoord(1,0);
   paddle->position(-0.5,-0.5,0.0);paddle->normal(-0.408248,-0.816497,-0.408248);paddle->textureCoord(0,0);
   paddle->position(-0.5,0.5,1.0);paddle->normal(-0.408248,0.816497,0.408248);paddle->textureCoord(1,0);
   paddle->position(0.5,0.5,0.0);paddle->normal(0.408248,0.816497,-0.408248);paddle->textureCoord(0,1);
   paddle->position(-0.5,0.5,0.0);paddle->normal(-0.666667,0.333333,-0.666667);paddle->textureCoord(1,1);
   paddle->position(0.5,0.5,1.0);paddle->normal(0.666667,0.333333,0.666667);paddle->textureCoord(0,0);

   paddle->triangle(0,1,2);      paddle->triangle(3,1,0);
   paddle->triangle(4,5,6);      paddle->triangle(4,7,5);
   paddle->triangle(8,9,10);      paddle->triangle(10,7,8);
   paddle->triangle(4,11,12);   paddle->triangle(4,13,11);
   paddle->triangle(14,8,12);   paddle->triangle(14,15,8);
   paddle->triangle(16,17,18);   paddle->triangle(16,19,17);
   paddle->end();

   return paddle;
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

void TutorialApplication::createScene()
    {
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


        // Make paddles
        Ogre::ManualObject *cmo = createCubeMesh("manual", "");
        cmo->convertToMesh("cube");
        //ent = scnMgr->createEntity("cube.mesh");
        paddle1 = new Paddle(mSceneMgr, btVector3(0, 0, 40), mSceneMgr->createEntity("cube.mesh"));
        collisionShapes.push_back(paddle1->getCollisionShape());
        dynamicsWorld->addRigidBody(paddle1->getRigidBody());

        paddle2 = new Paddle(mSceneMgr, btVector3(0, 0, -40), mSceneMgr->createEntity("cube.mesh"));
        collisionShapes.push_back(paddle2->getCollisionShape());
        dynamicsWorld->addRigidBody(paddle2->getRigidBody());
   
        //make ball
        ball = new Ball(mSceneMgr, btVector3(0, 5, 0));
        collisionShapes.push_back(ball->getCollisionShape());
        //ballNode = ball->getNode();
        dynamicsWorld->addRigidBody(ball->getRigidBody());


        //make ground
        ground = new Wall(mSceneMgr, btVector3(0,-6,0), Ogre::Degree(0), Ogre::Degree(0));
        collisionShapes.push_back(ground->getCollisionShape());
        dynamicsWorld->addRigidBody(ground->getRigidBody());
  }


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
#if OGRE_PLATFORM == OGRE_PLATFORM_APPLE
    m_ResourcePath = Ogre::macBundlePath() + "/Contents/Resources/";
#else
    m_ResourcePath = "";
#endif
}

TutorialApplication::~TutorialApplication()
{
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
    mCamNode->setPosition(0, 30, 75);
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
        mRenderer = &CEGUI::OgreRenderer::bootstrapSystem();
        CEGUI::ImageManager::setImagesetDefaultResourceGroup("Imagesets");
        CEGUI::Font::setDefaultResourceGroup("Fonts");
        CEGUI::Scheme::setDefaultResourceGroup("Schemes");
        CEGUI::WidgetLookManager::setDefaultResourceGroup("LookNFeel");
        CEGUI::WindowManager::setDefaultResourceGroup("Layouts");

        CEGUI::SchemeManager::getSingleton().createFromFile("TaharezLook.scheme");
        CEGUI::System::getSingleton().getDefaultGUIContext().getMouseCursor().setDefaultImage("TaharezLook/MouseArrow");
        
        
        CEGUI::WindowManager &wmgr = CEGUI::WindowManager::getSingleton();
        CEGUI::Window *sheet = wmgr.createWindow("DefaultWindow", "CEGUIDemo/Sheet");

        CEGUI::Window *quit = wmgr.createWindow("TaharezLook/Button", "CEGUIDemo/QuitButton");
        quit->setText("Quit");
        quit->setSize(CEGUI::USize(CEGUI::UDim(0.15, 0), CEGUI::UDim(0.05, 0)));
        sheet->addChild(quit);
        CEGUI::System::getSingleton().getDefaultGUIContext().setRootWindow(sheet);

        quit->subscribeEvent(CEGUI::PushButton::EventClicked, 
        CEGUI::SubscriberSlot(&TutorialApplication::quit, this));
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
    Ogre::SceneNode* lightNode = mSceneMgr->getRootSceneNode()->createChildSceneNode();
    lightNode->attachObject(light);
    lightNode->setPosition(0, 10, 0);

    // register our scene with the RTSS

    //! [lightingsset]
    mSceneMgr->setAmbientLight(Ogre::ColourValue(1, 1, 1));
    //mSceneMgr->setShadowTechnique(Ogre::ShadowTechnique::SHADOWTYPE_STENCIL_MODULATIVE);
    //! [lightingsset

    createScene();
    setupGUI();
    createFrameListener();
    
    /*
    Plane plane(Vector3::UNIT_Y, -5);
    //! [planedefine]
    MeshManager::getSingleton().createPlane(
            "plane",
            ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
            plane,
            10, 10, 20, 20,
            true,
            1, 5, 5,
            Vector3::UNIT_Z);

    Entity** walls_entity = new Entity*[6];
    SceneNode** walls_scene = new SceneNode*[6];

    for (int i = 0; i < 0; i++){
        std::string name = "plane"+std::to_string(i);
        std::cerr << name << '\n';
        walls_entity[i] = scnMgr->createEntity("plane");
        walls_scene[i] = scnMgr->getRootSceneNode()->createChildSceneNode();
        walls_scene[i]->attachObject(walls_entity[i]);
        walls_entity[i]->setCastShadows(false);
        walls_entity[i]->setMaterialName("Examples/Rockwall");
        if (i==1){
            walls_scene[i]->roll(Degree(90));
        }
        if (i==2){
            walls_scene[i]->roll(Degree(-90));
        }
        if (i==3){
            walls_scene[i]->pitch(Degree(90));
        }
        if (i==4){
            walls_scene[i]->pitch(Degree(-90));
        }
        if (i==5){
            walls_scene[i]->roll(Degree(180));
        }
    }
    
    //! [planesetmat] */
    return true;
}

void TutorialApplication::destroyScene(void)
{
}


bool TutorialApplication::keyPressed(const OIS::KeyEvent &arg)
{
    //std::cerr << "Key! " << '\n';

    switch(arg.key){
      case OIS::KC_ESCAPE: mShutDown = true; break;
      case OIS::KC_W:  keys[0]=true; break;
      case OIS::KC_A:  keys[1]=true; break;
      case OIS::KC_S:  keys[2]=true; break;
      case OIS::KC_D:  keys[3]=true; break;
      case OIS::KC_E:  keys[4]=true; break;
      case OIS::KC_Q:  keys[5]=true; break;
      case OIS::KC_LEFT: keys[6]=true; break;
      case OIS::KC_RIGHT: keys[7]=true; break;
      case OIS::KC_R: ball->reset(); break;
      case OIS::KC_SPACE: ball->push();
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
      case OIS::KC_ESCAPE: mShutDown = true; break;
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
    
    //b->Ball::move(evt);
    if (mCamera && ball){
       mCamera->lookAt(ball->getNode()->getPosition());
    }

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
    if (keys[6]){
       paddle1->moveLeft();
    }
    if (keys[7]){
       paddle1->moveRight();
    }

    /*
    if (!(keys[6] || keys[7])){
      paddle1->stop();
    }*/

    CEGUI::System::getSingleton().injectTimePulse(evt.timeSinceLastFrame);
    dynamicsWorld->stepSimulation(evt.timeSinceLastFrame,5);
    return true;
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

