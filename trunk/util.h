#ifndef KLUDGETUTIL_H
#define KLUDGETUTIL_H

#include <QtGui>
#include <QDomDocument>

class Util
{
public:
    static bool copyDir(const QString &source, const QString &destination, const bool overwrite);
    static bool deleteDir(const QString &source);
    static bool extract(const QString &source, const QString &destination);
};

class ImageUtil
{
public:
    static QImage blendImages(const QImage &image1, double alpha1, const QImage &image2, double alpha2);
    static QImage grayedImage(const QImage &image);
    static QImage tintedImage(const QImage &image, const QColor &color, QPainter::CompositionMode mode = QPainter::CompositionMode_Screen);
};

class WindowUtil
{
public:
    static void center(QWidget *w, int offsetX = 0, int offsetY = 0);
};

#endif // KLUDGETUTIL_H