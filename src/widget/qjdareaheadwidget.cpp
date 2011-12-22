#include "qjdareaheadwidget.h"
#include <QLayout>
#include <QPainter>
#include <QDebug>
QJDAreaHeadWidget::QJDAreaHeadWidget(QFrame *parent) :
    QFrame(parent)
{
    textLabel=new QLabel;
    horizontalSpacer = new QSpacerItem(400, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
    expandButton=new QToolButton;
    collapseButton=new QToolButton;

    textLabel->setText("Folders:");
    expandButton->setText("Expand");
    collapseButton->setText("Collapse");

    QHBoxLayout *layout=new QHBoxLayout();
    layout->addWidget(textLabel);
    layout->addItem(horizontalSpacer);
    layout->addWidget(expandButton);
    layout->addWidget(collapseButton);

    this->setLayout(layout);
    this->setFrameShape(QFrame::StyledPanel);
    this->setFrameShadow(QFrame::Plain);

    connect(expandButton,SIGNAL(pressed()),this,SLOT(emitSigExpand()));
    connect(collapseButton,SIGNAL(pressed()),this,SLOT(emitSigCollapse()));
}

void QJDAreaHeadWidget::emitSigExpand()
{
    emit sigExpandClicked();
}

void QJDAreaHeadWidget::emitSigCollapse()
{
    emit sigCollapseClicked();
}
