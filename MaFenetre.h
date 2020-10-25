#ifndef MAFENETRE_H
#define MAFENETRE_H

#include <QWidget>
#include <QMainWindow>
#include <QLabel>
#include <QPushButton>
#include <QComboBox>
#include <QString>
#include <QTableWidget>
#include <charger_csv.h>

class MaFenetre : public QMainWindow
{
    Q_OBJECT // Macro OBLIGATOIRE

public slots :
    void setQuitter();
    void predire();
public :
    MaFenetre(QWidget *parent = 0);
    vector<QString> fillMaladieVector();
    QString getMaladieMaxScore();
    float calculerScore(QString maladie, QString fievre, QString douleur, QString toux);
    unsigned compterNbOccurrences(QString string, unsigned colonne);
    unsigned compterNbOccurrencesParMaladie(QString string, unsigned colonne, QString maladie);
private :
    QLabel *m_label_fievre;
    QLabel *m_label_douleur;
    QLabel *m_label_toux;
    QLabel *m_label_result;
    QPushButton *m_bouton_quitter;
    QPushButton *m_bouton_predire;
    QComboBox *m_combo_fievre;
    QComboBox *m_combo_douleur;
    QComboBox *m_combo_toux;
    QTableWidget *m_tablewidget_patients;
    QString couleur;
    CVString m_vet;
    CMatString m_mat;

    void fillComboBox(QComboBox *combobox, unsigned colonne);
};

#endif // MAFENETRE_H
