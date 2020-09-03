#pragma once
#include <QObject>
#include <QQuickItem>
#include <Settings.h>

class getVariable : public QObject
{
    Q_OBJECT

public:
    explicit getVariable(QObject* parent = 0);

public:
    Q_INVOKABLE int get(const QString& what, const int& arg = 0, const int& arg1 = 0) const;

};

