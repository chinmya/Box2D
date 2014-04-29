#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include "Box2d/Box2D.h"
#include "GLES-Render.h"
#pragma comment (lib, "libBox2D.lib")

USING_NS_CC;

#define PTM_RATIO 32.0

class HelloWorld : public Layer
{
public:
	enum
	{
		e_count = 30
	};

	~HelloWorld();

	virtual bool init();
	virtual void draw(Renderer *renderer, const kmMat4& transform, bool transformUpdated);    //Layerのdrawメソッドをオーバーライド
	static Scene* scene();
	CREATE_FUNC(HelloWorld);

	void initPhysics();     //物理演算初期化用メソッド
	void update(float dt);
	//タッチイベント
	bool onTouchBegan(Touch* touch, Event* event);
	void onTouchMoved(Touch* touch, Event* event);
	//void onTouchEnded(Touch* touch, Event* event);

private:
	b2World *_world;
	GLESDebugDraw *_debugDraw;
	b2Body* m_middle;
};

#endif // __HELLOWORLD_SCENE_H__