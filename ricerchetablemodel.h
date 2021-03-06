#ifndef RICERCHETABLEMODEL_H
#define RICERCHETABLEMODEL_H

#include "Model/ricerca.h"
#include "Model/contenitore.h"
#include <QAbstractTableModel>

class ricerchetablemodel: public QAbstractTableModel{
    Q_OBJECT
public:
    ricerchetablemodel(QObject *parent = nullptr);
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;
    bool insertRows(int position, int rows, const QModelIndex &parent) override;
    bool removeRows(int row, int count, const QModelIndex &parent) override;
    void setListaRicerche(contenitore<ricerca> _listaRicerche);
    ricerchetablemodel(contenitore<ricerca> _listaRicerche, QObject *parent = nullptr);
    bool contains(const ricerca& _ric);
    contenitore<ricerca>& getListaRicerche();
private:
    contenitore<ricerca> listaRicerche;
};

#endif // RICERCHETABLEMODEL_H
