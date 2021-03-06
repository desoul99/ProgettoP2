#include "menu.h"

menu::menu(QWidget *parent)
    : QWidget(parent){
    mainLayout = new QVBoxLayout(this);

    //Bottoni di aggiunta / rimozione utenti
    aggiungiRimuoviButtonsLayout = new QHBoxLayout();
    aggiungiButton= new QPushButton("Aggiungi utente");
    rimuoviButton= new QPushButton("Rimuovi utente");
    modificaButton= new QPushButton("Visualizza dettagli utente");
    rimuoviButton->setEnabled(false);
    modificaButton->setEnabled(false);
    aggiungiRimuoviButtonsLayout->addWidget(aggiungiButton);
    aggiungiRimuoviButtonsLayout->addWidget(modificaButton);
    aggiungiRimuoviButtonsLayout->addWidget(rimuoviButton);
    mainLayout->addLayout(aggiungiRimuoviButtonsLayout);

    //Apre nuova finestra per inserimento dati utente
    connect(aggiungiButton, SIGNAL(clicked()), this, SLOT(aggiungiUtente()));
    //Rimuove l'utente selezionato
    connect(rimuoviButton, SIGNAL(clicked()), this, SLOT(rimuoviUtente()));
    //Visualizza l'utente selezionato
    connect(modificaButton, SIGNAL(clicked()), this, SLOT(modificaUtente()));

    //Ricerca per campi
    ricercaLayout = new QHBoxLayout();
    cercaLabel = new QLabel("Cerca");
    perLabel = new QLabel("Per");
    cercaLineEdit = new QLineEdit();
    colonnaRicercaComboBox = new QComboBox();

    ricercaLayout->addWidget(cercaLabel);
    ricercaLayout->addWidget(cercaLineEdit);
    ricercaLayout->addWidget(perLabel);
    ricercaLayout->addWidget(colonnaRicercaComboBox);
    mainLayout->addLayout(ricercaLayout);

    //Ricerca per parola nella colonna selezionata
    connect(cercaLineEdit, SIGNAL(textChanged(QString)), this, SLOT(aggiornaFiltro()));
    //Aggiorna colonna selezionata per ricerca di parola
    connect(colonnaRicercaComboBox, SIGNAL(currentIndexChanged(QString)), this, SLOT(aggiornaColonnaRicerca(QString)));

    //Lista Utenti
    modelloTabellaUtenti = new utentitablemodel(this);
    modelloProxy = new proxymodel(this);
    modelloProxy->setSourceModel(modelloTabellaUtenti);
    utentiTableView = new QTableView();
    utentiTableView->setModel(modelloProxy);
    utentiTableView->setSortingEnabled(true);
    utentiTableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    //Fa in modo che sizeHint() mostra tutte le colonne della tabella senza scrolling
    utentiTableView->setSizeAdjustPolicy(QAbstractScrollArea::AdjustToContents);
    //Seleziona una riga al posto di una singola cella
    utentiTableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    mainLayout->addWidget(utentiTableView);

    //Quando vengono aggiunte/rimosse colonne aggiusta la loro dimensione per riempire tutto lo spazio a loro disposizione
    connect(modelloProxy, SIGNAL(colonneModificate()), utentiTableView->horizontalHeader(), SLOT(resizeSections()));

    //Quando viene selezionata una riga viene sbloccato il pulsante di rimozione, altrimenti viene bloccato
    connect(utentiTableView->selectionModel(),SIGNAL(selectionChanged(QItemSelection,QItemSelection)) ,this, SLOT(aggiornaAzioni(QItemSelection,QItemSelection)));

    connect(utentiTableView, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(modificaUtente(QModelIndex)));

    //Modalità Visualizzazione
    visualizzazioneLayout = new QHBoxLayout();
    visualizzaLabel = new QLabel("Visualizza");
    tipoUtenteComboBox = new QComboBox();
    tipoUtenteComboBox->addItem("Utenti");
    tipoUtenteComboBox->addItem("Studenti");
    tipoUtenteComboBox->addItem("Professori");
    tipoUtenteComboBox->addItem("Insegnanti");
    tipoUtenteComboBox->addItem("Tutor");

    visualizzaLabel->setAlignment(Qt::AlignCenter);
    visualizzazioneLayout->addWidget(visualizzaLabel);
    visualizzazioneLayout->addWidget(tipoUtenteComboBox);

    mainLayout->addLayout(visualizzazioneLayout);

    //Quando cambia il tipo utente selezionato aggiorna le colonne visibili
    connect(tipoUtenteComboBox, SIGNAL(currentIndexChanged(QString)), this, SLOT(tipoUtenteComboBoxChanged(QString)));

    //Setup iniziale
    tipoUtenteComboBoxChanged("Utente");

    setLayout(mainLayout);


    //Imposta la dimensione della finestra in modo che tutte le colonne siano ben visibili in qualsiasi caso
    modelloProxy->setFiltroColonne(Tutor);
    setMinimumSize(sizeHint());
    modelloProxy->setFiltroColonne(Utente);

    //Allinea la finestra al centro dello schermo
    setGeometry(
                QStyle::alignedRect(
                    Qt::LeftToRight,
                    Qt::AlignCenter,
                    size(),
                    qApp->desktop()->availableGeometry()
                    )
                );
}

