#include <iostream>
#include <string.h>
#include <list>
#include <fstream>
#include "tabelHash.h"

using namespace std;

int main()
{
    tabelHash th,obj;
    int optiune,autentificat = 0;

    th.initializareLista(th);

    while(optiune!=3)
    {
        cout<<"Va rugam introduceti un numar corespunzator uneia dintre urmatoarele optiuni : "<<endl;
        cout<<"1.Autentificare."<<endl;
        cout<<"2.Creare cont nou."<<endl;
        cout<<"3.Terminare program."<<endl<<endl;
        cin>>optiune;
        cin.get();
        cout<<endl;

        if(optiune==1)
        {
            tabelHash::utilizator utilizatorCurent;
            th.autentificare(utilizatorCurent,autentificat);

            if(autentificat==0)
            {

                cout<<"Nu exista niciun utilizator inregistrat in baza noastra de date SAU datele introduse sunt incorecte!"<<endl<<endl;


            }
            else
            {

                autentificat = 0;
                int optiune2;
                cout<<endl;

                while(optiune2!=6)
                {
                    cout<<"Va rugam introduceti un numar corespunzator uneia drintre urmatoarele optiuni: "<<endl;
                    cout<<"1.Informatii cont."<<endl;
                    cout<<"2.Schimbare parola."<<endl;
                    cout<<"3.Schimbare nume de utilizator."<<endl;
                    cout<<"4.Schimbare adresa de email."<<endl;
                    cout<<"5.Delogare."<<endl;
                    cout<<"6.Terminare program."<<endl<<endl;
                    cin>>optiune2;
                    cin.get();
                    if(optiune2==5)
                    {
                        break;
                    }
                    else
                    {
                        switch(optiune2)
                        {
                        case 1:
                            th.informatiiCont(utilizatorCurent);
                            break;
                        case 2:
                            th.schimbareParola(utilizatorCurent);
                            cout<<endl;
                            break;
                        case 3:
                            th.schimbareNumeUtilizator(utilizatorCurent);
                            cout<<endl;
                            break;
                        case 4:
                            th.schimbareEmailUtilizator(utilizatorCurent);
                            cout<<endl;
                            break;
                        case 6:
                            th.rescrierefisier();
                            return 0;
                        default:
                            cout<<"NU ati introdus o valoare valida!"<<endl<<endl;
                            return 0;
                        }
                    }
                }
            }
        }
        else
        {
            if(optiune==2)
            {
                th.adaugareUtilizator(th);
                cout<<endl;
            }
            else
            {
                if(optiune==3)
                {
                    th.rescrierefisier();
                    cout<<endl;
                    return 0;
                }
                else
                {
                    cout<<"NU ati introdus o valoare valida!"<<endl;
                }
            }
        }
    }

    return 0;
}

