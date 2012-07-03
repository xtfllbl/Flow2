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
    expandButton->setToolTip("Expand/Collapse");
    expandButton->setFlat(true);
    expandButton->setIcon(QIcon(":/src/images/expand.png"));
    expandButton->setIconSize(QSize(32,32));
    QIcon exIcon;
    exIcon.addFile(QString::fromUtf8(":/src/images/expand.png"), QSize(), QIcon::Normal, QIcon::Off);
    exIcon.addFile(QString::fromUtf8(":/src/images/collapse.png"), QSize(), QIcon::Normal, QIcon::On);
    expandButton->setIcon(exIcon);
    expandButton->setCheckable(true);
    expandButton->setChecked(true);

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

    this->setLayout(layout);
    this->setFrameShape(QFrame::StyledPanel);
    this->setFrameShadow(QFrame::Plain);

    connect(expandButton,SIGNAL(clicked(bool)),this,SLOT(emitSigExpand(bool)));

}

void QJDFuncationHeadWidget::emitSigExpand(bool checked)
{
    if(checked==true)
    {
        emit sigExpandClicked(1);
    }
    if(checked==false)
    {
        emit sigCollapseClicked();
    }
}