//Imposta la dimensione delle colonne adatta al contenuto della tabella
void menu::setupColonneTableView(tipoutente _tipoutente){
    utentiTableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    if(_tipoutente != Studente) utentiTableView->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    utentiTableView->horizontalHeader()->setSectionResizeMode(4, QHeaderView::ResizeToContents);
    utentiTableView->horizontalHeader()->setSectionResizeMode(5, QHeaderView::ResizeToContents);
    if(_tipoutente != Tutor){
        utentiTableView->horizontalHeader()->setSectionResizeMode(3, QHeaderView::ResizeToContents);
    }else{
        utentiTableView->horizontalHeader()->setSectionResizeMode(6, QHeaderView::ResizeToContents);
    }
}

//Aggiorna la combobox che filtra per colonne con i nuovi valori delle colonne
void menu::tipoUtenteComboBoxChanged(const QString &_string){
    modelloProxy->setFiltroColonne(utente::toTipoUtente(_string.toStdString()));
    colonnaRicercaComboBox->clear();
    QStringList headers;
    for(int i = 0; i < modelloProxy->columnCount(); i++){
        headers.append(modelloProxy->headerData(i, Qt::Horizontal).toString());
    }
    colonnaRicercaComboBox->addItems(headers);
    setupColonneTableView(utente::toTipoUtente(_string.toStdString()));
}

utente* menu::creaUtenteTemp(const menudatiutente &_menu)const{
    QString codiceFiscale = _menu.codiceFiscaleLineEdit->text();
    QString nome = _menu.nomeLineEdit->text();
    QString cognome = _menu.cognomeLineEdit->text();
    QDate dataNascita = _menu.dataNascitaEdit->date();
    QString telefono = _menu.telefonoLineEdit->text();
    QString email = _menu.emailLineEdit->text();
    QString regione = _menu.regioneLineEdit->text();
    QString comune = _menu.comuneLineEdit->text();
    QString via = _menu.viaLineEdit->text();
    QString cap = _menu.capLineEdit->text();
    QString numeroCivico = _menu.numeroCivicoLineEdit->text();
    QString tipoUtente = _menu.tipoUtenteMenuComboBox->currentText();
    puntatoresmart<utente> utemp;
    if(tipoUtente == "Studente" || tipoUtente == "Tutor"){
        int matricola = _menu.matricolaLineEdit->text().toInt();
        //tipolaurea direttamente sotto
        QString corso = _menu.corsoLineEdit->text();
        int annoCorso = _menu.annocorsoLineEdit->text().toInt();
        bool fuoriCorso = _menu.checkBox->checkState();
        int anniFuoriCorso;
        if(fuoriCorso) {
            anniFuoriCorso = _menu.spinBox->value();
            if(anniFuoriCorso == 0) fuoriCorso = false;
        }else anniFuoriCorso = 0;
        QDate dataIscrizione = _menu.dataIscrizioneEdit->date();
        if(tipoUtente == "Studente"){
            utemp = new studente(nome.toStdString(), cognome.toStdString(), codiceFiscale.toStdString(), telefono.toStdString(), email.toStdString(), dataNascita.day(), dataNascita.month(), dataNascita.year(), regione.toStdString(), comune.toStdString(), via.toStdString(), cap.toStdString(), numeroCivico.toStdString(), matricola, triennale, corso.toStdString(), annoCorso, fuoriCorso, anniFuoriCorso, dataIscrizione.day(), dataIscrizione.month(), dataIscrizione.year());
            dynamic_cast<studente*>(utemp.operator ->())->setLaurea(_menu.laureaMenuComboBox->currentText().toStdString());
        }else{
            utemp = new tutor(nome.toStdString(), cognome.toStdString(), codiceFiscale.toStdString(), telefono.toStdString(), email.toStdString(), dataNascita.day(), dataNascita.month(), dataNascita.year(), regione.toStdString(), comune.toStdString(), via.toStdString(), cap.toStdString(), numeroCivico.toStdString(), matricola, triennale, corso.toStdString(), annoCorso, fuoriCorso, anniFuoriCorso, dataIscrizione.day(), dataIscrizione.month(), dataIscrizione.year());
        }
    }
    if(tipoUtente == "Professore"){
        QString tipo = _menu.tipoLineEdit->text();
        int anniServizio = _menu.anniServizioLineEdit->text().toInt();
        utemp = new professore(nome.toStdString(), cognome.toStdString(), codiceFiscale.toStdString(), telefono.toStdString(), email.toStdString(), dataNascita.day(), dataNascita.month(), dataNascita.year(), regione.toStdString(), comune.toStdString(), via.toStdString(), cap.toStdString(), numeroCivico.toStdString(), tipo.toStdString(), anniServizio);
        dynamic_cast<professore*>(utemp.operator ->())->setRicerche(_menu.modelloRicerche->getListaRicerche());
    }
    if(tipoUtente == "Professore" || tipoUtente == "Tutor"){
        dynamic_cast<insegnante*>(utemp.operator ->())->setLezioni(_menu.modelloLezioni->getListaLezioni());
    }
    return (*utemp).clone();
}

