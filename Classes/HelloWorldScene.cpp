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

	//�E�B���h�E�T�C�Y�擾
	Size visibleSize = Director::getInstance()->getVisibleSize();
	
	//�^�b�`�C�x���g
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
//�������Z������
void HelloWorld::initPhysics()
{
	//�d�͐ݒ�
	b2Vec2 gravity;
	gravity.Set(0.0f, -10.0f);
	//���[���h�쐬
	_world = new b2World(gravity);

	//���̂��Î~�������̉��Z����
	_world->SetAllowSleeping(true);
	//���̂������łԂ��������Ɋђʂ��Ȃ�
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

	b2BodyDef bodyDef;	//b2BodyDef�\����
	bodyDef.type = b2_dynamicBody;	//���I�ȕ��̂�
	bodyDef.userData = ballSprite;	//userData�ɃX�v���C�g�o�^
	bodyDef.position.Set(300 / PTM_RATIO, 300 / PTM_RATIO);	//�ʒu��ݒ�

	b2Body * body = _world->CreateBody(&bodyDef);	//world����Body���쐬

	b2CircleShape circle;	//�`���`����N���X
	circle.m_radius = 50 / PTM_RATIO;	//���a��50px��

	b2FixtureDef fixtureDef;	//Fixture�̒�`������\����
	fixtureDef.shape = &circle;	//�`
	fixtureDef.density = 0.4;	//���x
	fixtureDef.friction = 0.5;	//���C��
	fixtureDef.restitution = 0.6;//�����W��

	body->CreateFixture(&fixtureDef);	//Body����Fixture���쐬

	b2BodyDef boxBodyDef;
	boxBodyDef.position.Set(300 / PTM_RATIO, 100 / PTM_RATIO);
	b2Body *boxBody = _world->CreateBody(&boxBodyDef);	//Body���쐬

	b2PolygonShape rect;	//�����`�쐬
	rect.SetAsBox(200 / PTM_RATIO, 50 / PTM_RATIO);	//��400px,����100px

	b2FixtureDef boxFixtureDef;
	boxFixtureDef.shape = &rect;		//�`
	boxFixtureDef.density = 0.4;		//���x
	boxFixtureDef.friction = 0.5;		//���C��;
	boxFixtureDef.restitution = 1.0;	//�����W��1.0;
	boxBody->CreateFixture(&boxFixtureDef);     //Fixture���쐬

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
	float32 timeStep = 1.0f / 60.0f;		//���Z���x�Œ�
	int32 velocityIterations = 10;			//���x�v�Z���x
	int32 positionIterations = 10;			//�ʒu�v�Z���x

	_world->Step(timeStep,velocityIterations,positionIterations);	//�t���[�����Ƃ�World�̏��X�V
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

//�^�b�v�J�n��
bool HelloWorld::onTouchBegan(Touch* touch, Event* event)
{
	return true;
}
//��ʃ^�b�v
void HelloWorld::onTouchMoved(Touch* touch, Event* event)
{
	Point touch_point = touch->getLocation();
	log("X:%f Y:%f", touch_point.x, touch_point.y);
}

HelloWorld::~HelloWorld()
{
	delete _debugDraw;			//DebugDraw���J��
	_debugDraw = NULL;

	delete _world;				//World���J��
	_world = NULL;
}
