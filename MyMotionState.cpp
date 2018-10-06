#include <Ogre.h>
#include "btBulletCollisionCommon.h"
#include "btBulletDynamicsCommon.h"

class MyMotionState : public btMotionState {
public:
    MyMotionState(const btTransform &initialpos, Ogre::SceneNode *node) {
        mVisibleobj = node;
        mPos1 = initialpos;
    }
    virtual ~MyMotionState() {    }
    void setNode(Ogre::SceneNode *node) {
        mVisibleobj = node;
    }
    virtual void getWorldTransform(btTransform &worldTrans) const {
        worldTrans = mPos1;
    }
    virtual void setWorldTransform(const btTransform &worldTrans) {
        if(NULL == mVisibleobj) return; // silently return before we set a node
        btQuaternion rot = worldTrans.getRotation();
        mVisibleobj->setOrientation(rot.w(), rot.x(), rot.y(), rot.z());
        btVector3 pos = worldTrans.getOrigin();
        // TODO **** XXX need to fix this up such that it renders properly since this doesnt know the scale of the node
        // also the getCube function returns a cube that isnt centered on Z
        mVisibleobj->setPosition(pos.x(), pos.y()+5, pos.z()-5);
    }
protected:
    Ogre::SceneNode *mVisibleobj;
    btTransform mPos1;
};