//Nuova finestra aggiunta cliente
void menu::aggiungiUtente(){
    menudatiutente aggiungi;
    if (aggiungi.exec()) {
        puntatoresmart<utente> utemp = creaUtenteTemp(aggiungi);
        if (!(modelloTabellaUtenti->contains(utemp))) {
            modelloTabellaUtenti->aggiungiUtente(utemp);
        }
    }
}

void menu::modificaUtente(const QModelIndex &index){
    QString nome;
    QString cognome;
    QString codiceFiscale;
    QString telefono;
    QString email;
    QDate dataNascita;
    QString regione;
    QString comune;
    QString via;
    QString cap;
    QString numeroCivico;
    QString tipoUtente;
    QString matricola;
    QString laurea;
    QString corso;
    QString annoCorso;
    QString anniFuoriCorso;
    QDate dataIscrizione;

    int row;
    menudatiutente modifica;

    if(!index.isValid()){
        //Se sono selezionate più righe modifica la prima
        QModelIndexList indexes = utentiTableView->selectionModel()->selectedRows();
        row = modelloProxy->mapToSource(indexes.at(0)).row();
    }else{
        row = modelloProxy->mapToSource(index).row();
    }

    QModelIndex tipoUtenteIndex = modelloProxy->sourceModel()->index(row, 11, QModelIndex());
    tipoUtente = modelloProxy->sourceModel()->data(tipoUtenteIndex, Qt::DisplayRole).toString();

    QModelIndex codiceFiscaleIndex = modelloProxy->sourceModel()->index(row, 0, QModelIndex());
    codiceFiscale = modelloProxy->sourceModel()->data(codiceFiscaleIndex, Qt::DisplayRole).toString();

    QModelIndex nomeIndex = modelloProxy->sourceModel()->index(row, 2, QModelIndex());
    nome = modelloProxy->sourceModel()->data(nomeIndex, Qt::DisplayRole).toString();

    QModelIndex cognomeIndex = modelloProxy->sourceModel()->index(row, 3, QModelIndex());
    cognome = modelloProxy->sourceModel()->data(cognomeIndex, Qt::DisplayRole).toString();

    QModelIndex dataNascitaIndex = modelloProxy->sourceModel()->index(row, 4, QModelIndex());
    dataNascita = modelloProxy->sourceModel()->data(dataNascitaIndex, Qt::DisplayRole).toDate();

    QModelIndex telefonoIndex = modelloProxy->sourceModel()->index(row, 5, QModelIndex());
    telefono = modelloProxy->sourceModel()->data(telefonoIndex, Qt::DisplayRole).toString();

    QModelIndex emailIndex = modelloProxy->sourceModel()->index(row, 6, QModelIndex());
    email = modelloProxy->sourceModel()->data(emailIndex, Qt::DisplayRole).toString();

    QModelIndex regioneIndex = modelloProxy->sourceModel()->index(row, 14, QModelIndex());
    regione = modelloProxy->sourceModel()->data(regioneIndex, Qt::DisplayRole).toString();

    QModelIndex comuneIndex = modelloProxy->sourceModel()->index(row, 16, QModelIndex());
    comune = modelloProxy->sourceModel()->data(comuneIndex, Qt::DisplayRole).toString();

    QModelIndex viaIndex = modelloProxy->sourceModel()->index(row, 17, QModelIndex());
    via = modelloProxy->sourceModel()->data(viaIndex, Qt::DisplayRole).toString();

    QModelIndex capIndex = modelloProxy->sourceModel()->index(row, 15, QModelIndex());
    cap = modelloProxy->sourceModel()->data(capIndex, Qt::DisplayRole).toString();

    QModelIndex numeroCivicoIndex = modelloProxy->sourceModel()->index(row, 18, QModelIndex());
    numeroCivico = modelloProxy->sourceModel()->data(numeroCivicoIndex, Qt::DisplayRole).toString();

    if(tipoUtente == "Studente" || tipoUtente == "Tutor"){

        QModelIndex matricolaIndex = modelloProxy->sourceModel()->index(row, 1, QModelIndex());
        matricola = modelloProxy->sourceModel()->data(matricolaIndex, Qt::DisplayRole).toString();

        QModelIndex corsoIndex = modelloProxy->sourceModel()->index(row, 7, QModelIndex());
        corso = modelloProxy->sourceModel()->data(corsoIndex, Qt::DisplayRole).toString();

        QModelIndex laureaIndex = modelloProxy->sourceModel()->index(row, 8, QModelIndex());
        laurea = modelloProxy->sourceModel()->data(laureaIndex, Qt::DisplayRole).toString();

        QModelIndex annoCorsoIndex = modelloProxy->sourceModel()->index(row, 9, QModelIndex());
        annoCorso = modelloProxy->sourceModel()->data(annoCorsoIndex, Qt::DisplayRole).toString();

        QModelIndex anniFuoriCorsoIndex = modelloProxy->sourceModel()->index(row, 10, QModelIndex());
        anniFuoriCorso = modelloProxy->sourceModel()->data(anniFuoriCorsoIndex, Qt::DisplayRole).toString();

        QModelIndex dataIscrizioneIndex = modelloProxy->sourceModel()->index(row, 19, QModelIndex());
        dataIscrizione = modelloProxy->sourceModel()->data(dataIscrizioneIndex, Qt::DisplayRole).toDate();

        modifica.matricolaLineEdit->setText(matricola);
        modifica.corsoLineEdit->setText(corso);
        modifica.annocorsoLineEdit->setText(annoCorso);
        modifica.dataIscrizioneEdit->setDate(dataIscrizione);
        if(anniFuoriCorso == "Regolare"){
            modifica.checkBox->setChecked(false);
            modifica.spinBox->setValue(0);
        }else{
            modifica.checkBox->setChecked(true);
            modifica.spinBox->setValue(anniFuoriCorso.toInt());
        }
        if(laurea == "Triennale") laurea = "0";
        if(laurea == "Magistrale") laurea = "1";
        if(laurea == "Diploma Specializzazione") laurea = "2";
        if(laurea == "Dottorato Ricerca") laurea = "3";

        modifica.laureaMenuComboBox->setCurrentIndex(laurea.toInt());

        if(tipoUtente =="Studente"){
            modifica.tipoUtenteMenuComboBox->setCurrentIndex(0);
        }else{
            modifica.tipoUtenteMenuComboBox->setCurrentIndex(2);
        }
    }else if(tipoUtente == "Professore"){
        QString tipoProfessore;
        QString anniServizio;

        QModelIndex tipoProfessoreIndex = modelloProxy->sourceModel()->index(row, 12, QModelIndex());
        tipoProfessore = modelloProxy->sourceModel()->data(tipoProfessoreIndex, Qt::DisplayRole).toString();

        QModelIndex anniServizioIndex = modelloProxy->sourceModel()->index(row, 13, QModelIndex());
        anniServizio = modelloProxy->sourceModel()->data(anniServizioIndex, Qt::DisplayRole).toString();

        modifica.tipoLineEdit->setText(tipoProfessore);
        modifica.anniServizioLineEdit->setText(anniServizio);
        modifica.modelloRicerche->setListaRicerche(modelloTabellaUtenti->getListaRicerche(row));
        modifica.tipoUtenteMenuComboBox->setCurrentIndex(1);
    }
    if(tipoUtente == "Professore" || tipoUtente == "Tutor"){
        modifica.modelloLezioni->setListaLezioni(modelloTabellaUtenti->getListaLezione(row));
        if(modifica.modelloLezioni->rowCount()>0){
            for(int i=0; i<modifica.modelloLezioni->rowCount(); i++){
                modifica.lezioniTableView->openPersistentEditor(modifica.modelloLezioni->index(i, 4));
            }
        }
    }
    modifica.showRow(tipoUtente);

    modifica.tipoUtenteMenuComboBox->setEnabled(false);
    modifica.setWindowTitle(tr("Modifica utente"));
    modifica.aggiungiMenuButton->setText("Modifica");
    modifica.nomeLineEdit->setText(nome);
    modifica.cognomeLineEdit->setText(cognome);
    modifica.codiceFiscaleLineEdit->setText(codiceFiscale);
    modifica.dataNascitaEdit->setDate(dataNascita);
    modifica.telefonoLineEdit->setText(telefono);
    modifica.emailLineEdit->setText(email);
    modifica.regioneLineEdit->setText(regione);
    modifica.comuneLineEdit->setText(comune);
    modifica.capLineEdit->setText(cap);
    modifica.viaLineEdit->setText(via);
    modifica.numeroCivicoLineEdit->setText(numeroCivico);

    if (modifica.exec()) {
        puntatoresmart<utente> utemp = creaUtenteTemp(modifica);
        if (!(modelloTabellaUtenti->contains(utemp))){
            modelloTabellaUtenti->modificaUtente(row, utemp);
        }
    }
}

