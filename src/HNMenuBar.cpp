#include "HNMenuBar.h"
#include "HNMenu.h"
#include <QDebug>

QT_BEGIN_NAMESPACE

HNMenuBar::HNMenuBar(HNData *data) : QPlatformMenuBar(), m_data(data)
{
    m_object = hn_top_bar_create(m_data->heaven, this);
}

HNMenuBar::~HNMenuBar()
{
    qDebug() << "Destroy menu bar" << this;
}

void HNMenuBar::insertMenu(QPlatformMenu *menu, QPlatformMenu *before)
{
    qDebug() << "HNMenuBar::insertMenu" << menu << before;

    if(menu)
    {
        HNMenu *m = (HNMenu*)menu;

        if(before)
        {
            HNMenu *b = (HNMenu*)before;
            hn_menu_add_to_top_bar((hn_menu*)m->object(), (hn_top_bar*)object(), (hn_menu*)b->object());
        }
        else
            hn_menu_add_to_top_bar((hn_menu*)m->object(), (hn_top_bar*)object(), NULL);
    }

}

void HNMenuBar::removeMenu(QPlatformMenu *menu)
{
    qDebug() << "HNMenuBar::removeMenu" << menu;

    if(menu)
    {
        HNMenu *m = (HNMenu*)menu;
        hn_object_remove_fom_parent(m->object());
    }

}

void HNMenuBar::syncMenu(QPlatformMenu *menuItem)
{
    qDebug() << "HNMenuBar::syncMenu" << menuItem;
}

void HNMenuBar::handleReparent(QWindow *newParentWindow)
{
    qDebug() << "HNMenuBar::handleReparent" << newParentWindow;

    if(m_window)
        disconnect(m_window, &QWindow::activeChanged, this, &HNMenuBar::windowActivatedChanged);

    m_window = newParentWindow;

    if(m_window)
    {
        connect(m_window, &QWindow::activeChanged, this, &HNMenuBar::windowActivatedChanged);
        //if(m_window->isActive())
            //hn_top_bar_set_active((hn_top_bar*)m_object);
    }
}

QPlatformMenu *HNMenuBar::menuForTag(quintptr tag) const
{
    qDebug() << "HNMenuBar::menuForTag" << tag;

    hn_object *obj = hn_object_get_by_id(m_data->heaven, (hn_object_id)tag);

    if(obj && hn_object_get_type(obj) == HN_OBJECT_TYPE_MENU)
        return (QPlatformMenu*)hn_object_get_user_data(obj);

    return nullptr;
}

QPlatformMenu *HNMenuBar::createMenu() const
{
    qDebug() << "HNMenuBar::createMenu";

    return new HNMenu(m_data);
}

hn_object *HNMenuBar::object() const
{
    return m_object;
}

void HNMenuBar::windowActivatedChanged()
{
    if(m_window->isActive())
    {
        qDebug() << "New acttive window";
        hn_top_bar_set_active((hn_top_bar*)m_object);
    }
}


QT_END_NAMESPACE

