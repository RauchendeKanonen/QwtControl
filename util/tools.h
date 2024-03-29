#ifndef TOOLS_H
#define TOOLS_H
#include <QString>
#include <QStringList>

QStringList findCharacterStrings(QString Buffer);
void toComplexExpression(QString *Buffer);
void replaceComputerNotation(QString *Buffer);
void castConstTo(QString *Buffer, QString Cast);
QString inverseZTransform(QString ZDExpression);
#endif // TOOLS_H
