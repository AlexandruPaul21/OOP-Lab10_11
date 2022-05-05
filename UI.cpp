//
// Created by Alexandru-Paul Sirbu on 01.05.2022.
//

#include "UI.h"

void GUI::init_GUI() {
    setLayout(lyMain);

    QWidget *w_left = new QWidget;
    QVBoxLayout *l_left = new QVBoxLayout;
    w_left->setLayout(l_left);
    lst = new QListWidget;
    l_left->addWidget(lst);

    QWidget *wdg=new QWidget;
    QHBoxLayout *lay=new QHBoxLayout;
    wdg->setLayout(lay);
    btnSortName=new QPushButton("Sort by Name");
    btnSortProd=new QPushButton("Sort by Prod");
    btnSortSP  =new QPushButton("Sort by Subst");
    lay->addWidget(btnSortName);
    lay->addWidget(btnSortProd);
    lay->addWidget(btnSortSP);

    l_left->addWidget(wdg);

//    QWidget *flt_btn=new QWidget;
//    QHBoxLayout *flt_lay=new QHBoxLayout;
//    flt_btn->setLayout(flt_lay);
//
//    btnFilterPrice=new QPushButton("Filtrare dupa pret");
//    btnFilterSubst=new QPushButton("Filtrare dupa subst");
//
//    flt_lay->addWidget(btnFilterSubst);
//    flt_lay->addWidget(btnFilterPrice);
//
//    l_left->addWidget(flt_btn);

    lyMain->addWidget(w_left);

    //form pentru detalii
    QWidget *w_right=new QWidget;
    QVBoxLayout *lay_right=new QVBoxLayout;
    w_right->setLayout(lay_right);

    QWidget *form=new QWidget;
    QFormLayout *lay_form=new QFormLayout;
    form->setLayout(lay_form);
    QLabel *name=new QLabel("Name");
    txtName=new QLineEdit;
    QLabel *prod=new QLabel("Prod");
    txtProd=new QLineEdit;
    QLabel *subst=new QLabel("Subst");
    txtSubst=new QLineEdit;
    QLabel *price=new QLabel("Price");
    txtPrice=new QLineEdit;

    lay_form->addRow(name,txtName);
    lay_form->addRow(prod,txtProd);
    lay_form->addRow(subst,txtSubst);
    lay_form->addRow(price,txtPrice);

    lay_right->addWidget(form);

    QWidget *but_zone=new QWidget;
    QVBoxLayout *lay_but=new QVBoxLayout;
    but_zone->setLayout(lay_but);

    btn_add=new QPushButton("Add");
    lay_but->addWidget(btn_add);

    QWidget *sec_but=new QWidget;
    QHBoxLayout *sec_lay=new QHBoxLayout;
    sec_but->setLayout(sec_lay);

    btn_del=new QPushButton("Delete");
    btn_mod=new QPushButton("Update");
    sec_lay->addWidget(btn_del);
    sec_lay->addWidget(btn_mod);

    lay_but->addWidget(sec_but);

    btn_undo=new QPushButton("Undo");
    lay_but->addWidget(btn_undo);

    lay_right->addWidget(but_zone);

    lyMain->addWidget(w_right);
    lb_index=-1;

    opt_but=new QWidget;
    lay_opt=new QVBoxLayout;
    opt_but->setLayout(lay_opt);
    lyMain->addWidget(opt_but);
}

void GUI::connectSignalsSlots() {
    QObject::connect(lst,&QListWidget::itemSelectionChanged,[&](){
        auto sel=lst->selectedItems();
        if(sel.empty()){
            txtName->setText("");
            txtProd->setText("");
            txtSubst->setText("");
            txtPrice->setText("");
        } else {
            auto sel_item=sel.at(0);
            auto name=sel_item->text();
            auto prod=sel_item->data(Qt::UserRole).toString();
            Medicine to_print;
            int index=0;
            for(const auto& med : srv.get_all_ent()){
                if(med.get_name()==name.toStdString() && med.get_prod()==prod.toStdString()){
                    to_print=med;
                    lb_index=index;
                }
                ++index;
            }
            txtName->setText(name);
            txtProd->setText(prod);
            txtSubst->setText(QString::fromStdString(to_print.get_subst()));
            txtPrice->setText(QString::number(to_print.get_price()));
        }
    });

    QObject::connect(btnSortName,&QPushButton::clicked,[&](){
        vector<Medicine> meds;
        meds.resize(srv.get_all_ent().size());
        srv.sort(0,meds);
        reloadList(meds);
    });

    QObject::connect(btnSortProd,&QPushButton::clicked,[&](){
        vector<Medicine> meds;
        meds.resize(srv.get_all_ent().size());
        srv.sort(1,meds);
        reloadList(meds);
    });

    QObject::connect(btnSortSP,&QPushButton::clicked,[&](){
        vector<Medicine> meds;
        meds.resize(srv.get_all_ent().size());
        srv.sort(2,meds);
        reloadList(meds);
    });

    QObject::connect(btn_add,&QPushButton::clicked,this,&GUI::addMed);
    QObject::connect(btn_del,&QPushButton::clicked,this,&GUI::delMed);
    QObject::connect(btn_mod,&QPushButton::clicked,this,&GUI::uptMed);
    QObject::connect(btn_undo,&QPushButton::clicked,this,&GUI::undoMed);
}

