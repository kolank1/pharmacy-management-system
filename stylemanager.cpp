#include "stylemanager.h"
#include <QStyle>

void StyleManager::setActiveButton(
    QPushButton *activeButton,
    const QList<QPushButton*> &buttons)
{
    for(QPushButton *button : buttons)
    {
        button->setProperty("active", false);
        button->style()->unpolish(button);
        button->style()->polish(button);
    }

    if(activeButton)
    {
        activeButton->setProperty("active", true);
        activeButton->style()->unpolish(activeButton);
        activeButton->style()->polish(activeButton);
    }
}

void StyleManager::setBlueButton(QPushButton *button)
{
    button->setFixedSize(100,28);
    button->setStyleSheet(
        "QPushButton{"
        "background-color:#6C4DFF;"
        "color:white;"
        "border:none;"
        "border-radius:8px;"
        "padding:6px 14px;"
        "font-size:13px;"
        "font-weight:600;"
        "}"
        "QPushButton:hover{"
        "background-color:#7A5CFF;"
        "}"
        "QPushButton:pressed{"
        "background-color:#5637E8;"
        "}"
        );
}



