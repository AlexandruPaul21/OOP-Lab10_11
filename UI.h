//
// Created by Alexandru-Paul Sirbu on 01.05.2022.
//

#ifndef LAB10_11V2_UI_H
#define LAB10_11V2_UI_H

#include "service.h"
#include <QWidget>
#include <QListWidget>
#include <QBoxLayout>
#include <QPushButton>
#include <QFormLayout>
#include <QLabel>
#include <QLineEdit>
#include <QMessageBox>


class GUI : public QWidget{
private:
    Service srv;
    QListWidget *lst;
    QHBoxLayout *lyMain = new QHBoxLayout;
    QPushButton *btnSortName;
    QPushButton *btnSortProd;
    QPushButton *btnSortSP;
    QPushButton *btnFilterPrice;
    QPushButton *btnFilterSubst;
    QPushButton *btn_add;
    QPushButton *btn_mod;
    QPushButton *btn_del;
    QPushButton *btn_undo;
    QLineEdit *txtName;
    QLineEdit *txtProd;
    QLineEdit *txtSubst;
    QLineEdit *txtPrice;
    int lb_index;
    void init_GUI();
    void connectSignalsSlots();
    void reloadList(vector<Medicine>& meds);

    void addMed();
    void delMed();
    void uptMed();
public:
    GUI(Service& srv) : srv {srv}{
        init_GUI();
        connectSignalsSlots();
        reloadList(srv.get_all_ent());
    }
};

#endif //LAB10_11V2_UI_H
