#include "qjdareaheadwidget.h"
#include <QLayout>
#include <QPainter>
#include <QDebug>
#include <QPicture>
QJDAreaHeadWidget::QJDAreaHeadWidget(QFrame *parent) :
    QFrame(parent)
{
    this->setMaximumHeight(55);
    textLabel=new QLabel;
    textLabel->setText("Folders:");
    horizontalSpacer = new QSpacerItem(400, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
    refreshButton=new QJDPushButton;
    refreshButton->setToolTip("Refresh");
    refreshButton->setIcon(QIcon(":/src/images/refresh.png"));
    refreshButton->setFlat(true);
    refreshButton->setIconSize(QSize(32,32));
    expandButton=new QJDPushButton;
    expandButton->setToolTip("Expand All");
    expandButton->setIcon(QIcon(":/src/images/expand.png"));
    expandButton->setFlat(true);
    expandButton->setIconSize(QSize(32,32));
    collapseButton=new QJDPushButton;
    collapseButton->setToolTip("Collapse All");
    collapseButton->setIcon(QIcon(":/src/images/collapse.png"));
    collapseButton->setFlat(true);
    collapseButton->setIconSize(QSize(32,32));


    QPushButton *pixButton=new QPushButton;
    pixButton->setMinimumSize(32,32);
    QIcon icon;
    icon.addFile(QString::fromUtf8(":/src/images/home.png"), QSize(), QIcon::Normal, QIcon::Off);
    icon.addFile(QString::fromUtf8(":/src/images/home.png"), QSize(), QIcon::Disabled, QIcon::Off);
    pixButton->setIcon(icon);
    pixButton->setFlat(true);
    pixButton->setEnabled(false);

    QHBoxLayout *layout=new QHBoxLayout();
    layout->addWidget(pixButton);
    layout->addWidget(textLabel);
    layout->addItem(horizontalSpacer);
    layout->addWidget(refreshButton);
    layout->addWidget(expandButton);
    layout->addWidget(collapseButton);

    this->setLayout(layout);
    this->setFrameShape(QFrame::StyledPanel);
    this->setFrameShadow(QFrame::Plain);

    connect(refreshButton,SIGNAL(pressed()),this,SLOT(emitSigRefresh()));
    connect(expandButton,SIGNAL(pressed()),this,SLOT(emitSigExpand()));
    connect(collapseButton,SIGNAL(pressed()),this,SLOT(emitSigCollapse()));
}

void QJDAreaHeadWidget::emitSigRefresh()
{
    emit sigRefreshClicked();
}

void QJDAreaHeadWidget::emitSigExpand()
{
    emit sigExpandClicked();
}

void QJDAreaHeadWidget::emitSigCollapse()
{
    emit sigCollapseClicked();
}
