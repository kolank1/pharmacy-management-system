#ifndef STYLEMANAGER_H
#define STYLEMANAGER_H

#include <QPushButton>
#include <QList>

class StyleManager
{
public:
    static void setActiveButton(QPushButton *activeButton,
                                const QList<QPushButton*> &buttons);
    static void setBlueButton(QPushButton *button);
    static void setDeleteButton(QPushButton *button);

};

#endif