void GUI::reloadList(vector<Medicine>& meds) {
    lst->clear();
    for(const auto& med : meds){
        QListWidgetItem *item=new QListWidgetItem(QString::fromStdString(med.get_name()));
        item->setData(Qt::UserRole,QString::fromStdString(med.get_prod()));
        lst->addItem(item);
    }
}

void GUI::addMed() {
    try{
        srv.add(txtName->text().toStdString(),txtProd->text().toStdString(),txtSubst->text().toStdString(),txtPrice->text().toInt());
        reloadList(srv.get_all_ent());
        updateBut(srv.get_all_ent());
    } catch (ValidationException& ve){
        QMessageBox::warning(this,"Warning",QString::fromStdString(toMyString(ve.msg)));
    } catch (RepoException& re){
        QMessageBox::warning(this,"Warning",QString::fromStdString(toMyString(re.msg)));
    }
}

void GUI::delMed() {
    auto name=txtName->text().toStdString();
    auto prod=txtProd->text().toStdString();
    auto subst=txtSubst->text().toStdString();
    auto price=txtPrice->text().toInt();
    auto m=Medicine(name,prod,subst,price);
    int index=0;
    for(const auto& med : srv.get_all_ent()){
        if(med==m){
            break;
        }
        ++index;
    }
    if(index==srv.get_all_ent().size()){
        QMessageBox::warning(this,"Warning",QString::fromStdString("Nu exista elementul introdus"));
    } else {
        srv.del(index);
        reloadList(srv.get_all_ent());
        updateBut(srv.get_all_ent());
    }
}

void GUI::uptMed() {
    auto name=txtName->text().toStdString();
    auto prod=txtProd->text().toStdString();
    auto subst=txtSubst->text().toStdString();
    auto price=txtPrice->text().toInt();

    if(lb_index==-1){
        QMessageBox::warning(this,"Warning",QString::fromStdString("Nu ati selectat nici-un element"));
        return;
    }

    try {
        srv.modify(lb_index, name, prod, subst, price);
        reloadList(srv.get_all_ent());
        updateBut(srv.get_all_ent());
    } catch(ValidationException& ve){
        QMessageBox::warning(this,"Warning",QString::fromStdString(toMyString(ve.msg)));
    }
}

void clearLayout(QLayout *layout){
    if (layout == NULL)
        return;
    QLayoutItem *item;
    while((item = layout->takeAt(0))) {
        if (item->layout()) {
            clearLayout(item->layout());
            delete item->layout();
        }
        if (item->widget()) {
            delete item->widget();
        }
        delete item;
    }
}

void GUI::updateBut(vector<Medicine> &all) {
    clearLayout(lay_opt);
    set<string> unique;
    for(const auto& med : srv.get_all_ent()){
        unique.insert(med.get_subst());
    }

    subst_but.clear();

    for(auto& it : unique){
        subst_but.push_back(new QPushButton(QString::fromStdString(it)));
    }

    for(auto btn : subst_but){
        lay_opt->addWidget(btn);
        QObject::connect(btn,&QPushButton::clicked,[=](){
            auto val=btn->text().toStdString();
            int nr=0;
            for(const auto& med : srv.get_all_ent()){
                if(med.get_subst()==val){
                    ++nr;
                }
            }
            QMessageBox::information(nullptr,"Info",QString::number(nr));
        });
    }
}

void GUI::undoMed() {
    try {
        srv.undo();
        reloadList(srv.get_all_ent());
        updateBut(srv.get_all_ent());
    } catch(RepoException& re){
        QMessageBox::warning(this,"Warning",QString::fromStdString("Nu se mai poate face undo"));
    }
}
