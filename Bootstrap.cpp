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
#include <iostream>

#include <Ogre.h>
#include <OgreApplicationContext.h>
#include <OgreInput.h>
#include <OgreRTShaderSystem.h>
#include <OgreApplicationContext.h>
#include <OgreCameraMan.h>
#include "Ball.h"

using namespace Ogre;
using namespace OgreBites;

int i = 0;
Camera* cam;

class TutorialApplication
        : public ApplicationContext
        , public InputListener
{
public:
    TutorialApplication();
    virtual ~TutorialApplication();

    void setup();
    bool keyPressed(const KeyboardEvent& evt);
    bool keyReleased(const KeyboardEvent& evt);
    void frameRendered(const Ogre::FrameEvent & evt );
    Ball* b;
    bool* keys = new bool[6];
    SceneNode* camNode;
};


TutorialApplication::TutorialApplication()
    : ApplicationContext("OgreTutorialApp")
{
}


TutorialApplication::~TutorialApplication()
{
}


void TutorialApplication::setup()
{
    // do not forget to call the base first
    for (int i = 0; i < 6; i++){
        keys[i]=false;
    }
    ApplicationContext::setup();
    addInputListener(this);

    // get a pointer to the already created root
    Root* root = getRoot();
    SceneManager* scnMgr = root->createSceneManager();

    // register our scene with the RTSS
    RTShader::ShaderGenerator* shadergen = RTShader::ShaderGenerator::getSingletonPtr();
    shadergen->addSceneManager(scnMgr);

    // -- tutorial section start --
    //! [cameracreate]
    camNode = scnMgr->getRootSceneNode()->createChildSceneNode();
    cam = scnMgr->createCamera("myCam");
    //! [cameracreate]

    //! [cameraposition]
    camNode->setPosition(0, 0, 20);
    camNode->lookAt(Vector3(0, 0, -300), Node::TransformSpace::TS_WORLD);
    //! [cameraposition]

    //! [cameralaststep]
    cam->setNearClipDistance(5);
    camNode->attachObject(cam);
    //! [cameralaststep]

    //! [addviewport]
    Viewport* vp = getRenderWindow()->addViewport(cam);
    //! [addviewport]

    //! [viewportback]
    vp->setBackgroundColour(ColourValue(0, 0, 0));
    //! [viewportback]

    //! [cameraratio]
    cam->setAspectRatio(Real(vp->getActualWidth()) / Real(vp->getActualHeight()));
    //! [cameraratio]

    Light* light = scnMgr->createLight("MainLight");
    SceneNode* lightNode = scnMgr->getRootSceneNode()->createChildSceneNode();
    lightNode->attachObject(light);
    lightNode->setPosition(0, 10, 0);

    scnMgr->setSkyBox(true, "Examples/MorningSkyBox", 5000, false);
    //! [ninja]
    /*
    Entity* sphereEntity = scnMgr->createEntity("sphere.mesh");

    sphereEntity->setCastShadows(true);
    
    SceneNode* sphere_scene = scnMgr->getRootSceneNode()->createChildSceneNode();
    sphere_scene->attachObject(sphereEntity);
    sphere_scene->setScale(.01, .01, .01);
    sphere_scene->setPosition(0,0,0);
    */
    b = new Ball(scnMgr);

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

    for (int i = 0; i < 6; i++){
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
    
    //! [planesetmat]

    //! [lightingsset]
    scnMgr->setAmbientLight(ColourValue(1, 1, 1));
    scnMgr->setShadowTechnique(ShadowTechnique::SHADOWTYPE_STENCIL_MODULATIVE);
    //! [lightingsset]
}


bool TutorialApplication::keyPressed(const KeyboardEvent& evt)
{
    std::cerr << "Key! " << '\n';
    if (evt.keysym.sym == SDLK_ESCAPE)
    {
        getRoot()->queueEndRendering();
    }
    if (evt.keysym.sym == 'r')
    {
        b->Ball::reset();
    }
    if (evt.keysym.sym == 'w')
    {
        keys[0]=true;
    }
    if (evt.keysym.sym == 'a')
    {
        keys[1]=true;
    }
    if (evt.keysym.sym == 's')
    {
        keys[2]=true;
    }
    if (evt.keysym.sym == 'd')
    {
        keys[3]=true;
    }
    if (evt.keysym.sym == 'e')
    {
        keys[4]=true;
    }
    if (evt.keysym.sym == 'q')
    {
        keys[5]=true;
    }
    return true;
}

bool TutorialApplication::keyReleased(const KeyboardEvent& evt)
{
    std::cerr << "Key! " << '\n';
    if (evt.keysym.sym == SDLK_ESCAPE)
    {
        getRoot()->queueEndRendering();
    }
    if (evt.keysym.sym == 'w')
    {
        keys[0]=false;
    }
    if (evt.keysym.sym == 'a')
    {
        keys[1]=false;
    }
    if (evt.keysym.sym == 's')
    {
        keys[2]=false;
    }
    if (evt.keysym.sym == 'd')
    {
        keys[3]=false;
    }
    if (evt.keysym.sym == 'e')
    {
        keys[4]=false;
    }
    if (evt.keysym.sym == 'q')
    {
        keys[5]=false;
    }
    return true;
}

void TutorialApplication::frameRendered(const Ogre::FrameEvent & evt ){
    b->Ball::move(evt);

    if (keys[0]==true){
        camNode->translate(0,0,-.1);
    }
    if (keys[1]==true){
        camNode->translate(-.1,0,0);
    }
    if (keys[2]==true){
        camNode->translate(0,0,.1);
    }
    if (keys[3]==true){
        camNode->translate(.1,0,0);
    }
    if (keys[4]==true){
        camNode->translate(0,.1,0);
    }
    if (keys[5]==true){
        camNode->translate(0,-.1,0);
    }
}

int main(int argc, char **argv)
{
    try
    {
        TutorialApplication app;
        app.initApp();
        app.getRoot()->startRendering();
        app.closeApp();
    }
    catch (const std::exception& e)
    {
        std::cerr << "Error occurred during execution: " << e.what() << '\n';
        return 1;
    }

    return 0;
}

//! [starter]