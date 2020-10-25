#include "MaFenetre.h"

MaFenetre::MaFenetre(QWidget *parent) : QMainWindow(parent)
{
    setFixedSize(800,600);
    m_bouton_quitter = new QPushButton("Quitter", this);
    m_bouton_quitter->setGeometry(720,0,80,40);
    connect(m_bouton_quitter, SIGNAL(clicked()), this, SLOT(setQuitter()));

    m_bouton_predire = new QPushButton("Prédire", this);
    m_bouton_predire->setGeometry(75,200,80,40);
    connect(m_bouton_predire, SIGNAL(clicked()), this, SLOT(predire()));

    m_label_result = new QLabel("Coucou", this);
    m_label_result->move(200,200);
    m_label_result->setFont(QFont("Arial", 12, QFont::Bold, true));

    m_label_fievre = new QLabel("Fièvre", this);
    m_label_fievre->setFont(QFont("Arial", 12, QFont::Bold, true));
    m_label_fievre->move(225, 125);

    m_combo_fievre = new QComboBox(this);
    m_combo_fievre->setGeometry(200,150,100,30);

    m_label_douleur = new QLabel("Douleur", this);
    m_label_douleur->setFont(QFont("Arial", 12, QFont::Bold, true));
    m_label_douleur->move(325, 125);

    m_combo_douleur = new QComboBox(this);
    m_combo_douleur->setGeometry(300,150,100,30);

    m_label_toux = new QLabel("Toux", this);
    m_label_toux->setFont(QFont("Arial", 12, QFont::Bold, true));
    m_label_toux->move(425, 125);

    m_combo_toux = new QComboBox(this);
    m_combo_toux->setGeometry(400,150,100,30);

    read_csv (m_mat, m_vet, "data.csv");
    fillComboBox(m_combo_fievre, 0);
    fillComboBox(m_combo_douleur, 1);
    fillComboBox(m_combo_toux, 2);

    m_tablewidget_patients = new QTableWidget(m_mat.size(), m_mat[0].size(), this);
    m_tablewidget_patients->setGeometry(75,250,500,300);
    for (unsigned i(0); i < m_vet.size(); ++i) {
        m_tablewidget_patients->setHorizontalHeaderItem(i, new QTableWidgetItem(m_vet[i].c_str(), 1));
    }
    for (unsigned i(0); i < m_mat.size(); ++i) {
        m_tablewidget_patients->setVerticalHeaderItem(i, new QTableWidgetItem(QString::fromStdString("Patient " + to_string(i) ), 1));
    }
    for(unsigned i = 0; i < m_mat.size() ; ++i){
            for(unsigned j = 0 ; j < m_mat[i].size() ; ++j){
                QTableWidgetItem *newItem = new QTableWidgetItem(QString::fromStdString(m_mat[i][j]));
                m_tablewidget_patients->setItem(i,j,newItem);
            }
        }

}

void MaFenetre::fillComboBox(QComboBox *combobox, unsigned colonne)
{
    vector<string> elements;
    for (unsigned i(0); i < m_mat.size(); ++i) {
        elements.push_back(m_mat[i][colonne]);
    }
    sort( elements.begin(), elements.end() );
    elements.erase( unique( elements.begin(), elements.end() ), elements.end() );
    combobox->addItem("NULL");
    for (unsigned i(0); i < elements.size(); ++i) {
        combobox->addItem(QString::fromUtf8(elements[i].c_str()));
    }
}
void MaFenetre::setQuitter()
{
    exit(0);
}

void MaFenetre::predire(){
    m_label_result->setText(getMaladieMaxScore());
}

vector<QString> MaFenetre::fillMaladieVector(){
    vector<QString> elements;
    for (unsigned i(0); i < m_mat.size(); ++i) {
        elements.push_back(QString::fromStdString(m_mat[i][3]));
    }
    sort( elements.begin(), elements.end() );
    elements.erase( unique( elements.begin(), elements.end() ), elements.end() );
    return elements;
}

unsigned MaFenetre::compterNbOccurrences(QString string, unsigned colonne){
    unsigned result =0;
    for (unsigned i(0); i < m_mat.size(); ++i) {
        if(QString::fromStdString(m_mat[i][colonne]) == string)
            ++result;
    }
    return result;
}

unsigned MaFenetre::compterNbOccurrencesParMaladie(QString string, unsigned colonne, QString maladie){
    unsigned result =0;
    for (unsigned i(0); i < m_mat.size(); ++i) {
        if(QString::fromStdString(m_mat[i][3]) == maladie)
            if(QString::fromStdString(m_mat[i][colonne]) == string)
                ++result;
    }
    return result;
}

float MaFenetre::calculerScore(QString maladie, QString fievre, QString douleur, QString toux){
    unsigned nbOccurrencesMaladie = compterNbOccurrences(maladie, 3);
    float freqMaladie = double(nbOccurrencesMaladie) / double(this->m_mat.size());

    float confFievre;
    float confDouleur;
    float confToux;

    if(fievre == "NULL")
        confFievre = 1;
    else
        confFievre = double(compterNbOccurrencesParMaladie(fievre, 0, maladie)) / double(nbOccurrencesMaladie);

    if(douleur == "NULL")
        confDouleur = 1;
    else
        confDouleur = double(compterNbOccurrencesParMaladie(douleur, 1, maladie)) / double(nbOccurrencesMaladie);

    if(toux == "NULL")
        confToux = 1;
    else
        confToux = double(compterNbOccurrencesParMaladie(toux, 2, maladie)) / double(nbOccurrencesMaladie);

    return freqMaladie * confFievre * confDouleur * confToux;
}

QString MaFenetre::getMaladieMaxScore()
{
    QString result;
    vector<QString> maladies = fillMaladieVector();
    vector<float> scores;
    QString fievre = m_combo_fievre->currentText();
    QString douleur = m_combo_douleur->currentText();
    QString toux = m_combo_toux->currentText();

    cout << "1" << endl;
    cout.flush();

    if(fievre == "NULL" && douleur == "NULL" && toux == "NULL")
        return "NULL";

    cout << "2" << endl;
    cout.flush();

    for(unsigned i(0); i < maladies.size(); ++i){
        scores.push_back(calculerScore(maladies[i], fievre, douleur, toux));
    }

    float scoreMax = 0;
    unsigned indiceMaladie;
    for (unsigned i(0); i < scores.size(); ++i) {
        if(scores[i] >= scoreMax){
            scoreMax = scores[i];
            indiceMaladie = i;
        }
    }
    return maladies[indiceMaladie];
}
