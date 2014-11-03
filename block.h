#ifndef BLOCK_H
#define BLOCK_H
#include <QtWidgets>
#include <QFrame>
#include <QLabel>
#include <QPoint>

enum TypeBlock{
    Start,
    Avanti,
    Sx,
    Dx,
    Loop,
    End,
    Continue1,
    Continue2
};

class Block: public QFrame
{
public:
    Block(TypeBlock tipo, QWidget *parent);
    ~Block();
    TypeBlock type;
    bool controlla_posiz(int x, int y, QMouseEvent *event);
    void setPosizione(int x,int y);
protected:

    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    QPoint offset;
private:
    QLabel * Label;



};

#endif // BLOCK_H
