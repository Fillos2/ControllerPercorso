#include "block.h"
#include <controllerpercorso.h>
Block::Block(TypeBlock tipo,QWidget * parent)
{
    QString fileName;
    type = tipo;
    this->setParent(parent);
    this->setFrameShadow(QFrame::Raised);
    this->setFrameShape(QFrame::StyledPanel);
    this->setGeometry(QRect(250,250 , 80, 80));
    this->setMouseTracking(true);
    switch (tipo) {
    case Start:fileName = ":icone progetto percorso/stock-vector-start-flag-illustration-26697331.jpg";
               this->setGeometry(QRect(20,100,80,80));
        break;
    case Avanti: fileName = ":icone progetto percorso/dritto.jpg";

        break;
    case Sx:fileName = ":icone progetto percorso/sx.jpg";

        break;
    case Dx:fileName = ":icone progetto percorso/dx.jpg";

        break;
    case Loop:fileName = ":icone progetto percorso/loop.jpg";

        break;
    case End :fileName = ":icone progetto percorso/stock-vector-end-flag-illustration-26697307.jpg";
              this->setGeometry(QRect(510,410,80,80));
        break;
    case Continue1:fileName = ":icone progetto percorso/continue.jpg";

        break;
    case Continue2:fileName = ":icone progetto percorso/continue2.jpg";

        break;
    }
    Label = new QLabel(this);
    Label->setGeometry(QRect(0,0,80,80));
    Label->setBackgroundRole(QPalette::Base);

    Label->setScaledContents(true);

    if (!fileName.isEmpty()) {
        QImage image(fileName);
        if (image.isNull()) {
            QMessageBox::information(this, tr("Image Viewer"),
                                     tr("Cannot load %1.").arg(fileName));
            return;
        }


    Label->setPixmap(QPixmap::fromImage(image));
    }
    this->show();
    Label->show();


}

Block::~Block()
{

}

bool Block::controlla_posiz(int x, int y,QMouseEvent * event)
{
    int xM=x+160,xm=x+80,yM=y+80;
    QPoint mouse = mapToParent(event->pos());

    if( (mouse.x()>xm) && (mouse.x()<xM) && (mouse.y()>y) && (mouse.y()<yM)){
        return true;
    }
    return false;
}

void Block::setPosizione(int x, int y)
{
    this->setGeometry(x,y,80,80);
}

void Block::mousePressEvent(QMouseEvent * event)
{
    if(type != Start){
    ControllerPercorso::Percorso.removeAll(this);
    ControllerPercorso::Percorso.last()->setEnabled(true);
    }
    offset = event->pos();
}

void Block::mouseMoveEvent(QMouseEvent * event)
{
    if(type != Start){
        if(event->buttons() & Qt::LeftButton ){
            this->move(mapToParent(event->pos() - offset));
        }
   }
}

void Block::mouseReleaseEvent(QMouseEvent *event)
{

    Block * lastBlock = ControllerPercorso::Percorso.last();
    if(controlla_posiz(lastBlock->x(),lastBlock->y(),event)){

         this->setPosizione(lastBlock->x()+80,lastBlock->y());
         lastBlock->setEnabled(false);
         ControllerPercorso::Percorso.push_back(this);
    }
}
