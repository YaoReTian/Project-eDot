#include "sprite.h"

#include "../tileset.h"

Sprite::Sprite(QGraphicsItem * parent)
    : QObject(), GameItem(parent), m_SpriteID(-1), m_name("Unset"),
    m_type("Unset"), m_frameSize(0,0), m_elapsed_time(0), m_currentFrame(0),
    m_currentStateName("idle"), m_animationComplete(false), m_prevActiveVector(0,1),
    m_WALK_SPEED(2.0f/1000.0f), m_RUN_SPEED(3.0f/1000.0f),
    m_defaultSpeed(m_RUN_SPEED), m_currentSpeed(m_defaultSpeed),
    m_leftTransform(1,0,0,1,1,1)
{}

Sprite::~Sprite()
{
    qDeleteAll(m_hitboxes);
    m_hitboxes.clear();
    qDeleteAll(m_states);
    m_states.clear();
}

void Sprite::setID(int ID)
{
    m_SpriteID = ID;
}

void Sprite::setName(QString name)
{
    m_name = name;
}

void Sprite::setType(QString type)
{
    m_type = type;
}

void Sprite::setTransform(QTransform transform)
{
    m_transform = transform;
}

void Sprite::showHitbox()
{
    for (const auto h : std::as_const(m_hitboxes))
    {
        h->setPen(QPen(Qt::yellow));
    }
}

void Sprite::hideHitbox()
{
    for (const auto h : std::as_const(m_hitboxes))
    {
        h->setPen(QPen(Qt::transparent));
    }
}

int Sprite::getID()
{
    return m_SpriteID;
}

QString Sprite::getName()
{
    return m_name;
}

QString Sprite::getType()
{
    return m_type;
}

QTransform Sprite::transform()
{
    return m_transform;
}

void Sprite::setSpriteSheet(QPixmap spriteSheet)
{
    m_spriteSheet = spriteSheet;
}

void Sprite::setFrameSize(QSize frameSize)
{
    m_frameSize = frameSize;
}

void Sprite::setHitbox(HitboxInfo* hitbox)
{
    m_hitboxes.append(new Hitbox(this));
    m_hitboxes.back()->setPen(QPen(Qt::transparent));
    m_hitboxes.back()->setRect(-hitbox->m_width*GLOBAL::Scale/2,
                               -hitbox->m_height*GLOBAL::Scale/2,
                               hitbox->m_width * GLOBAL::Scale,
                               hitbox->m_height * GLOBAL::Scale);
    m_hitboxes.back()->m_solid = hitbox->m_solid;
    m_hitboxes.back()->m_interactable = hitbox->m_interactable;
    m_hitboxes.back()->setTransform(m_transform);
    m_hitboxes.back()->m_mapLinkID = hitbox->m_mapLinkID;
    m_hitboxes.back()->setPos((hitbox->m_x+hitbox->m_width/2)*GLOBAL::Scale,
                              (hitbox->m_y+hitbox->m_height/2)*GLOBAL::Scale);
}

void Sprite::addAnimationState(QString stateName, int line, float frameTime)
{
    QPixmap current;
    QColor back(0,255,61);
    m_states[stateName] = new AnimationState;
    m_states[stateName]->stateName = stateName;
    m_states[stateName]->frameTime = frameTime;

    int y = line*m_frameSize.height();
    int x = m_frameSize.width();

    current = m_spriteSheet.copy(0, y, m_frameSize.width(), m_frameSize.height());
    current = current.scaled(m_frameSize.width()*GLOBAL::Scale, m_frameSize.height()*GLOBAL::Scale);
    m_states[stateName]->frames.append(current);

    while(current.toImage().pixelColor(m_frameSize.width()/2, m_frameSize.height()/2) != back  &&
           x < m_spriteSheet.width())
    {
        m_states[stateName]->frames.append(current.scaled(m_frameSize.width()*GLOBAL::Scale,
                                                          m_frameSize.height()*GLOBAL::Scale));
        current = m_spriteSheet.copy(x, y, m_frameSize.width(), m_frameSize.height());
        x += m_frameSize.width();
    }
}

