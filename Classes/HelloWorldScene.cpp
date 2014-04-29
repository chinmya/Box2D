#include "HelloWorldScene.h"

CCScene* HelloWorld::scene()
{
	CCScene *scene = CCScene::create();
	HelloWorld *layer = HelloWorld::create();
	scene->addChild(layer);
	return scene;
}

bool HelloWorld::init()
{
	if (!CCLayer::init())
	{
		return false;
	}

	//ウィンドウサイズ取得
	Size visibleSize = Director::getInstance()->getVisibleSize();
	
	//タッチイベント
	auto listener = EventListenerTouchOneByOne::create();
	listener->setSwallowTouches(true);

	listener->onTouchBegan = CC_CALLBACK_2(HelloWorld::onTouchBegan, this);
	listener->onTouchMoved = CC_CALLBACK_2(HelloWorld::onTouchMoved, this);
	listener->onTouchEnded = CC_CALLBACK_2(HelloWorld::onTouchEnded, this);

	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	this->initPhysics();

	scheduleUpdate();

	return true;
}
//物理演算初期化
void HelloWorld::initPhysics()
{
	//重力設定
	b2Vec2 gravity;
	gravity.Set(0.0f, -10.0f);
	//ワールド作成
	_world = new b2World(gravity);

	//物体が静止した時の演算無視
	_world->SetAllowSleeping(true);
	//物体が高速でぶつかった時に貫通しない
	_world->SetContinuousPhysics(true);

	_debugDraw = new GLESDebugDraw(PTM_RATIO);
	_world->SetDebugDraw(_debugDraw);

	uint32 flags = 0;
	flags += b2Draw::e_shapeBit;
	//        flags += b2Draw::e_jointBit;
	//        flags += b2Draw::e_aabbBit;
	//        flags += b2Draw::e_pairBit;
	//        flags += b2Draw::e_centerOfMassBit;
	_debugDraw->SetFlags(flags);

	Sprite* ballSprite = Sprite::create("ball.png");
	ballSprite->setPosition(ccp(300, 300));
	this->addChild(ballSprite);

	b2BodyDef bodyDef;	//b2BodyDef構造体
	bodyDef.type = b2_dynamicBody;	//動的な物体に
	bodyDef.userData = ballSprite;	//userDataにスプライト登録
	bodyDef.position.Set(300 / PTM_RATIO, 300 / PTM_RATIO);	//位置を設定

	b2Body * body = _world->CreateBody(&bodyDef);	//worldからBodyを作成

	b2CircleShape circle;	//形を定義するクラス
	circle.m_radius = 50 / PTM_RATIO;	//半径を50pxに

	b2FixtureDef fixtureDef;	//Fixtureの定義を入れる構造体
	fixtureDef.shape = &circle;	//形
	fixtureDef.density = 0.4;	//密度
	fixtureDef.friction = 0.5;	//摩擦率
	fixtureDef.restitution = 0.6;//反発係数

	body->CreateFixture(&fixtureDef);	//BodyからFixtureを作成

	b2BodyDef boxBodyDef;
	boxBodyDef.position.Set(300 / PTM_RATIO, 100 / PTM_RATIO);
	b2Body *boxBody = _world->CreateBody(&boxBodyDef);	//Bodyを作成

	b2PolygonShape rect;	//長方形作成
	rect.SetAsBox(200 / PTM_RATIO, 50 / PTM_RATIO);	//横400px,高さ100px

	b2FixtureDef boxFixtureDef;
	boxFixtureDef.shape = &rect;		//形
	boxFixtureDef.density = 0.4;		//密度
	boxFixtureDef.friction = 0.5;		//摩擦率;
	boxFixtureDef.restitution = 1.0;	//反発係数1.0;
	boxBody->CreateFixture(&boxFixtureDef);     //Fixtureを作成

}

void HelloWorld::draw(Renderer *renderer, const kmMat4& transform, bool transformUpdated)
{
	ccGLEnableVertexAttribs(kCCVertexAttribFlag_Position);
	kmGLPushMatrix();
	_world->DrawDebugData();
	kmGLPopMatrix();
}

void HelloWorld::update(float dt)
{
	float32 timeStep = 1.0f / 60.0f;		//演算速度固定
	int32 velocityIterations = 10;			//速度計算密度
	int32 positionIterations = 10;			//位置計算密度

	_world->Step(timeStep,velocityIterations,positionIterations);	//フレームごとにWorldの情報更新
	for (b2Body* b = _world->GetBodyList(); b; b = b->GetNext())
	{
		if (b->GetUserData() != NULL)
		{
			Sprite* myActor = (Sprite*) b->GetUserData();
			myActor->setPosition(CCPointMake(b->GetPosition().x * PTM_RATIO, b->GetPosition().y * PTM_RATIO));
			myActor->setRotation(-1 * CC_RADIANS_TO_DEGREES(b->GetAngle()));
		}
	}
}

//タップ開始時
bool HelloWorld::onTouchBegan(Touch* touch, Event* event)
{
	return true;
}
//画面タップ
void HelloWorld::onTouchMoved(Touch* touch, Event* event)
{
	Point touch_point = touch->getLocation();
	log("X:%f Y:%f", touch_point.x, touch_point.y);
}

HelloWorld::~HelloWorld()
{
	delete _debugDraw;			//DebugDrawを開放
	_debugDraw = NULL;

	delete _world;				//Worldを開放
	_world = NULL;
}
