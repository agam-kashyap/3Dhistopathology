#include <inviwo/qt/widgets/editablelabelqt.h>

namespace inviwo{

EditableLabelQt::EditableLabelQt(std::string text){
    text_ = text;
    generateWidget();
}

void EditableLabelQt::generateWidget(){
    QHBoxLayout* hLayout = new QHBoxLayout();
    label_ = new QLabel();
    label_->setText(QString::fromStdString(shortenText()));
    lineEdit_ = new QLineEdit();
    hLayout->addWidget(lineEdit_);
    lineEdit_->hide();
    lineEdit_->setAlignment(Qt::AlignLeft);
    hLayout->addWidget(label_);
    setLayout(hLayout);

    contextMenu_ = new QMenu();
    contextMenu_->addAction("Rename");

    label_->setContextMenuPolicy(Qt::CustomContextMenu);
  

    connect(label_,SIGNAL(customContextMenuRequested(const QPoint&)),this,SLOT(showContextMenu(const QPoint&)));
    connect(lineEdit_, SIGNAL(editingFinished()),this, SLOT(editingOff()));
}

void EditableLabelQt::edit(){
    label_->hide();
    lineEdit_->setText(QString::fromStdString(text_));
    lineEdit_->show();
    lineEdit_->setContentsMargins(0,0,0,0);
    lineEdit_->setFocus();
    lineEdit_->selectAll();
}


void EditableLabelQt::mouseDoubleClickEvent( QMouseEvent* e ){
    edit();
}

void EditableLabelQt::editingOff(){
    lineEdit_->hide();
    text_ = lineEdit_->text().toLocal8Bit().constData();
    label_->setText(QString::fromStdString(shortenText()));    
    label_->show();
    emit textChanged();
}

void EditableLabelQt::showContextMenu( const QPoint& pos ){
    QPoint globalPos = label_->mapToGlobal(pos);
    QAction* selecteditem = contextMenu_->exec(globalPos);
    if (selecteditem == contextMenu_->actions().at(0)) {
        edit();
    }
}

std::string EditableLabelQt::shortenText(){
    if (text_.length()>25) {
        std::string shortText = text_.substr(0,15)+"..."+text_.substr(text_.length()-7,7);
        label_->setToolTip(QString::fromStdString(text_));
        return shortText;
    }else{
        label_->setToolTip(QString::fromStdString(""));
        return text_;
    }
    
}


}//namespace