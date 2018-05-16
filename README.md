Iliescu Oana-Maria
Monea Damian
312CA

Data inceperii temei: 10.05.2018
Data terminarii temei: 15.05.2018

Impartirea sarcinilor:

  Pentru realizarea temei, am impartit sarcinile in proportii egale, fiecare
avand anumite functii de rezolvat. Totusi, aceasta repartizare a sarcinilor
nu a fost urmata cu strictete, ajungandu-se in situatia in care amandoi am
lucrat la fiecare functie a programului, fie pentru a o imbunatati, fie pentru
a reface acea functie complet. Cu alte cuvinte, aceasta tema a fost facuta
printr-o cooperare totala, incepand de la intelegerea algoritmului,
implementarea si optimizarea lui, si pana la scrierea acestui README.

Probleme intampinate:

  Dintre toate problemele intampinate, s-ar putea spune ca cea principala, cea
care a cauzat si restul problemelor in implementarea algoritmului, a fost
imposibilitatea de a testa individual functiile ce nu sunt testate de checker,
datorita restrictiei de a urma structura scheletului.
  Pe langa aceasta problema, putem spune ca am mai intampinat urmatoarele
dificultati minore:
    -cresterea preciziei algoritmului: precizia algoritmului nostru a fost
    puternic influentata de get_random_samples si get_random_dimensions,
    mai exact de functiile de random folosite. Initial am folosit functia
    rand() pentru a alege numere aleatorii, dar cum folosirea ei era
    considerata o greseala de coding style, am reusit sa "simulam" un numar
    aleator printr-o formula matematica, combinata cu functia rand_r. Metoda
    aceasta a rezultat intr-o precizie de aproximativ 50%, iar inlocuirea
    ei cu functia random() a dus la precizia actuala, de 91-92%;
    -intelegerea algoritmului, in special a felului in care implementarea
    acestuia se foloseste de matrici pentru a simula arborii, nu de arbori
    propriu-zisi, intrucat explicatiile de pe internet nu se aseamana cu
    implementarea ceruta.

Algoritm utilizat:

  Dupa cum este precizat si in cerinta temei, am avut de implementat algoritmul
Random Forest pentru recunoasterea cifrelor scrise de mana. Algoritmul in sine
presupune generarea unei "paduri", formata din arbori binari de decizie, care,
in urma etapei de "train"(prin supervised learning) si in urma selectiei unui
arbore care indeplineste cel mai bine criteriile de selectie, va putea, cu o
precizie ridicata, sa detecteze literele scrise, date ca input. Desi algoritmul
acesta este este mai eficient atunci cand vine vorba de regresii, e potrivit si
pentru a crea un clasificator, cum e acest program.
  Explicatia algoritmului implementat nu difera de cea din cerinta, din punct de
vedere conceptual. Din punctul de vedere al functiilor implementate, acestea
pot fi explicate astfel:
    -make_leaf: daca parametrul is_single_class are valoarea true, facem nodul
    curent frunza. Daca are valoarea false, alegem clasa care apare cel mai des
    si o facem frunza;
    -same_class: verifica daca toate testele din samples au aceeasi clasa;
    -get_entropy_by_indexes: 

Complexitatea temei:

  Dintre toate cele 3 teme primite, am putea spune ca aceasta a fost cea mai
interesanta. Desi algoritmul de implementat a fost mult mai greu de inteles
decat cel al temelor anterioare, faptul ca am putut sa facem tema in echipa a
ajutat la o intelegere mai buna a conceptelor prezentate in tema si a rezultat
nu numai intr-o implementare mai rapida, dar si in una mai eficienta.

Alte precizari: