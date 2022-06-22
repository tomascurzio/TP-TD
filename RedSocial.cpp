#include "RedSocial.h"
#include <string>
#include <set>
#include <map>

using namespace std;

RedSocial::RedSocial(){
    _usuarios = {};
    _amistades = {};
    _alias = {};
    _cant_amistades = 0;
    _amigos_popular = {};
    _popular;
}

const set<int> & RedSocial::usuarios() const{
    return _usuarios;
}
string RedSocial::obtener_alias(int id) const{
    return _alias.at(id); 
}

const set<string> & RedSocial::obtener_amigos(int id) const{
    return _amistades.at(id);
}

int RedSocial::cantidad_amistades() const{
    return _cant_amistades;
}

void RedSocial::registrar_usuario(string alias, int id){
    _usuarios.insert(id);
    _alias[id] = alias;
    _amistades[id] = {};
}

void RedSocial::desamigar_usuarios(int id_A, int id_B){
    string alias_A = obtener_alias(id_A);
    string alias_B = obtener_alias(id_B);
    _amistades[id_A].erase(alias_B);
    _amistades[id_B].erase(alias_A);
    _cant_amistades--;
    if(id_A == _popular || id_B == _popular){
        if(id_A == _popular){
            _amigos_popular.erase(alias_B);
        } else {
            _amigos_popular.erase(alias_A);
        }
        for(auto it = _amistades.begin(); it != _amistades.end(); it++){
            if(it->second.size() > _amigos_popular.size()){
                _amigos_popular = it->second;
                _popular = it->first;
            }
        }
    }
}

int RedSocial::obtener_id(string alias) const{
    int res = 0;
    for(auto it = _alias.begin(); it != _alias.end(); it++){
        if(it->second == alias){
            res = it->first;
        }
    }
    return res;
}

void RedSocial::eliminar_usuario(int id){
    _usuarios.erase(id);
    set<string> amigos_actuales = _amistades.at(id);
    if(amigos_actuales.size() > 0){
        for(auto it = amigos_actuales.begin(); it != amigos_actuales.end(); it++){
            int usuario = obtener_id(*it);
            desamigar_usuarios(id, usuario);
        }
    }
    _alias.erase(id);
    _amistades.erase(id);
}

void RedSocial::amigar_usuarios(int id_A, int id_B){
    string alias_A = obtener_alias(id_A);
    string alias_B = obtener_alias(id_B);
    _amistades[id_A].insert(alias_B);
    _amistades[id_B].insert(alias_A);
    _cant_amistades++;
    if(id_A != _popular || id_B != _popular){
        if(_amistades[id_A].size() > _amigos_popular.size()){
            _amigos_popular = _amistades[id_A];
            _popular = id_A;
        } else if (_amistades[id_B].size() > _amigos_popular.size()){
            _amigos_popular = _amistades[id_B];
            _popular = id_B;
        }
    } else {
        if(id_A == _popular){
            _amigos_popular.insert(alias_B);
        } else {
            _amigos_popular.insert(alias_A);
        }
    }
}

const set<string> & RedSocial::amigos_del_usuario_mas_popular() const{
    return _amigos_popular;
}