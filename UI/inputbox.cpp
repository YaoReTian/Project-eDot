#include "inputbox.h"

#include "../Utils/global.h"

InputBox::InputBox(QGraphicsItem* parent)
    : QObject(), QGraphicsPixmapItem(parent), m_text(""), m_textBox(this), m_warning(this)
{
    m_warning.setPlainText("Invalid Input");
    QPixmap pix(":/image/res/textbox.png");
    setPixmap(pix.scaledToHeight(pix.height()*GLOBAL::Scale/2));
    m_textBox.setPos(22*GLOBAL::Scale/2, 40*GLOBAL::Scale/2);
    m_textBox.setTextWidth(418*GLOBAL::Scale/2);
    hide();

    QFont font("rainyhearts", 9 * GLOBAL::Scale);
    font.setBold(true);
    m_textBox.setDefaultTextColor(Qt::white);
    m_textBox.setFont(font);

    font.setPointSize(5*GLOBAL::Scale);
    m_warning.setDefaultTextColor(Qt::white);
    m_warning.setFont(font);
    m_warning.setPlainText("Invalid Input.");
    m_warning.setPos(230*GLOBAL::Scale/2 - m_warning.boundingRect().width()/2,
                             80*GLOBAL::Scale/2);
    m_warning.hide();
}

void InputBox::input(KeyMap *keys)
{
    if (keys->keyReleasedStatus(GLOBAL::ENTER))
    {
        if (m_text.length() < 1 ||
            m_text[0] == ' ')
        {
            m_warning.show();
            emit invalidInput();
        }
        else
        {
            emit textInputted(m_text);
        }
    }
    else if (keys->keyReleasedStatus(GLOBAL::DELETE))
    {
        m_text.removeLast();
    }
    else if (keys->lastKeystring().length() != 0)
    {
        m_text += keys->lastKeystring();
    }
}

void InputBox::update(int deltatime)
{
    if (m_warning.isVisible())
    {
        m_elapsedTime += deltatime;
        if (m_elapsedTime >= 1000)
        {
            m_warning.hide();
        }
    }
    m_textBox.setPlainText(m_text);
}

// SLOTS

void InputBox::hide()
{
    m_text = "";
    m_textBox.setPlainText("");
    m_warning.hide();
    QGraphicsPixmapItem::hide();
}

void InputBox::show()
{
    QGraphicsPixmapItem::show();
}
