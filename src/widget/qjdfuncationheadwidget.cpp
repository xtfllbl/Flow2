#include "qjdfuncationheadwidget.h"
#include <QLayout>

QJDFuncationHeadWidget::QJDFuncationHeadWidget(QFrame *parent) :
    QFrame(parent)
{
    this->setMaximumHeight(55);

    textLabel=new QLabel;
    textLabel->setText("Funcation List:");
//    textLabel->setPixmap();
    horizontalSpacer = new QSpacerItem(400, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
    expandButton=new QJDPushButton;
    expandButton->setToolTip("Expand All");
    expandButton->setFlat(true);
    expandButton->setIcon(QIcon(":/src/images/expand.png"));
    expandButton->setIconSize(QSize(32,32));
    collapseButton=new QJDPushButton;
    collapseButton->setToolTip("Collapse All");
    collapseButton->setFlat(true);
    collapseButton->setIcon(QIcon(":/src/images/collapse.png"));
    collapseButton->setIconSize(QSize(32,32));

    QPushButton *pixButton=new QPushButton;
    pixButton->setMinimumSize(32,32);
    QIcon icon;
    icon.addFile(QString::fromUtf8(":/src/images/property.png"), QSize(), QIcon::Normal, QIcon::Off);
    icon.addFile(QString::fromUtf8(":/src/images/property.png"), QSize(), QIcon::Disabled, QIcon::Off);
    pixButton->setIcon(icon);
    pixButton->setFlat(true);
    pixButton->setEnabled(false);

    QHBoxLayout *layout=new QHBoxLayout();
    layout->addWidget(pixButton);
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

void QJDFuncationHeadWidget::emitSigExpand()
{
    emit sigExpandClicked();
}

void QJDFuncationHeadWidget::emitSigCollapse()
{
    emit sigCollapseClicked();
}
