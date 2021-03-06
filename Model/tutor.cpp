#include "Model/tutor.h"

tutor::tutor(string _nome, string _cognome, string _codiceFiscale, string _telefono, string _email, int _giornoNascita, int _meseNascita, int _annoNascita, string _regione, string _comune, string _via, string _cap, string _numeroCivico, int _matricola, TipoLaurea _laurea, string _corso, int _annoCorso, bool _fuoriCorso, int _anniFuoriCorso, int _giornoIscrizione, int _meseIscrizione, int _annoIscrizione):utente(_nome, _cognome, _codiceFiscale, _telefono, _email, _giornoNascita, _meseNascita, _annoNascita, _regione, _comune, _via, _cap, _numeroCivico), studente(_nome, _cognome, _codiceFiscale, _telefono, _email, _giornoNascita, _meseNascita, _annoNascita, _regione, _comune, _via, _cap, _numeroCivico, _matricola, _laurea, _corso, _annoCorso, _fuoriCorso, _anniFuoriCorso, _giornoIscrizione, _meseIscrizione, _annoIscrizione){
}

tipoutente tutor::getTipoUtente() const{
    return Tutor;
}

tutor* tutor::clone() const{
    return new tutor(*this);
}