//Rimuove l'utente selezionato quando viene premuto il bottone
void menu::rimuoviUtente(){
    QItemSelectionModel *selectionModel = utentiTableView->selectionModel();
    QList<int> rows;
    foreach (QModelIndex index, selectionModel->selectedRows()){
        rows.append(modelloProxy->mapToSource(index).row());
    }
    //Ordina la lista di righe in ordine decrescende, così da
    //non modificare gli indici successivi a quello rimosso e
    //non saper più che elemento rimuovere (Solo per rimozioni multiple)
    std::sort(rows.rbegin(), rows.rend());
    foreach(int row, rows){
        modelloProxy->sourceModel()->removeRows(row, 1, QModelIndex());
    }
}

//Disabilita il pulsante di rimozione utente se nessuna riga è selezionata
void menu::aggiornaAzioni(const QItemSelection &selected , const QItemSelection &deselected){
    Q_UNUSED(deselected);
    QModelIndexList indexes = selected.indexes();
    if (!indexes.isEmpty()) {
        rimuoviButton->setEnabled(true);
        modificaButton->setEnabled(true);
    } else {
        rimuoviButton->setEnabled(false);
        modificaButton->setEnabled(false);
    }
}

//Aggiorna il filtro per cercare nella colonna selezionata i valori nel QLineEdit di ricerca
void menu::aggiornaFiltro(){
    QRegExp regExp(cercaLineEdit->text(),
                   Qt::CaseInsensitive,
                   QRegExp::FixedString);
    modelloProxy->setFilterRegExp(regExp);
}

//Aggiorna la colonna di ricerca in base all'elemento selezionato nella ComboBox delle colonne
void menu::aggiornaColonnaRicerca(const QString& _colonna){
    for(int i = 0; i < modelloProxy->sourceModel()->columnCount(); i++){
        QString t = modelloProxy->sourceModel()->headerData(i, Qt::Horizontal).toString();
        if(!t.compare(_colonna, Qt::CaseSensitive)){
            modelloProxy->setColonnaRicerca(i);
            return;
        }
    }
}

menu::~menu(){
}

