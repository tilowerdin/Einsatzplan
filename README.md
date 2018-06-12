# Einsatzplan Trainer

## Aufgabenstellung 1

* Schwimmhallen mit Bahnen und Zeiten
* Turnhallen mit Zeiten
* je eine Schwimmhalle und ein Ort gehören zusammen
* Gruppen mit Altersklasse, Häufigkeit Wasser, Anzahl Bahnen, Häufigkeit Athletik
    * wenn Athletik, dann eine Stunde vor oder nach dem Wassertraining
    * Beispiele: 
        * 1x Wasser
        * 1x Wasser, 1x Athletik
        * 2x Wasser, 1x Athletik
        * 3x Wasser, 1x Athletik
        * 4x Wasser, 1x Athletik
* Wassertrainingszeiten nach Möglichkeit im Abstand von ein besser zwei Tagen
    * betrachten wir erstmal nicht, da es vermutlich Mittwochs keine Zeit gibt, sodass es eh nur maximal für Gurppen mit zwei Zeiten infrage kommt. Ist allerdings nicht so schwer hinzuzufügen.
* Altersklasse Kind muss bis 19 Uhr fertig sein
* Altersklasse Jugend muss bis 20 Uhr fertig sein

## Aufgabenstellung 2 - Erweiterung

* Gruppen haben einen Trainer
* Gruppen mit gleichem Trainer nicht parallel
* möglicherweise Tage an denen Trainer nicht kann
* möglicherweise alle Gruppen eines Trainer in direkter Folge

## Aufgabenstellung 3 - Erweiterung

Eine dritte Aufgabe könnte eine weitere Randbedingung enthalten, dass ein Trainer eine Gruppe mit mehrmaligen Training und eine Gruppe mit einmaligem Training betreuen soll. Dies muss bei Aufgabe 1 und 2 nicht berücksichtigt werden. 


# Programm

Benutzung des Programms `prog`, wenn es kompiliert ist:

```bash
prog input [output]
```

wobei

* `input` : ein Pfad zur Inputdatei ist, welches das im Abschnitt **Input** foglende Format haben muss
* `output` : ein optionaler Pfad zu einem Ordner ist, in welchen die Ouptut Dateien gespeichert werden. By default wird ein Ordner `out/` erstellt und die Dateien darin gespeichert.

## Input

der Input ist in folgende Abschnitte geteilt:

* Schwimmhalle : beginnt mit dem Token `>Schwimmhalle`
* Athletikraum : beginnt mit dem Token `>Athletik`
* Gebäude in der Nähe : beginnt mit dem Token `>Nahe`
* Gruppen : beginnt mit dem Token `>Gruppe`
* Trainerabwesenheit : beginnt mit dem Token `>Trainerabwesenheit`
* Trainer nur an einem Tag da : beginnt mit dem Token `>EinTag`

alle Token können in `src/Constants.cpp` angepasst werden (muss dann neu kompiliert werden).

Die folgenden Abschnitte zeigen, wie die Zeilen innerhalb eines solchen Abschnitts formatiert sein müssen.

`_` geben dabei Leerzeichen an.

### Schwimmhalle

```bash
>Schwimmhalle LABEL
...
TAG_VON_BIS_BAHN
...
```

wobei 

* `LABEL` : eine Bezeichnung für diese Schwimmhalle ist
* `TAG` : die ersten zwei Buchstaben des Wochentags sind (`Mo`, `Di`, ...)
* `VON` : die Uhrzeit ist, von wann die Bahn reserviert ist
* `BIS` : die Uhrzeit ist, bis wann die Bahn reserviert ist
* `BAHN` : eine Ganzzahl für die Bahn ist

`VON` und `BIS` sind dabei Gleitkommazahlen. Es kann beispielsweise `15:30` als `15.5` oder `20:45` als `20.75` ausgedrückt werden. Zu beachten ist, dass hier ein Punkt (`.`) statt Komma (`,`) verwendet werden muss.

mehrere Schwimmhallen können einfach dadurch getrennt werden, dass ein neues `>Schwimmhalle`-Token mit neuem `LABEL` eingefügt wird.

### Athletikraum

```bash
>Athletik LABEL
...
TAG_VON_BIS
...
```

wobei

* `LABEL` : eine Bezeichnung für diesen Athletikraum ist
* `TAG` : die ersten zwei Buchstaben des Wochentags sind (`Mo`, `Di`, ...)
* `VON` : die Uhrzeit ist, von wann die Bahn reserviert ist
* `BIS` : die Uhrzeit ist, bis wann die Bahn reserviert ist

`VON` und `BIS` sind dabei Gleitkommazahlen. Es kann beispielsweise `15:30` als `15.5` oder `20:45` als `20.75` ausgedrückt werden. Zu beachten ist, dass hier ein Punkt (`.`) statt Komma (`,`) verwendet werden muss.

mehrere Athletikräume können einfach dadurch getrennt werden, dass ein neues `>Athletik`-Token mit neuem `LABEL` eingefügt wird.

### Gebäude in der Nähe

```bash
>Nahe
...
LABEL1 LABEL2 [ZEIT]
...
```

wobei 

* `LABEL1`, `LABEL2` : die Label der Gebäude in den Abschnitten **Schwimmhalle** und **Athletikraum**, die sich in der Nähe von einander befinden.
* `ZEIT` : ein optionaler Parameter ist, bei dem die Zeit angegeben werden kann, die benötigt wird um von einem Gebäude zum anderen zu kommen.

Benötigt man also eine halbe Stunde von `SH1` nach `AT1`, kann man die Zeile `SH1 AT1 0.5` angeben. Wird der Paramter `ZEIT` nicht angeben, wird keine Zeit eingerechnet, um das Gebäude zu wechseln.

### Gruppen

```bash
>Gruppe
...
TRAINER_ALTER_#WASSER_#BAHNEN_#ATHL
...
```

wobei 

* `TRAINER` : eine Bezeichnung für den Trainer ist
* `ALTER` : einer der folgenden Werte ist: `Kind`, `Jugend`, `AlterSack`, was die Altersgruppe der Trainingsgruppe beschreibt.
* `#WASSER` : die Anzahl der Wassertrainingseinheiten dieser Gruppe
* `#BAHNEN` : die Anzahl der Bahnen, welche die Trainingsgruppe parallel haben soll, wenn sie im Wasser ist.
* `#ATHL` : die Anzahl der Athletikeinheiten dieser Gruppe (muss kleiner oder gleich `#WASSER` sein.)

Hat ein Trainer mehrere Gruppen, muss hier die gleiche Bezeichnung gewählt werden.
Die Bezeichungen für den `ALTER`-Parameter können in `src/Constants.cpp` geändert werden (muss dann neu kompiliert werden.)

### Trainerabwesenheit

```bash
>Trainerabwesenheit
...
TRAINER_T1_T2_...
...
```

wobei 

* `TRAINER` : die Bezeichnung eines Trainers aus dem Abschnitt **Gruppe** ist.
* `T1`, `T2`, ... : die ersten zwei Buchstaben der Tage an denen dieser Trainer nicht in der Schwimmhalle sein kann.

Kann ein Trainer `TR` nur Montag und Dienstag, kann also folgende Zeile angeben werden: `TR Mi Do Fr Sa So`

### Trainer nur an einem Tag da

```bash
>EinTag
...
TRAINER
...
```

wobei 

* `TRAINER` : die Bezeichnung des Trainers ist, welcher nur einem Tag in der Woche in der Schwimmhalle stehen möchte.

Hier ergibt sich ein Konflikt, wenn dieser Trainer Gruppen mit mehr als einmal Training die Woche hat.