void Sprite::addTransition(QString startStateName, GLOBAL::Action action, QString endStateName)
{
    m_states[startStateName]->transitions[action] = endStateName;
}

void Sprite::update(int deltaTime)
{
    m_elapsed_time += deltaTime;

    // Check wall collisions
    QPointF prevPos;
    Vector v = vector();
    v.toUnitVector();
    v *= m_currentSpeed;

    for (int i = 0; i < 10; i++)
    {
        prevPos = pos();
        setX(x()+(v.i() * GLOBAL::ObjectLength * deltaTime)/10);
        if (collidedWithWall()) setPos(prevPos);
    }
    for (int i = 0; i < 10; i++)
    {
        prevPos = pos();
        setY(y()+(v.j() * GLOBAL::ObjectLength * deltaTime)/10);
        if (collidedWithWall()) setPos(prevPos);
    }

    if (!v.null())
    {
        m_prevActiveVector = v;
        if (v.i() < 0)
        {
            m_leftTransform = QTransform(-1,0,0,1,1,1);
        }
        else
        {
            m_leftTransform = QTransform( 1,0,0,1,1,1);
        }
    }

    if (m_states[m_currentStateName]->frameTime == -1)
    {
        setPixmap(m_states[m_currentStateName]->frames[m_currentFrame].transformed(m_leftTransform*m_transform));
    }
    else if (m_states[m_currentStateName]->frameTime <= m_elapsed_time)
    {
        m_animationComplete = false;
        m_elapsed_time = 0;
        m_currentFrame++;
        if (m_currentFrame == m_states[m_currentStateName]->frames.size() -1)
        {
            m_animationComplete = true;
            emit animationComplete();
        }
        if (m_currentFrame  == m_states[m_currentStateName]->frames.size())
        {
            m_currentFrame = 0;
        }
        setPixmap(m_states[m_currentStateName]->frames[m_currentFrame].transformed(m_leftTransform*m_transform));
    }

    setVector(0,0);
    GameItem::update(deltaTime);
}

void Sprite::setAction(GLOBAL::Action action)
{
    // Animations
    if (m_states[m_currentStateName]->transitions.contains(action))
    {
        m_currentStateName = m_states[m_currentStateName]->transitions[action];
        m_currentFrame = 0;
        m_animationComplete = false;
    }
    else if (m_states[m_currentStateName]->transitions.contains(GLOBAL::ANIMATION_DONE) && m_animationComplete)
    {
        m_currentStateName = m_states[m_currentStateName]->transitions[GLOBAL::ANIMATION_DONE];
        m_currentFrame = 0;
        m_animationComplete = false;
    }

    // Movement
    if (action == GLOBAL::MOVE_LEFT)
    {
        setI(-1);
    }
    else if (action == GLOBAL::MOVE_RIGHT)
    {
        setI(1);
    }
    else if (action == GLOBAL::MOVE_UP)
    {
        setJ(-1);
    }
    else if (action == GLOBAL::MOVE_DOWN)
    {
        setJ(1);
    }
}

void Sprite::setDefaultToWalk()
{
    m_defaultSpeed = m_WALK_SPEED;
    m_currentSpeed = m_defaultSpeed;
}

bool Sprite::animationDone()
{
    return m_animationComplete;
}

void Sprite::setDefaultToRun()
{
    m_defaultSpeed = m_RUN_SPEED;
    m_currentSpeed = m_defaultSpeed;
}

bool Sprite::collidedWithWall()
{
    for (const auto h : std::as_const(m_hitboxes))
    {
        QList<QGraphicsItem*> list = h->collidingItems();

        for (const auto s : std::as_const(list))
        {
            if (typeid(*s) == typeid(Hitbox))
            {
                Hitbox* h = dynamic_cast<Hitbox*>(s);
                if (h->m_mapLinkID != -1) emit collidedWithLink(h->m_mapLinkID);
                if (h->m_solid) return true;
            }
        }
    }
    return false;
}

Vector Sprite::prevActiveVector()
{
    return m_prevActiveVector;
}


QSize Sprite::frameSize()
{
    return m_frameSize;
}
