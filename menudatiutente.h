#ifndef MENUDATIUTENTE_H
#define MENUDATIUTENTE_H

#include <QDialog>
#include <QLabel>
#include <QLineEdit>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QPushButton>
#include <QWidget>
#include <QComboBox>
#include <QFormLayout>
#include <QDateEdit>
#include <QCheckBox>
#include <QSpinBox>
#include <QTableView>
#include "menuricerca.h"
#include "menulezione.h"
#include "lezionitablemodel.h"
#include "ricerchetablemodel.h"
#include "comboboxdelegate.h"
#include <stdexcept>
#include <QMessageBox>


class menudatiutente : public QDialog{
    Q_OBJECT
public:
    menudatiutente(QWidget *parent = nullptr);

    QGridLayout *formLayoutUtente, *formLayoutProf, *formLayoutStudente;
    QVBoxLayout *maindatiLayout;
    QHBoxLayout *aggRimuoviLezioneLayout, *aggRimuoviRicercaLayout, *lezTableLayout, *ricTableLayout, *bottoniLayout, *labelLayout, *aggiungiAnnullaMenuButtonLayout, *fuoriLayout, *vnLayout;
    QPushButton *ricercaModificaButton, *lezioneModificaButton, *aggiungiMenuButton, *annullaMenuButton, *ricercaAggiungiButton, *lezioneAggiungiButton, *lezioneEliminaButton, *ricercaEliminaButton;
    QComboBox *tipoUtenteMenuComboBox, *laureaMenuComboBox;
    QLabel *lezioneEliminaLabel, *ricercaEliminaLabel, *tipoUtenteComboboxLabel, *codiceFiscaleLabel, *nomeLabel, *cognomeLabel, *dataNascitaLabel, *tipoUtenteLabel, *annifuoricorsoLabel, *fuoricorsoLabel, *viaLabel, *numeroCivicoLabel, *emailLabel,*telefonoLabel, *capLabel, *regioneLabel, *comuneLabel, *tipoLabel, *anniServizioLabel, *laureaLabel,*corsoLabel,*matricolaLabel,*annocorsoLabel, *dataIscrizioneLabel, *ricercaLabel, *lezioneLabel;
    QLineEdit *codiceFiscaleLineEdit, *nomeLineEdit, *cognomeLineEdit, *dataNascitaLineEdit, *emailLineEdit, *telefonoLineEdit, *capLineEdit, *regioneLineEdit, *comuneLineEdit, *viaLineEdit, *numeroCivicoLineEdit, *annocorsoLineEdit, *corsoLineEdit, *anniServizioLineEdit, *tipoLineEdit, *matricolaLineEdit;
    QDateEdit * dataNascitaEdit, *dataIscrizioneEdit, * dataPubblicazioneEdit;
    QCheckBox *checkBox;
    QSpinBox *spinBox;
    lezionitablemodel *modelloLezioni;
    ricerchetablemodel *modelloRicerche;
    QTableView *lezioniTableView, *ricercheTableView;
    comboboxdelegate *oraridelegate;

public slots:
    void showRow(QString);
    int exec() override;

private slots:
    void aggiungiLezione();
    void aggiungiRicerca();
    void eliminaLezione();
    void eliminaRicerca();
    void modificaLezione(const QModelIndex &index = QModelIndex());
    void modificaRicerca(const QModelIndex &index = QModelIndex());
    void aggiornalezioneAzioni(const QItemSelection &selected , const QItemSelection &deselected);
    void aggiornaricercaAzioni(const QItemSelection &selected , const QItemSelection &deselected);
    void accept() override;
};

#endif // MENUDATIUTENTE_H
